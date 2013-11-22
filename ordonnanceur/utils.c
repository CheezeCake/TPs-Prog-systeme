#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "utils.h"

Process* create_process(pid_t pid)
{
	Process *p = NULL;

	p = malloc(sizeof(Process));
	p->next = p->previous = NULL;
	p->pid = pid;

	return p;
}

void init_list(Process_list *l)
{
	l->first = l->current = NULL;
}

void destroy_list(Process_list *l)
{
	Process *p = NULL;
	Process *d = NULL;

	p = l->first;
	while(p != NULL)
	{
		d = p;
		p = p->next;
		free(d);
	}
}

void add_process(Process_list *l, pid_t pid)
{
	Process *p = NULL;
	
	p = create_process(pid);
	if(!empty(l))
	{
		p->previous = l->first->previous;
		l->first->previous = p;
	}

	p->next = l->first;
	l->first = p;
}

void delete_process(Process_list *l, pid_t pid)
{
	Process *p = NULL;

	if(empty(l))
		return;

	for(p = l->first; p != NULL; p = p->next)
	{
		if(p->pid == pid)
		{
			if(p == l->current)
				l->current = p->next;

			if(p == l->first)
			{
				l->first = p->next;
				if(p->next != NULL)
					p->next->previous = NULL;
			}
			else
			{
				p->previous->next = p->next;
				if(p->next != NULL)
					p->next->previous = p->previous;
			}

			free(p);
			return;
		}
	}
}

int empty(Process_list *l)
{
	return (l->first == NULL);
}

pid_t get_process(Process_list *l)
{
	pid_t ret = 0;

	if(!empty(l))
	{
		if(l->current == NULL)
			l->current = l->first;

		ret = l->current->pid;
		l->current = l->current->next;
	}

	return ret;
}

pid_t read_pid(const char *path)
{
	pid_t pid;
	int fd;

	fd = open(path, O_RDONLY);
	read(fd, &pid, sizeof(pid));
	close(fd);

	return pid;
}

void write_pid(const char *path, pid_t pid)
{
	int fd;

	fd = open(path, O_WRONLY | O_CREAT, S_IRWXU);
	lockf(fd, F_LOCK, sizeof(pid));
	write(fd, &pid, sizeof(pid));
	lseek(fd, 0, SEEK_SET);
	lockf(fd, F_ULOCK, sizeof(pid));
	close(fd);
}
