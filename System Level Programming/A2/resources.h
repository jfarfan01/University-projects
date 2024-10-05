
#ifndef A2_RESOURCES_H
#define A2_RESOURCES_H

#include "stdint.h"
#include "stdlib.h"
#include "vector.h"
#include <stdbool.h>
#include <assert.h>

/* STUDENT TODO:
 *  add any needed synchronization primitives here
 */

typedef struct {
  pthread_t student;
  ssize_t id;
  ssize_t points;
  sem_t personal_Semaphore;
} Student;

typedef struct{
  pthread_t tutor;
  ssize_t id;
  bool interviews_done;
  sem_t personal_Semaphore;
} Tutor;

typedef struct {
  ssize_t id;
  Tutor* tutor;
  Student* student;
  bool active; 
} Desk;

#endif //A2_RESOURCES_H
