#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

/* Manipulation for Project1. */
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

#include "../filesys/file.h"
#include "../filesys/filesys.h"
#include "../filesys/off_t.h"
#include <string.h>

struct file 
  {
    struct inode *inode;        /* File's inode. */
    off_t pos;                  /* Current position. */
    bool deny_write;            /* Has file_deny_write() been called? */
  };

struct lock filesys_lock;

static void syscall_handler (struct intr_frame *);

/* Manipulation for Project1. */
bool arg_valid (const void* arg);
void syscall_halt (void);
void syscall_exit (int);
tid_t syscall_exec (const char *);
static int syscall_wait (tid_t);
int syscall_read (int, void* , unsigned);
int syscall_write (int, const void*, unsigned);
int fibonacci (int);
int max_of_four_int (int, int, int, int);


void
syscall_init (void) 
{
  lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

bool arg_valid(const void* uaddr)
{
  if(!uaddr) 
    return false;
  if(!is_user_vaddr(uaddr)) 
    return false;
  //if(!(pagedir_get_page(thread_current()->pagedir,  uaddr))) return false;
  return true;
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
  //printf ("syscall num : %d\n", *(uint32_t*)(f->esp));
  //hex_dump(f->esp, f->esp, 100, true);
  //if(!arg_valid(f->esp)) syscall_exit(-1);
  switch(*(uint32_t*)(f->esp))
    {
    case SYS_HALT:
      //printf("halt\n");
      syscall_halt();
      break;
    case SYS_EXIT:
      //printf("exit\n");
      if (!arg_valid(f->esp +4)) 
        syscall_exit (-1);
      syscall_exit(*(uint32_t*)(f->esp+4));
      break;
    case SYS_EXEC: 
      //printf("exec\n");
      if (!arg_valid(f->esp+4))
        syscall_exit(-1);  
      f->eax = syscall_exec((char*)*(uint32_t*)(f->esp+4));
      break;
    case SYS_WAIT: 
      //printf("wait\n");
      if (!arg_valid(f->esp +4)) 
        syscall_exit(-1);
      f-> eax = syscall_wait(*(uint32_t*)(f->esp+4));
      break;
    case SYS_READ:
      //printf("read\n");
      // if (!arg_valid(f->esp + 4) || !arg_valid(f->esp + 8) || !arg_valid(f->esp + 12)) 
      //   syscall_exit(-1);
      f->eax = syscall_read ((int)*(uint32_t*)(f->esp+4),(void*)*(uint32_t*)(f->esp+8),
	  			(unsigned)*(uint32_t*)(f->esp+12));
      break;
    case SYS_WRITE:
      //printf("write\n");
      // if (!arg_valid(f->esp + 4) || !arg_valid(f->esp + 8) || !arg_valid(f->esp + 12)) 
      //   syscall_exit(-1);
      f->eax = syscall_write ((int)*(uint32_t*)(f->esp+4),(void*)*(uint32_t*)(f->esp+8),
	  			(unsigned)*(uint32_t*)(f->esp+12));
      break;
    /* Additional Implemenation in Project1. */
    case SYS_FIB:
      //printf("fibonacci\n");
      if(!arg_valid(f->esp+4)) 
        syscall_exit(-1);
      f->eax = fibonacci((int)*(uint32_t*)(f->esp+4));
      break;
    case SYS_MAX_FOUR:
      //printf("max of four\n");
      if (!arg_valid(f->esp + 4) || !arg_valid(f->esp + 8) || !arg_valid(f->esp + 12) 
      || !arg_valid(f->esp + 16))
        syscall_exit(-1);
      f->eax = max_of_four_int((int)*(uint32_t*)(f->esp+4), (int)*(uint32_t*)(f->esp+8), (int)*(uint32_t*)(f->esp+12), (int)*(uint32_t*)(f->esp+16));
      break;
    
    case SYS_CREATE:
      if (!arg_valid(f->esp+4) || !arg_valid(f->esp+8))
        syscall_exit(-1);
      f->eax = syscall_create ((const char*)*(uint32_t*)(f->esp+4), (unsigned)*(uint32_t*)(f->esp+8));
      break;
    case SYS_REMOVE:
      if (!arg_valid(f->esp+4))
        syscall_exit (-1);
      f->eax = syscall_remove ((const char*)*(uint32_t*)(f->esp+4));
      break;
    case SYS_OPEN:
      if (!arg_valid(f->esp+4))
        syscall_exit (-1);
      f->eax = syscall_open((const char*)*(uint32_t*)(f->esp+4));
      break;
    case SYS_CLOSE:
      if (!arg_valid(f->esp+4))
        syscall_exit (-1);
      syscall_close ((int)*(uint32_t*)(f->esp+4));
      break;
    case SYS_FILESIZE:
      if (!arg_valid(f->esp+4))
        syscall_exit (-1);
      f->eax = syscall_filesize ((int)*(uint32_t*)(f->esp+4));
      break;
    case SYS_SEEK:
      if (!arg_valid(f->esp+4)||!arg_valid(f->esp+8))
        syscall_exit (-1);
      syscall_seek ((int)*(uint32_t*)(f->esp+4), (unsigned)*(uint32_t*)(f->esp+8));
      break;
    case SYS_TELL:
      if (!arg_valid(f->esp+4))
        syscall_exit (-1);
      f->eax = syscall_tell ((int)*(uint32_t*)(f->esp+4));
      break;
    default: 
      //thread_exit();
      break;
    }
  //thread_exit ();
}

void syscall_halt()
{
  shutdown_power_off();
}

void syscall_exit (int status)
{
  printf("%s: exit(%d)\n", thread_name(), status);
  thread_current()->exit_status = status;
  int i;
  for (i = 3; i < 128; i++)
  {
    if(thread_current()->fd[i]!=NULL)
      syscall_close(i);
  }

  struct thread* t;
  struct list_elem* e;
  for (e = list_begin(&thread_current()->child); e!=list_end(&thread_current()->child); e = list_next(e))
  {
    t = list_entry(e, struct thread, child_elem);
    process_wait(t->tid);
  }
  thread_exit();
}

tid_t syscall_exec (const char *cmd_line)
{
  if(cmd_line == NULL)
    return -1;
  return process_execute(cmd_line);
}

static int syscall_wait (tid_t tid)
{
  return process_wait(tid);
}

int syscall_read (int fd, void* buffer, unsigned size)
{
  int i;

  if(!arg_valid(buffer)) syscall_exit(-1);
  if(!arg_valid(buffer+size)) syscall_exit(-1);
  lock_acquire(&filesys_lock);
  if(fd == 0) // STDIN
    {
      for (i = 0; i < (int)size; i++)
        {
          *((char*)buffer+i) = input_getc();
          if ( *((char*)buffer+i) == '\0')
            break;
        }
      
    }
  else if (fd > 2)
  {
    if(thread_current()->fd[fd]==NULL)
    {
      lock_release(&filesys_lock);
      syscall_exit(-1);
    }
    i = file_read(thread_current()->fd[fd], buffer, size);
  }
  lock_release(&filesys_lock);
  return i; 
}

int syscall_write (int fd, const void* buffer, unsigned size)
{
  //printf("fd: %d\n", fd);
  if(!arg_valid(buffer)) syscall_exit(-1);
  if(!arg_valid(buffer+size)) syscall_exit(-1);
  lock_acquire(&filesys_lock);
  int ret = -1;
  if(fd == 1)
    {
      putbuf((char*)buffer, size);
      ret = size;
    }
  else if (fd > 2) 
    {
      if(thread_current()->fd[fd] == NULL)
      {
        lock_release(&filesys_lock);
        syscall_exit(-1);
      }
      
      // if (thread_current()->fd[fd]->deny_write)
      //   file_deny_write(thread_current()->fd[fd]);
      
      ret = file_write(thread_current()->fd[fd], buffer, size);
    }
  lock_release(&filesys_lock);
  return ret;
}

int fibonacci (int n)
{
  int tmp, i;
  int a = 1; 
  int b = 1;
  if (n ==1 || n== 2) return 1;
  else
    {
      for(i = 2 ; i<= n; i++)
        {
          tmp = a + b;
          a = b; 
          b = tmp;
        }
      return b;
    } 
    
}

int max_of_four_int (int a, int b, int c, int d)
{
  int i, max=0;
  int sample[4]={a, b, c, d};
  for (i = 0; i <4; i++)
    {
      if(max<sample[i])
        max = sample[i];
    }
  return max;
}

bool syscall_create(const char* file, unsigned initial_size)
{
  if (file == NULL)
    syscall_exit(-1);
  
  if(!arg_valid(file))
    syscall_exit(-1);
  return filesys_create(file, initial_size);
}

bool syscall_remove (const char* file)
{
  if (file == NULL)
    syscall_exit(-1);

  if(!arg_valid(file))
    syscall_exit(-1);

  return filesys_remove(file);
}

int syscall_open (const char* file)
{
  if (file == NULL)
    syscall_exit(-1);

  if(!arg_valid(file))
    syscall_exit(-1);
  
  int i;
  struct file* fp = filesys_open(file);
  if (fp == NULL)
    return -1;
  lock_acquire(&filesys_lock);
  for (i = 3 ; i < 128; i++)
  {
    if (thread_current()->fd[i] == NULL)
    {
      if (strcmp(thread_current()->name, file) == 0)
        file_deny_write(fp);
      thread_current()->fd[i] = fp;
      lock_release(&filesys_lock);
      return i;
    }
  }
  
  lock_release(&filesys_lock);
  return -1;
}

int syscall_filesize (int fd)
{
  if (thread_current()->fd[fd] == NULL)
    syscall_exit(-1);
    
  return file_length(thread_current()->fd[fd]);
}

void syscall_seek (int fd, unsigned position)
{
  if (thread_current()->fd[fd] == NULL)
    syscall_exit(-1);
    
  file_seek(thread_current()->fd[fd], position);
}

unsigned syscall_tell (int fd)
{
  if (thread_current()->fd[fd] == NULL)
    syscall_exit(-1);
    
  return file_tell(thread_current()->fd[fd]);
}

void syscall_close (int fd)
{
  if (thread_current()->fd[fd] == NULL)
    syscall_exit(-1);

  thread_current()->fd[fd] = NULL;
  return file_close(thread_current()->fd[fd]);
}