#pragma once

#define fd_stdin 0
#define fd_stdout 1
#define fd_stderr 2

#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DISABLE 1
 
#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 1

#define sc_exit 1
#define sc_fork 2
#define sc_read 3
#define sc_write 4
#define sc_open 5
#define sc_close 6
#define sc_lseek 19
#define sc_pseudols 43
#define sc_outline 105
#define sc_sched_yield 158
#define sc_createprocess 191
#define sc_trace 252

#define sc_pthread 2000
#define sc_pthread_create sc_pthread + 1
#define sc_pthread_exit sc_pthread + 2
#define sc_pthread_cancel sc_pthread + 3
#define sc_pthread_join sc_pthread + 4
#define sc_pthread_setcancelstate sc_pthread + 5
#define sc_pthread_setcanceltype sc_pthread + 6

typedef void*(*funPointer)(void*);
