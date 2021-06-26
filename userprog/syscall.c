#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <list.h>
#include <stdbool.h>

#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/synch.h"
#include "devices/shutdown.h"
#include "process.h"
#include "threads/vaddr.h" 
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "devices/input.h"


#include "../filesys/file.h"
#include "../filesys/filesys.h"
#include "../filesys/inode.h"
#include "../filesys/off_t.h"
static void syscall_handler (struct intr_frame *);
void check_user_vaddr(const void *vaddr);
void exit(int status);
int wait(int pid);
int exec(const char *cmd_line);
int max_of_four_integers(int a, int b, int c, int d);
int fibonacci(int n);
int write(int fd, const void *buffer, unsigned size);
int read(int fd, void *buffer, unsigned size);
bool create(const char *file, unsigned initial_size);
bool remove(const char *file);
int open(const char *file);
int filesize(int fd);
void seek(int fd, unsigned position);
unsigned tell(int fd);
void close(int fd);

struct file
{
	struct inode *inode;
	off_t pos;
	bool deny_write;
};

struct lock file_lock;

void
syscall_init (void) 
{
  	lock_init(&file_lock);
	intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //printf ("system call!\n");
  //if(!is_user_vaddr(f->esp)) {printf("error!\n"); thread_exit();}

  int syscall_num = *((int*)f->esp);
  //int memory_flag = 0;
  //hex_dump(f->esp, f->esp, 100, 1); 
  
  //printf("syscall_num: %d\n", syscall_num);
  //hex_dump((uintptr_t*)(f->esp), (int*)(f->esp), sizeof(char) * 100, true);

  switch(syscall_num){
	  case SYS_HALT:
		  shutdown_power_off();
		  break;

	  case SYS_EXIT:;
		  check_user_vaddr(f->esp + 4);
		  /*
		  int status = *((int*)f->esp + 1);
		  char *save_ptr;
		  char *ptr = strtok_r(thread_name(), " ", &save_ptr);	

		  char parse_thread_name[256];
		  strlcpy(parse_thread_name, ptr, &save_ptr);
		  
		  printf("%s: exit(%d)\n", parse_thread_name, status);
		  thread_current()->exit_status = status;
		  thread_exit();
		  //f->eax = status;
		  */
		  exit(*(uint32_t *)(f->esp + 4));
		  break;

	  case SYS_EXEC:;
		  check_user_vaddr(f->esp + 4);
		  //char file[20];
		  //strlcpy(file, ((int*)f->esp + 1), strlen((int*)f->esp + 1));
		  //printf("file: %s\n", file);
		  //f->eax = process_execute((const char*)*(int*)(f->esp + 1));
		  f->eax = exec((const char*)*(uint32_t *)(f->esp + 4));
		  //printf("file: %s\n", file);
		  //f->eax = exec(file);
		  
		  break;
	 
	  case SYS_WAIT:;
		  check_user_vaddr(f->esp + 4);
		  f->eax = wait(*(uint32_t*)(f->esp + 4));
		  break;

	  case SYS_WRITE: ;
		  check_user_vaddr((int*)f->esp + 1);
		  check_user_vaddr((int*)f->esp + 2);
		  check_user_vaddr((int*)f->esp + 3);
	 	  
		  int fd = *((int*)f->esp + 1);
		  //printf("fd check\n");
		  void* buffer = (void*)(*((int*)f->esp + 2));
		  //printf("buffer check\n");
		  //hex_dump(f->esp, f->esp, 30, true);
		  unsigned size = *((unsigned*)f->esp + 3);
		  //printf("fd: %d buffer: %s size: %d\n", fd, buffer, size);
		  /*
		  if(fd == 1){
		  	putbuf(buffer, size);
		        f->eax = size;
		  }
		  */
		  f->eax = write(fd, buffer, size);
		  break;

	  case SYS_READ:;
		  check_user_vaddr((int*)f->esp + 1);
		  check_user_vaddr((int*)f->esp + 2);
		  check_user_vaddr((int*)f->esp + 3);

		  fd = *((int*)f->esp + 1);
		  buffer = (void*)(*((int*)f->esp + 2));
		  size = *((unsigned*)f->esp + 3);
		  /*
		  if(fd == 0){
			int i;
		  	for(i = 0; i < size; i++){
				*(uint8_t*)(buffer + i) = input_getc();
			//	if(((char*)buffer)[i] == '\0') break;
			}
			f->eax = i;
		  }
		  */
		  f->eax = read(fd, buffer, size);
		  break;
	  case SYS_FIBONACCI:;
	        
		check_user_vaddr((int*)f->esp + 1);
		//printf("hello, I'm fibonacci!\n");
		int n = *((int*)f->esp + 1);
	        f->eax = fibonacci(*((int*)f->esp + 1));
		break;

	  case SYS_SUM_INT:;
		check_user_vaddr((int*)f->esp + 6);
		check_user_vaddr((int*)f->esp + 7);
		check_user_vaddr((int*)f->esp + 8);
         	check_user_vaddr((int*)f->esp + 9);
	     	//printf("hello, I'm sum!\n");
		int n1 = *((int*)f->esp + 6);
		int n2 = *((int*)f->esp + 7);
		int n3 = *((int*)f->esp + 8);
		int n4 = *((int*)f->esp + 9);
	 	//printf("n1: %d n2: %d n3: %d n4: %d\n", n1, n2, n3, n4);
		int ans = max_of_four_integers(n1, n2, n3, n4);
		f->eax = ans;
		break;

	  case SYS_CREATE:;
		check_user_vaddr(f->esp + 4);
		check_user_vaddr(f->esp + 8);
		
		//check_user_vaddr(f->esp + 16);
		//check_user_vaddr(f->esp + 20);
		f->eax = create(*(const char**)(f->esp + 4), *(unsigned*)(f->esp+8));
		break;

	  case SYS_REMOVE:;
		check_user_vaddr(f->esp + 4);
		f->eax = remove(*(const char**)(f->esp + 4));
		break;

	  case SYS_OPEN:;
		check_user_vaddr(f->esp + 4);
		f->eax = open(*(const char**)(f->esp + 4));
		break;

	  case SYS_FILESIZE:;
		check_user_vaddr(f->esp + 4);
		f->eax = filesize(*(const char**)(f->esp + 4));
		break;

	  case SYS_SEEK:;
		check_user_vaddr(f->esp + 4);
		check_user_vaddr(f->esp + 8);
		seek(*(int*)(f->esp + 4), *(unsigned*)(f->esp + 8));
		break;

	  case SYS_TELL:;
		check_user_vaddr(f->esp + 4);
		f->eax = tell(*(int*)(f->esp + 4));
		break;

	  case SYS_CLOSE:;
		check_user_vaddr(f->esp + 4);
		close(*(int*)(f->esp + 4));
		break;
	  default: ;
  }

  /* 요부분 주석 처리함 */
 //thread_exit ();
}

