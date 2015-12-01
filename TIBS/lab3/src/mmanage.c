#include <errno.h>
#include "vmem.h"
#include "debug.h"
#include "mmanage.h"

#define MXT_PFNAME "./pagefile.bin"
#define MXT_LOGFNAME "./logfile.txt"
#define MXT_PFF "w+b"
#define MXT_LOGF "w"


struct logevent {
    int req_pageno;
    int replaced_page;
    int alloc_frame;
    int pf_count;
    int g_count;
};

typedef struct
{
	struct vmem_struct *vmem;
	FILE *pagefile, *logfile;
	int running;
} mmanager_t;

typedef void (*sighandler_t)(int);

// # ---------------------------------------------------------------------------

mmanager_t mmu;

// # ---------------------------------------------------------------------------

void initialize(mmanager_t *);
void run(mmanager_t *);
void cleanup(mmanager_t *);

FILE *init_file(const char *, const char *);
struct vmem_struct *init_vmem(void);
void init_signals(void);

void close_file(FILE *);
void close_vmem(struct vmem_struct *);

void register_sig(sighandler_t, int);
void sighandler(int);
void logger(FILE *, struct logevent);

void handle_pf(mmanager_t *);
void handle_dump(mmanager_t *);
void handle_int(mmanager_t *);

// # ---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	initialize(&mmu);

	run(&mmu);

	cleanup(&mmu);

	printf("\nGoodbye.\n");

	return EXIT_SUCCESS;
}

// # ---------------------------------------------------------------------------

void initialize(mmanager_t *mmanager)
{
	mmanager->pagefile = init_file(MXT_PFNAME, MXT_PFF);
	mmanager->logfile = init_file(MXT_LOGFNAME, MXT_LOGF);
	mmanager->vmem = init_vmem();
	mmanager->running = TRUE;
	
	init_signals();
}

void run(mmanager_t *mmanager)
{
	while(mmanager->running)
	{
		pause();
	}
}

void cleanup(mmanager_t *mmanager)
{
	close_file(mmanager->pagefile);
	close_file(mmanager->logfile);
	close_vmem(mmanager->vmem);
}

// # ---------------------------------------------------------------------------

FILE *init_file(const char *fn, const char *s)
{
	FILE *pf = fopen(fn, s);

	if(!pf)
		fatal("Error opening '%s'!", fn);
	
	return pf;
}

struct vmem_struct *init_vmem(void)
{
	int shm_id = shmget(SHMKEY, SHMSIZE, 0644 | IPC_CREAT);

	if(shm_id == -1)
		fatal("Error reserving shared memory!");

	struct vmem_struct *vmem = shmat(shm_id, NULL, 0);

	if(vmem == (void *) -1)
		fatal("Error attaching shared memory!");

	memset(vmem->pt.framepage, MXT_VIDX, sizeof(vmem->pt.framepage));

	vmem->adm.mmanage_pid = getpid();
	vmem->adm.shm_id = shm_id;

	if(sem_init(&vmem->adm.sema, 1, 0) == -1)
		fatal("Error (%i) in init_vmems sem_init!", errno);

	return vmem;
}

void init_signals(void)
{
	register_sig(sighandler, SIGUSR1);
	register_sig(sighandler, SIGUSR2);
	register_sig(sighandler, SIGINT);
}

// # ---------------------------------------------------------------------------

void close_file(FILE *f)
{
	if(fclose(f))
		fatal("Error closing file.");
}

void close_vmem(struct vmem_struct *vmem)
{
	int shm_id = vmem->adm.shm_id;

	if(sem_destroy(&vmem->adm.sema) == -1)
		fatal("Error (%i) in sem_destroy!", errno);

	if(shmdt(vmem) == -1)
		fatal("Error detaching shared memory!");
	
	if(shmctl(shm_id, IPC_RMID, NULL) == -1)
		fatal("Error removing shared memory!");
}

// # ---------------------------------------------------------------------------


void read_page(mmanager_t *mmanager, int frame, int page)
{
	struct pt_struct *pt = &mmanager->vmem->pt;
	FILE *pf = mmanager->pagefile;

	fseek(pf, page * VMEM_PAGESIZE * sizeof(int), SEEK_SET);
	fread(mmanager->vmem->data + frame * VMEM_PAGESIZE, VMEM_PAGESIZE * sizeof(int), 1, pf);

	pt->entries[page].flags |= PTF_PRESENT | PTF_REF;
	pt->entries[page].frame = frame;
	pt->framepage[frame] = page;

#if VMEM_ALGO == VMEM_ALGO_AGING
	pt->entries[page].count = 1 << (MXT_AGINGW - 1);
#endif
}

void write_page(mmanager_t *mmanager, int frame, int page)
{
	if(page == MXT_VIDX)
		return;

	struct pt_struct *pt = &mmanager->vmem->pt;

	if(pt->entries[page].flags & PTF_DIRTY)
	{
		FILE *pf = mmanager->pagefile;

		fseek(pf, page * VMEM_PAGESIZE * sizeof(int), SEEK_SET);
		fwrite(mmanager->vmem->data + frame * VMEM_PAGESIZE, VMEM_PAGESIZE * sizeof(int), 1, pf);
	}

	pt->entries[page].flags &= ~(PTF_PRESENT | PTF_DIRTY);
	pt->entries[page].frame = MXT_VIDX;
}

