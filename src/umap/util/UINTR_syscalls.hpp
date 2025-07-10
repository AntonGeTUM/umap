#pragma once

#include <unistd.h>
#include <sys/syscall.h>
#include <cstdint>
#include <x86intrin.h>

#ifndef __NR_uintr_register_handler
#define __NR_uintr_register_handler 471
#define __NR_uintr_unregister_handler 472
#define __NR_uintr_create_fd 473
#define __NR_uintr_register_sender 474
#define __NR_uintr_unregister_sender 475
#define __NR_uintr_wait 476
#endif

inline long uintr_register_handler(void* handler, uint64_t flags) {
    return syscall(__NR_uintr_register_handler, handler, flags);
}

inline long uintr_unregister_handler(uint64_t flags) {
    return syscall(__NR_uintr_unregister_handler, flags);
}

inline long uintr_create_fd(int vector, uint64_t flags) {
    return syscall(__NR_uintr_create_fd, vector, flags);
}

inline long uintr_register_sender(int fd, uint64_t flags) {
    return syscall(__NR_uintr_register_sender, fd, flags);
}

inline long uintr_unregister_sender(int ipi_idx, uint64_t flags) {
    return syscall(__NR_uintr_unregister_sender, ipi_idx, flags);
}

inline long uintr_wait(uint64_t flags) {
    return syscall(__NR_uintr_wait, flags);
}
