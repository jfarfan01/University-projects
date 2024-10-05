/*
 * You don't have to change anything in this file!
 * Feel free to play around with parameters for testing purposes.
 */

#ifndef A2_DEFINITIONS_H
#define A2_DEFINITIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#include "resources.h"
#include "vector.h"

#define UPPER_LIMIT_TUTORS    1000
#define UPPER_LIMIT_STUDENTS  2000
#define LOWER_LIMIT              1

#define NUM_TUTORS     "num_tutors"
#define NUM_STUDENTS "num_students"

#define SECOND_CHANCE_STUDENTS_CAPACITY 2

#define MAX_POINTS 100
#define POSITIVE_POINTS 55
#define NO_ENTRY -2

#define NR_ARGUMENTS 3
#define ERROR -1

// ------------------------------------------------------------------------
void checkAllowedRange(ssize_t value, ssize_t upper_limit, char* name)
{
  if (value < LOWER_LIMIT || value > upper_limit)
  {
    fprintf(stderr, "%s allowed range: [%d,%zd].\n", name, LOWER_LIMIT, upper_limit);
    exit(ERROR);
  }
}

// ------------------------------------------------------------------------
void checkParametersValidity(ssize_t num_tutors, ssize_t num_students) 
{
  checkAllowedRange(num_tutors, UPPER_LIMIT_TUTORS, NUM_TUTORS);
  checkAllowedRange(num_students, UPPER_LIMIT_STUDENTS, NUM_STUDENTS);
}

// ------------------------------------------------------------------------
void handleParameters(int argc, char* argv[], ssize_t* num_tutors, ssize_t* num_students)
{
  if (argc != NR_ARGUMENTS) 
  {
    fprintf(stderr, "Usage: %s <num_tutors> <num_students>\n", argv[0]);
    exit(ERROR);
  }

  // check the validity of parameters
  *num_tutors = atoi(argv[1]);
  *num_students = atoi(argv[2]);

  checkParametersValidity(*num_tutors, *num_students);
}

// ------------------------------------------------------------------------
void createTutors(Tutor* tutors, ssize_t num_tutors, void* start_routine)
{
  for (ssize_t i = 0; i < num_tutors; i++) 
  {
    tutors[i].id = i;
    tutors[i].interviews_done = false;
    assert(!pthread_create(&tutors[i].tutor, NULL, start_routine, (void*)&tutors[i]));
  }
}

// ------------------------------------------------------------------------
void createStudents(Student* students, ssize_t num_students, void* start_routine)
{
  for (ssize_t i = 0; i < num_students; i++) 
  {
    students[i].id = i;
    students[i].points = NO_ENTRY;
    assert(!pthread_create(&students[i].student, NULL, start_routine, (void*)&students[i]));
  }
}

// ------------------------------------------------------------------------
void freeResources(Tutor* tutors, Student* students, vector* free_desks)
{
  free(tutors);
  free(students);
  
  vector_iterator it = vector_begin(free_desks);
  while (it != vector_end(free_desks)) 
  {
    free(*it);
    vector_erase(free_desks, it);
  }
  
  vector_destroy(free_desks);
}

// ------------------------------------------------------------------------
void showStudentID() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (20 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

// ------------------------------------------------------------------------
void checkStudentID() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (7 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

// ------------------------------------------------------------------------
ssize_t isBreakNeeded() 
{
  return rand() % 2;
}

// ------------------------------------------------------------------------
ssize_t getPoints()  
{
  return rand() % MAX_POINTS;
}

// ------------------------------------------------------------------------
void coffeeBreak() 
{
  ssize_t microsec_to_sleep = 1000 + rand() % (15 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);
}

// ------------------------------------------------------------------------
void interviewTheStudent()
{
  ssize_t microsec_to_sleep = 1000 + rand() % (15 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);  
}

// ------------------------------------------------------------------------
void secondChanceInterview()
{
  ssize_t microsec_to_sleep = 1000 + rand() % (13 * 1000);
  nanosleep((const struct timespec[]){{0, 1000L * microsec_to_sleep}}, NULL);  
}

#endif //A2_DEFINITIONS_H
