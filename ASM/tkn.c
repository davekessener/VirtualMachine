#include "tkn.h"

void TKN_init(TKN *this)
{
	this->src = NULL;
	this->cp  = NULL;
	this->pp  = NULL;
}

void TKN_load(TKN *this, const char *init)
{
	int i, l;

	// Copy input, set all pointers to the beginning of the input
	this->src = strdup(init);
	this->cp  = this->src;
	this->pp  = this->src;

	// Convert input to lowercase
	for(i = 0, l = strlen(this->src) ; i < l ; i++)
	{
		if(this->src[i] >= 'A' && this->src[i] <= 'Z')
		{
			this->src[i] -= 'A' - 'a';
		}
	}

	// Remove all whitespace from the input
	deleteWhitespace(this->src);
}

T	 TKN_get(TKN *this, TABLE *table)
{
	T t;
	char c, buf[T_BUF_SIZE], *tmp;
	int p = 0, r;

	// The has to be input loaded, and a symbol table provided
	assert(this->src!=NULL);
	assert(this->cp!=NULL);
	assert(table!=NULL);

	// Override previously successfully processed token with current position
	this->pp = this->cp;
	
	// Initialize read token with an invalid return type. (aka 'nothing has been read')
	t.type = TAG_NONE;

	// Read first character from the input
	c = *this->cp;
	
	// If we are at the end of the input, return empty token
	if(c == '\0') return t;

	// If we are at the beginning of a number ...
	if(c >= '0' && c <= '9')
	{
		// ... assign T the 'value'-type and store the evaluated number
		t.type = TAG_VALUE;
		t.data.val = evalNumber(&this->cp);
	}
	else
	{
		// ... else we are reading a token.
		// While we are not at the end of the input
		while(c)
		{
			buf[p++] = c; // Buffer character
			buf[p] = '\0'; // Terminate buffer
	
			// See if the buffer contains a recognized token
			// (or parts of it)
			r = TABLE_find(table, buf);

			// If there is no chance that the buffer can contain
			// a valid token
			if(r == 0)
			{
				// If we have already read a token successfully,
				// reset the current position pointer to the first
				// character after the read token
				if(t.type == TAG_TAG)
				{
					this->cp = tmp;
				}
	
				break; // exit loop
			}
			// If the buffer contains the beginning of a valid token ...
			else if(r == -1)
			{
				// ... do nothing. 
			}
			// If the buffer contents equal a valid token ...
			else if(r > 0)
			{
				// ... store the tokens integer ID inside the T-structure
				t.type = TAG_TAG;
				t.data.tag = r;
				// save current position (pointing to the first character
				// following the valid token), so we can restore its position later
				tmp = this->cp + 1;

				// Continue reading from the input, in case there is a
				// valid token that matches the input and is longer than
				// the currently matched token.
			}
	
			// Read next character, increment current position
			c = *++this->cp;
		}
	}

	// Return the token
	return t;
}

// Push back the last read token (set current position to the
// beginning of the previous token)
void TKN_unget(TKN *this)
{
	this->cp = this->pp;
}

void TKN_dispose(TKN *this)
{
	free(this->src);
}

// # ==========================================================================

void TABLE_init(TABLE *this)
{
	this->ids  = NULL;
	this->vals = NULL;
	this->c    = 0;
}

// Add a token identified by string 'id' and associate it with 'v'
void TABLE_add(TABLE *this, const char *id, int v)
{
	this->ids = realloc(this->ids, ++this->c * sizeof(char *));
	this->vals = realloc(this->vals, this->c * sizeof(int));

	this->ids[this->c - 1]  = strdup(id);
	this->vals[this->c - 1] = v;
}

// Find the token 'id'; and if the table contains said token
// return its associated value. Otherwise return -1 if it equals
// the beginning of a valid token and 0 if it cannot  any
// token 
int  TABLE_find(TABLE *this, const char *id)
{
	int r = 0;

	int i;
	for(i = 0 ; i < this->c ; i++)
	{
		if(strcmp(this->ids[i], id) == 0)
		{
			r = this->vals[i];
			break;
		}
		else if(memcmp(this->ids[i], id, strlen(id)) == 0)
		{
			r = -1;
		}
	}

	return r;
}

void TABLE_dispose(TABLE *this)
{
	int i;
	for(i = 0 ; i < this->c ; i++)
	{
		free(this->ids[i]);
	}

	free(this->ids);
	free(this->vals);

	TABLE_init(this);
}

// # ==========================================================================


void deleteWhitespace(char *src)
{
	int i, p = 0, l = strlen(src) + 1;
	char *tmp = malloc(strlen(src) + 1);

	for(i = 0 ; i < l ; i++)
	{
		if(src[i] != ' ' && src[i] != '\t' && src[i] != '\n')
		{
			tmp[p++] = src[i];
		}
	}

	strncpy(src, tmp, l);
	free(tmp);
}

