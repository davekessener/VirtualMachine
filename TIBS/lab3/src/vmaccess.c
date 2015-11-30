#include "vmaccess.h"
#include "debug.h"
#include "vmem.h"

#define MXT_FRAMEIDX(i) (i/VMEM_PAGESIZE)
#define MXT_FRAMEOFF(i) (i%VMEM_PAGESIZE)

struct vmem_struct *vmem;

void vm_init(void)
{
	static int is_initialized = FALSE;

	if(!is_initialized)
	{
		int shm_id = shmget(SHMKEY, SHMSIZE, 0644);
	
		if(shm_id == -1)
			fatal("Error accessing shared memory!");
	
		vmem = shmat(shm_id, NULL, 0);
	
		if(vmem == (void *) -1)
			fatal("Error attaching shared memory!");

		is_initialized = TRUE;
	}
}

void load_page(struct vmem_struct *vmem, int idx)
{
	struct pt_entry *pe = &vmem->pt.entries[idx];

	if(!(pe->flags & PTF_PRESENT))
	{
		vmem->adm.req_pageno = idx;
		kill(vmem->adm.mmanage_pid, SIGUSR1);
		sem_wait(&vmem->adm.sema);
	}

	++pe->count;
	++vmem->adm.g_count;
}

int vmem_read(int address)
{
	int idx = MXT_FRAMEIDX(address);
	int off = MXT_FRAMEOFF(address);

	vm_init();

	load_page(vmem, idx);

	return vmem->data[vmem->pt.entries[idx].frame * VMEM_PAGESIZE + off];
}

void vmem_write(int address, int data)
{
	int idx = MXT_FRAMEIDX(address);
	int off = MXT_FRAMEOFF(address);

	vm_init();

	struct pt_entry *pe = &vmem->pt.entries[idx];

	load_page(vmem, idx);

	vmem->data[pe->frame * VMEM_PAGESIZE + off] = data;
	pe->flags |= PTF_DIRTY;
}

