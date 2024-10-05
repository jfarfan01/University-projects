#ifndef ASS4_UTIL_H
#define ASS4_UTIL_H

#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <semaphore.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

/*
 * Use those defines for the name required
 * to open shared objects
 */
#define SHM_NAME_REQUEST "/request"
#define SHM_NAME_RESPONSE "/response"
#define SHM_NAME_LOCKS "/locks"
#define SHM_NAME_STATE "/state"

/*
 * Other defines. Use whenever possible.
 */
#define RESPONSE_MAX_LENGTH 45
#define REQUEST_MAX_LENGTH 30

/*
 * Can be used for testing
 */
#define TEST_STR "DEAD"
#define LENGTH_TEST_STR 0x4

/*
 * Important and useful predefined values
 * for shared objects
 */
#define FLAGS_SHM_READONLY O_RDONLY | O_CREAT
#define FLAGS_SHM_READWRITE O_RDWR | O_CREAT
#define MODERW S_IRUSR | S_IWUSR

/*
 *  Error codes you can use for testing.
 */
#define ERROR_SHM_USER -1
#define ERROR_MMAP_USER -3
#define ERROR_RES_USER -5
#define ERROR_UNMAP_CLOSE_USER -7

#define ERROR_SHM_MAGICCONCHSHELL -2
#define ERROR_MMAP_MAGICCONCHSHELL -4
#define ERROR_REQ_MAGICCONCHSHELL -6
#define ERROR_UNMAP_CLOSE_MAGICCONCHSHELL -8


/*
 * file descriptors of your shared objects
 */
typedef struct
{
  int fd_shm_request_;
  int fd_shm_state_;
  int fd_shm_response_;
  int fd_shm_locks_;
} filedescriptors;

/*
 * shared objects for your locks / semaphores / cond variables (you don't necessarily need them all)
 */
typedef struct
{
  // TODO Student START
  sem_t user_can_strcpy;
  sem_t user_wrote_request;
  sem_t conch_sent_reponse;
  pthread_mutex_t reponse_mutex;
  
  // TODO Student END
} shmlocks;

/*
 * the response struct
 * can be extended if wanted
 */
typedef struct
{
  char message[RESPONSE_MAX_LENGTH];
} shmresponse;

/*
 * the state struct
 * can be extended if wanted
 */
typedef struct
{
  int motivated;
} shmstate;

/*
 * the request struct
 * can be extended if wanted
 */
typedef struct
{
  char message[REQUEST_MAX_LENGTH];
} shmrequest;

/*
 *the according address pointing to the mapped region
 */
typedef struct
{
  shmrequest *mapped_region_request_;
  shmstate *mapped_region_state_;
  shmresponse *mapped_region_response_;
  shmlocks *mapped_region_locks_;
} mappings;

/*
 * global variables.You MUST USE them!!!!
 */
extern filedescriptors fds;
extern pid_t process_id;
extern mappings mmaps;

/*
 * check functions you should not move in the code (you can implement your own functionality in util.c)
 */
void removeAllSHM();
void checkResults();
int checkSetup();
void checkSHMMagicConchShell();
void checkMMAPMagicConchShell();
void checkSHMUser();
void checkMMAPUser();
void checkMagicConchShell();
void checkCleanup();

int checkProgress();
int checkProgressUser();
int checkProgressMagicConchShell();

extern int startUser();
extern int startMagicConchShell();

/*
 * Useful functions to use
 */
int getCommand(char *buffer);

#endif //ASS4_UTIL_H
