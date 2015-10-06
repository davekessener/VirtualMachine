#ifndef DAV_BS_LAB2_QUEUE
#define DAV_BS_LAB2_QUEUE

#define MXT_QUEUE_ERR_OK 0
#define MXT_QUEUE_ERR_FULL 1
#define MXT_QUEUE_ERR_EMPTY 2
#define MXT_QUEUE_ERR_SYNC 3

#define MXT_QUEUE_ST 0
#define MXT_QUEUE_SYNC 1

typedef struct
{
	size_t capacity;
	size_t begin, end;
	size_t size;
	void *data;
	int type;
	void *control;
} QUEUE;

int QUEUE_new(QUEUE *, size_t, size_t, int);
int QUEUE_push(QUEUE *, const void *);
int QUEUE_poll(QUEUE *, void *);
size_t QUEUE_size(QUEUE *);
void QUEUE_delete(QUEUE *);

#endif

