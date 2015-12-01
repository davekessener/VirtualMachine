#ifndef DAV_TIBS_3_MMANAGE_H
#define DAV_TIBS_3_MMANAGE_H

#define VMEM_ALGO_FIFO 0
#define VMEM_ALGO_CLOCK 1
#define VMEM_ALGO_AGING 2

#ifndef VMEM_ALGO
#	define VMEM_ALGO VMEM_ALGO_FIFO
#endif

#define MXT_VIDX -1

#define MXT_AGINGW 8
#define MXT_AGINGINT 20

#endif

