#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "process.h"

void syscall_init (void);
bool arg_valid (const void*);
void syscall_halt (void);
void syscall_exit (int);
tid_t syscall_exec (const char *);
static int syscall_wait (tid_t);
int syscall_read (int, void* , unsigned);
int syscall_write (int, const void*, unsigned);
int fibonacci (int);
int max_of_four_int (int, int, int, int);
bool syscall_create (const char*, unsigned);
bool syscall_remove (const char*);
int syscall_open (const char*);
void syscall_close (int);
int syscall_filesize (int);
void syscall_seek (int, unsigned);
unsigned syscall_tell (int);
#endif /* userprog/syscall.h */
