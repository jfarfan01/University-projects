#pragma once

#include <types.h>
#define PTHREAD_CANCELLED -10


class Syscall
{
  public:
    static size_t syscallException(size_t syscall_number, size_t arg1, size_t arg2, size_t arg3, size_t arg4, size_t arg5);

    static void exit(size_t exit_code);
    static void outline(size_t port, pointer text);

    static size_t write(size_t fd, pointer buffer, size_t size);
    static size_t read(size_t fd, pointer buffer, size_t count);
    static size_t close(size_t fd);
    static size_t open(size_t path, size_t flags);
    static void pseudols(const char *pathname, char *buffer, size_t size);

    static size_t createprocess(size_t path, size_t sleep);
    static void trace();
    static size_t spawnThread(size_t wrapper, size_t routine, size_t args);
    static void pthread_exit(void* value_ptr);
    static size_t cancel_thread(size_t target_tid);
    static size_t joinThread(size_t tid, void** value_ptr);
    static size_t setCancelState(size_t state, int* oldstate);
    static size_t setCancelType(size_t type, int* oldtype);

};