void call_logger(FILE *lf, int req, int rep, int al, int pf, int g)
{
	struct logevent le = { req, rep, al, pf, g };
	logger(lf, le);
}

// # ---------------------------------------------------------------------------

int get_next_frame(struct vmem_struct *vmem)
#define INCTO(v,l) (((v)+1)%(l))
{
	int r = vmem->adm.next_alloc_idx;

#if VMEM_ALGO == VMEM_ALGO_FIFO
	vmem->adm.next_alloc_idx = INCTO(r, VMEM_NFRAMES);
#elif VMEM_ALGO == VMEM_ALGO_CLOCK
	int p;
	while((p = vmem->pt.framepage[r]) != MXT_VIDX && (vmem->pt.entries[p].flags & PTF_REF))
	{
		vmem->pt.entries[p].flags &= ~PTF_REF;
		r = INCTO(r, VMEM_NFRAMES);
	}
	vmem->adm.next_alloc_idx = INCTO(r, VMEM_NFRAMES);
#elif VMEM_ALGO == VMEM_ALGO_AGING
	int i, a, p;
	for(i = 0, r = MXT_VIDX ; i < VMEM_NFRAMES ; ++i)
	{
		if((p = vmem->pt.framepage[i]) == MXT_VIDX)
			return i;
		else if(r == MXT_VIDX || vmem->pt.entries[p].count <= a)
		{
			r = i;
			a = vmem->pt.entries[p].count;
		}
	}
#else
	fatal("Error; invalid page replacemenet algorithm defined.");
#endif

	return r;
}
#undef INCTO

// # ---------------------------------------------------------------------------

void handle_pf(mmanager_t *mmanager)
{
	struct vmem_struct *vmem = mmanager->vmem;
	struct pt_struct *pt = &vmem->pt;
	struct vmem_adm_struct *adm = &vmem->adm;

	if(!(pt->entries[adm->req_pageno].flags & PTF_PRESENT))
	{
		int r = get_next_frame(vmem);

		++adm->pf_count;

		call_logger(mmanager->logfile, adm->req_pageno, pt->framepage[r], r, adm->pf_count, adm->g_count);

		write_page(mmanager, r, pt->framepage[r]);
		read_page(mmanager, r, adm->req_pageno);
	}
	else debug("Called to load page %i which is already present!", adm->req_pageno);

	sem_post(&vmem->adm.sema);
}

void handle_dump(mmanager_t *mmanager)
{
	struct vmem_struct *vmem = mmanager->vmem;
	int i, j;

#define MXT_PRINT(v,t) printf(#v " = " t "\n", v)
	// vmem_adm_struct
    MXT_PRINT(vmem->adm.size, "0x%08x");
    MXT_PRINT(vmem->adm.mmanage_pid, "0x%016x");
    MXT_PRINT(vmem->adm.shm_id, "0x%08x");
    MXT_PRINT(vmem->adm.req_pageno, "%i");
    MXT_PRINT(vmem->adm.next_alloc_idx, "%i");
    MXT_PRINT(vmem->adm.pf_count, "%i");
    MXT_PRINT(vmem->adm.g_count, "%i");
	for(i = 0 ; i < VMEM_BMSIZE ; ++i)
    	MXT_PRINT(vmem->adm.bitmap[i], "%08X");

	// pt_struct
	printf("\nPage Table:\nFlags Frame    Count\n------------------------------\n");
	for(i = 0 ; i < VMEM_NPAGES ; ++i)
	{
		struct pt_entry *e = &vmem->pt.entries[i];
		printf("%x     %08x %i\n", e->flags, e->frame, e->count);
	}

	// data
	printf("\nData:\n");
	for(i = 0 ; i < VMEM_NFRAMES ; ++i)
		for(j = 0 ; j < VMEM_PAGESIZE ; ++j)
			printf("%08X%c", vmem->data[i * VMEM_PAGESIZE + j], j + 1 == VMEM_PAGESIZE ? '\n' : ' ');
	
#undef MXT_PRINT
}

void handle_int(mmanager_t *mmanager)
{
	mmanager->running = FALSE;
}

// # ---------------------------------------------------------------------------

void register_sig(sighandler_t h, int s)
{
	struct sigaction sa;

	sa.sa_handler = h;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if(sigaction(s, &sa, NULL) == -1)
		fatal("Error reinstalling sighandler for %i!", s);
}

void sighandler(int s)
{
	mmanager_t *mmanager = &mmu;

	if(mmanager->running)
	{
		switch(s)
		{
			case SIGUSR1:
				handle_pf(mmanager);
				break;
			case SIGUSR2:
				handle_dump(mmanager);
				break;
			case SIGINT:
				handle_int(mmanager);
				break;
		}
	}

	register_sig(sighandler, s);
}

void logger(FILE *logfile, struct logevent le)
{
    fprintf(logfile, "Page fault %10d, Global count %10d:\n"
            "Removed: %10d, Allocated: %10d, Frame: %10d\n",
            le.pf_count, le.g_count,
            le.replaced_page, le.req_pageno, le.alloc_frame);
    fflush(logfile);
}

