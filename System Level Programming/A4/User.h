#pragma once
#ifndef ASS4_USER_H
#define ASS4_USER_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = {-1, -1, -1};
mappings mmaps = {NULL, NULL, NULL};
pid_t process_id = -1;

/*
 * Various headers for used functions in User.c
 */
int startUser();
void initProcess();
void initSharedMemoriesUser();
void initMmapingsUser();
void initLocks();
int getCommand(char *buffer);
void closeMmapingsUser();

#endif //ASS4_USER_H
