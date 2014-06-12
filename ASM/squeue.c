#include "squeue.h"

void SQ_init(S_QUE *this)
{
	this->data = NULL;
	this->c = this->i = 0;
}

void SQ_push(S_QUE *this, const char *v)
{
	this->data = realloc(this->data, ++this->c * sizeof(char *));
	this->data[this->c - 1] = strdup(v);
}

const char *SQ_poll(S_QUE *this)
{
	if(this->i >= this->c) return NULL;

	return this->data[this->i++];
}

char *SQ_join(S_QUE *this)
{
	char buf[BUF_SIZE];
	int i;

	buf[0] = '\0';

	for(i = this->i ; i < this->c ; i++)
	{
		strncat(buf, this->data[i], BUF_SIZE);
		if(i < this->c - 1) strcat(buf, " ");
	}

	return strdup(buf);
}

void SQ_dispose(S_QUE *this)
{
	while(this->c-- > 0)
	{
		free(this->data[this->c]);
	}

	free(this->data);

	SQ_init(this);
}