void check_user_vaddr(const void *vaddr){
	//printf("vaddr: %s\n", (char*)vaddr);	
	if(!is_user_vaddr(vaddr)){
		/*
		char *save_ptr;
		char *ptr = strtok_r(thread_name(), " ", &save_ptr);	

		char parse_thread_name[256];
		strlcpy(parse_thread_name, ptr, &save_ptr);
		
		printf("happen!\n");
		printf("%s: exit(-1)\n", parse_thread_name);
		thread_exit();
		*/
		exit(-1);
	}
}

void exit(int status) {
	char *save_ptr;
	char *ptr = strtok_r(thread_name(), " ", &save_ptr);	
	printf("%s: exit(%d)\n", ptr, status);
	
	thread_current()->exit_status = status;
	int fdFirst = 3; int fdLast = 128;
	for(int i=fdFirst; i<fdLast; ++i){
		if(thread_current()->file_des[i] != NULL){
			close(i);
		}
	}
	thread_exit();
}

int exec(const char *cmd_line){
	int new_tid;
	new_tid = process_execute(cmd_line);
	return new_tid;
}

int wait(int pid){
	return process_wait(pid);
}

int fibonacci(int n)
{
	int n0, n1, nt;
	n0 = 1;
	n1 = 1;
	if(n == 1) return n0;
	else if(n == 2) return n1;
	else{
		n -= 2;
		while(n)
		{
			nt = n1;
			n1 = n1 + n0;
			n0 = nt;
			n -= 1;
		}
		return n1;
	}
	return -1;
}

