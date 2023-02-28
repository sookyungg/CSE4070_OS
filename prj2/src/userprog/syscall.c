#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void syscall_init (void){
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&file_lock);
}

int sys_read (int fd, void *buffer, unsigned length){
	int i;
	if(fd == 0){
		for(i=0; i<(int)length; i++){
			uint8_t c = input_getc();
			if(c=='\0') break;
		} return i;
	}
	else return -1;
}

int sys_write (int fd, const void *buffer, unsigned length){
	if(fd == 1){
		putbuf((char*)buffer, (size_t)length);
		return length;
	}
	else return -1;
}


static void syscall_handler (struct intr_frame *f){
	void *addr = (f->esp);
	int syscall = (*(uint64_t *)addr);
    int flag = 0;
	struct file *fp;
	pid_t temp; 

	switch(syscall){
		case SYS_HALT:
			shutdown_power_off();
			break;

		case SYS_EXIT:
			if(!is_user_vaddr(f->esp+4)) sys_exit(-1);
			sys_exit((int)*(uint64_t*)(f->esp+4));
			break;

		case SYS_EXEC :
			if(!is_user_vaddr(f->esp+4)) sys_exit(-1);
			f->eax = ((char*)*(uint64_t*)(f->esp+4));
			break;

		case SYS_WAIT :
			if(!is_user_vaddr(f->esp+4)) sys_exit(-1);
			temp = (int)*(uint64_t*)(f->esp+4);
			f->eax = process_wait((int)*(uint64_t*)(f->esp+4));
			break;

		case SYS_READ:
			if(is_user_vaddr((void*)*(uint32_t*)(f->esp+8)) == 0) sys_exit(-1);
			lock_acquire(&file_lock);
			if(!(int)*(uint32_t*)(f->esp+4)){
				
                int i = 0;
				while(i<(unsigned)*(uint32_t*)(f->esp+12)){
					uint8_t c = input_getc();
					if(c=='\0') {
						f->eax = i;
						break;
					}
					i++;
				}lock_release(&file_lock);
				break;
			}
			else if((int)*(uint32_t*)(f->esp+4) > 2){
				if(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)] == NULL){
					lock_release(&file_lock);
					sys_exit(-1);
				}
				lock_release(&file_lock);
				f->eax = file_read(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)],
						 (void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
				break;
			}
			else{
				lock_release(&file_lock);
				f->eax =  -1;
				break;
			}

		case SYS_WRITE:
			if(is_user_vaddr((const void*)*(uint32_t*)(f->esp+8)) == 0) sys_exit(-1);
			lock_acquire(&file_lock);
			if((int)*(uint32_t*)(f->esp+4) == 1){
				putbuf((char*)(const void*)*(uint32_t*)(f->esp+8), (size_t)(unsigned)*(uint32_t*)(f->esp+12));
				lock_release(&file_lock);
				f->eax = (unsigned)*(uint32_t*)(f->esp+12);
				break;
			}
			else if((int)*(uint32_t*)(f->esp+4) > 2){
				lock_release(&file_lock);
				f->eax = file_write(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)],
						 (const void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
				break;
			}
			else {
				lock_release(&file_lock);
				f->eax = -1;
				break;
			}

		case SYS_FIBO:
			f->eax=sys_fibonacci((int)*(uint64_t*)(f->esp+4));
			break;

		case SYS_MAX4:
			f->eax=sys_max_of_four_int((int)*(uint64_t*)(f->esp+4),
				   (int)*(uint64_t*)(f->esp+8),
				   (int)*(uint64_t*)(f->esp+12),
				   (int)*(uint64_t*)(f->esp+16));
			break;
			
        case SYS_CREATE:
			if(!is_user_vaddr(f->esp+4)|| !is_user_vaddr(f->esp+8)) sys_exit(-1);
			if((const char*)*(uint32_t*)(f->esp+4) == NULL) sys_exit(-1);
			else
			    f->eax = filesys_create((const char*)*(uint32_t*)(f->esp+4),(unsigned)*(uint32_t*)(f->esp+8));
			break;

		case SYS_REMOVE:
			if(is_user_vaddr(f->esp+4)) 
			    f->eax = filesys_remove((const char*)*(uint32_t*)(f->esp+4));
			else sys_exit(-1);
			break;

		case SYS_OPEN:
			if(!is_user_vaddr(f->esp+4)) sys_exit(-1);
			if((const char*)*(uint32_t*)(f->esp+4) == NULL) sys_exit(-1);
			lock_acquire(&file_lock);
			fp = filesys_open((const char*)*(uint32_t*)(f->esp+4));
			if(fp == NULL){
				lock_release(&file_lock);
				f->eax = -1;
				break;
			}
			
			else{
				
                int i = 3;
				while(i<128){
					if(thread_current()->fd[i] == NULL){
						thread_current()->fd[i] = fp;
						lock_release(&file_lock);
						f->eax =  i;
						flag = 1;
						break;
					}
					i++;
				} if(flag) break;

			} 
			lock_release(&file_lock);
			f->eax = -1;
			break;

		case SYS_CLOSE:
			if(is_user_vaddr(f->esp+4)||thread_current()->fd[(int)*(uint32_t*)(f->esp+4)] != NULL) {
			file_close(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)]);
			thread_current()->fd[(int)*(uint32_t*)(f->esp+4)] = NULL;
			break;}
			else sys_exit(-1);

		case SYS_FILESIZE:
			if(is_user_vaddr(f->esp+4)||thread_current()->fd[(int)*(uint32_t*)(f->esp+4)]!=NULL)
			{
   			    f->eax =  file_length(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)]);
			    break;
			} 
			
			else sys_exit(-1);
		case SYS_SEEK:
			if(!is_user_vaddr(f->esp+4)||!is_user_vaddr(f->esp+8)||thread_current()->fd[(int)*(uint32_t*)(f->esp+4)] == NULL) sys_exit(-1);
			
			
			else{
			    file_seek(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)], (unsigned)*(uint32_t*)(f->esp+8));
			    break;}

		case SYS_TELL:
			if(!is_user_vaddr(f->esp+4)||thread_current()->fd[(int)*(uint32_t*)(f->esp+4)] == NULL) sys_exit(-1);
			else {
			    f->eax = file_tell(thread_current()->fd[(int)*(uint32_t*)(f->esp+4)]);
			    break;}
   } 
	return;
}


int sys_fibonacci (int n){
	int array[3]={0,1,0};
    array[0]=0;
    array[1]=1;
    for(int i=2;i<n+1;i++){
        array[i]=array[i-1]+array[i-2];
    }
    return array[n];
}

int sys_max_of_four_int(int a, int b, int c, int d){
	int max = a;
	if(max<b) max = b;
	if(max<c) max = c;
	if(max<d) max = d;
	return max;
}

void sys_exit (int status){
	printf("%s: exit(%d)\n", thread_name(), status);
	thread_current()->exit_status = status;

	
	for(int idx = 3; idx<128; idx++){
		if(thread_current()->fd[idx] != NULL) {
			sys_close(idx);
		}
	}

	struct list_elem *e = list_begin(&(thread_current())->list_child);
	struct thread *t = list_entry(e, struct thread, child_element);

	while(e != list_end(&(thread_current()->list_child))){
		process_wait(t->tid);
		e = list_next(e);
		t = list_entry(e, struct thread, child_element);
	}

	file_close(thread_current()->current_file);
	thread_exit();
}
void sys_close(int fd){
	
	file_close(thread_current()->fd[fd]);
	thread_current()->fd[fd] = NULL;
}