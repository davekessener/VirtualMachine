/* File: vmem.h
 * Global Definitions for BSP3 sample solution
 * Model of virtual memory management
 */

#ifndef VMEM_H
#define VMEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define FALSE 0
#define TRUE (!FALSE)


#define SHMKEY          42413

typedef unsigned int Bmword;    /* Frame bitmap */

//#define VMEM_PAGESIZE       8   /* Items per page */
#ifndef VMEM_PAGESIZE
#	define VMEM_PAGESIZE 8
#endif

/* Sizes */
#define VMEM_VIRTMEMSIZE 1024   /* Process address space / items */
#define VMEM_PHYSMEMSIZE  128   /* Physical memory / items */
#define VMEM_NPAGES     (VMEM_VIRTMEMSIZE / VMEM_PAGESIZE)      /* Total number of pages */
#define VMEM_NFRAMES (VMEM_PHYSMEMSIZE / VMEM_PAGESIZE) /* Number of available frames */
#define VMEM_LASTBMMASK (~0U << (VMEM_NFRAMES % (sizeof(Bmword) * 8)))
#define VMEM_BITS_PER_BMWORD     (sizeof(Bmword) * 8)
#define VMEM_BMSIZE     ((VMEM_NFRAMES - 1) / VMEM_BITS_PER_BMWORD + 1)

/* Page Table */
#define PTF_PRESENT     1
#define PTF_DIRTY       2       /* store: need to write */
#define PTF_REF         4       /* reference flag */


struct pt_entry {
   int flags;                  /* see defines above */
   int frame;                  /* Frame idx */
   int count;                  /* Global counter as quasi-timestamp for LRU */
};

struct vmem_adm_struct {
    int size;
    pid_t mmanage_pid;
    int shm_id;
    sem_t sema;                 /* Coordinate acces to shm */
    int req_pageno;             /* Number of requested page */
    int next_alloc_idx;         /* Next frame to allocate (FIFO, CLOCK) */
    int pf_count;               /* Page fault counter */
    int g_count;               /* Global acces counter as quasi-timestamp */
    Bmword bitmap[VMEM_BMSIZE]; /* bitmap for free frames -- 0 = free */
};

struct pt_struct {
    struct pt_entry entries[VMEM_NPAGES];
    int framepage[VMEM_NFRAMES];        /* pages on frame */
};

/* This is to be located in shared memory */
struct vmem_struct {
    struct vmem_adm_struct adm;
    struct pt_struct pt;
    int data[VMEM_NFRAMES * VMEM_PAGESIZE];
};

#define SHMSIZE (sizeof(struct vmem_struct))

#endif /* VMEM_H */
