#define __USE_GNU
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <thread>
#include <cinttypes>
#include <atomic>
#include <iomanip>

#include "util/UINTR_syscalls.hpp"

int count;
int received;
int descriptor = 0;
constexpr int NUMS_ITER = 100;

void __attribute__((interrupt))
    __attribute__((target("general-regs-only", "inline-all-stringops")))
    ui_handler(struct __uintr_frame *ui_frame, unsigned long long vector) {

    std::cout << "uintr_frame:\n";
    std::cout << "  Addr:   " << &ui_frame << std::endl;
    std::cout << "  RIP:    0x" << std::hex << ui_frame->rip << std::endl;
    std::cout << "  RFLAGS: 0x" << ui_frame->rflags << std::endl;
    std::cout << "  RSP:    0x" << ui_frame->rsp << std::endl;
    std::cout << std::dec;

    count++;
    received = 1;
}

void sender() 
{
    std::cout << "Registering Sender\n";
    uint64_t uipi_index = uintr_register_sender(descriptor, 0);
    if (uipi_index < 0) {
        std::cerr << "Failed to register sender\n";
        std::exit(-1);
    } else {
        std::cout << "UIPI Index: " << uipi_index << std::endl;
    }

    std::cout << "Sending UINTR" << std::endl;
    for (int i = 0; i < NUMS_ITER; ++i) {
        received = 0;

        _senduipi(uipi_index);
        while (received == 0) {

        }
    }
}

int main()
{
    std::cout << "Registering handler" << std::endl;
    int handler = uintr_register_handler(reinterpret_cast<void*>(ui_handler), 0);
    if (handler < 0) {
        std::cerr << "Failed to register handler" << std::endl;
        std::exit(-1);
    } else {
        std::cout << "Handler registered successfully: " << handler << std::endl;
    }

    std::cout << "Creating file descriptor" << std::endl;
    descriptor = uintr_create_fd(0, 0);
    if (descriptor < 0) {
        std::cerr << "Failed to create file descriptor" << std::endl;
        std::exit(-1);
    } else {
        std::cout << "File descriptor created successfully: " << descriptor << std::endl;
    }

    std::cout << "Enabling UINTR" << std::endl;
    _stui();

    std::cout << "Creating sender thread" << std::endl;
    std::thread sender_thread;
    try {
        sender_thread = std::thread(sender);
    } catch (const std::system_error& e) {
        std::cerr << "Error creating sender thread: " << e.what() << std::endl;
        return -1;
    }

    
    std::cout << "Waiting for Sender" << std::endl;
    while (count < NUMS_ITER) {

    }
    sender_thread.join();
    std::cout << "Interrupts handled: " << count << std::endl;

    return 0;
}