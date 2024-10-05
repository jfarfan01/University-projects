#pragma once
#ifndef ASS4_MAGICCONCHSHELL_H
#define ASS4_MAGICCONCHSHELL_H

#include "util.h"

/*
 * Initialization of global variables
 */
filedescriptors fds = { -1, -1, -1};
mappings mmaps = {NULL, NULL, NULL};
pid_t process_id = -1;

/*
 * Various headers for used functions in MagicConchShell.c
 */
int startMagicConchShell();
void initSharedMemoriesMagicConchShell();
void initMmapingsMagicConchShell();
void closeMmapingsMagicConchShell();
char* dispatcher(shmrequest* request);

#endif //ASS4_MAGICCONCHSHELL_H