int max_of_four_integers(int a, int b, int c, int d)
{
	int max = -10000;
	int data[4]; data[0] = a; data[1] = b; data[2] = c; data[3] = d;
	for(int i=0; i<4; ++i){
		if(max <= data[i]) max = data[i];
	}
	return max;
		
}

int read(int fd, void* buffer, unsigned size){
	check_user_vaddr(buffer);
	int retVal;
	int i;
	struct file *f = thread_current()->file_des[fd];
	
	if(fd == 0){
		for(i=0; i<size; ++i){
			if(((char*)buffer)[i] == '\0'){
				break;
			}
		}
		retVal = i;
	}
	if(fd > 2){
		if(f == NULL){
			//lock_release(&fileLock);
			exit(-1);
		}
		retVal = file_read(f, buffer, size);
	}
	return retVal;
}

int write(int fd, const void *buffer, unsigned size){
	check_user_vaddr(buffer);
	int retVal;
	struct file *f = thread_current()->file_des[fd];
	
	if(fd == 1){
		putbuf(buffer, size);
		retVal = size;
	}
	if(fd > 2){
		if(f == NULL){
			exit(-1);
		}
		
		if(f->deny_write){
			file_deny_write(f);
			//return 0;
		}
		
		retVal = file_write(f, buffer, size);
	}
	
	return retVal;
}

bool create(const char *file, unsigned initial_size)
{
	if(file == NULL) {
		exit(-1);
	}
	check_user_vaddr(file);
	
	bool flag = filesys_create(file, initial_size);
	return flag;
}

bool remove(const char *file)
{
	if(file == NULL) exit(-1);
	check_user_vaddr(file);
	
	bool flag = filesys_remove(file);
	return flag;
}

int open(const char *file){
	int i; int retVal = -1;
	if(file == NULL){
		exit(-1);
	}
	check_user_vaddr(file);


	struct file* f = filesys_open(file);
	if(f == NULL){
		retVal = -1;
	}
	else{
		int fdFirst = 3; int fdLast = 128;
		struct thread *t = thread_current();

		for(i=fdFirst; i<fdLast; ++i){
			if(t->file_des[i] == NULL){
				//printf("filename: %s\n", file);	
				
				if(!strcmp(t->name, file)){
					file_deny_write(f);
				}
				
				//if(is_executable_file(file)) file_deny_write(f);

				t->file_des[i] = f;
				retVal = i;
				break;
			}
		}
	}
	return retVal;
}

int filesize(int fd){
	struct file *f = thread_current()->file_des[fd];
	if(fd < 0 || fd > 128) exit(-1);
	if(f == NULL){
		exit(-1);
	}
	int retVal = file_length(f);
	return retVal;
}

void seek(int fd, unsigned position){
	struct file *f = thread_current()->file_des[fd];
	if(thread_current()->file_des[fd] == NULL){
		exit(-1);
	}
	file_seek(f, position);
}

unsigned tell(int fd){
	struct file *f = thread_current()->file_des[fd];
	if(f == NULL){
		exit(-1);
	}
	unsigned retVal = file_tell(f);
	return retVal;
}

void close(int fd){
	struct file* f;
	f = thread_current()->file_des[fd];
	if(f == NULL){
		exit(-1);
	}
	f = NULL;
	file_close(f);
}
