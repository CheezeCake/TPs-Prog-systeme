#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>

typedef struct process Process;
struct process
{
	pid_t pid;
	Process *next;
	Process *previous;
};

typedef struct process_list
{
	Process *first;
	Process *current;
} Process_list;

Process* create_process(pid_t);
void init_list(Process_list*);
void destroy_list(Process_list*);
void add_process(Process_list*, pid_t);
void delete_process(Process_list*, pid_t);
int empty(Process_list*);
pid_t get_process(Process_list*);

pid_t read_pid(const char*);
void write_pid(const char*, pid_t);

#endif
