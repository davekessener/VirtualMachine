#ifndef DAV_GSP_1_OPMAP_H
#define DAV_GSP_1_OPMAP_H

#include "common.h"
#include "stack.h"

/* der type op_fn spezifiziert einen pointer zu
   einer function void(stack_t *).
*/
typedef void (*op_fn)(stack_t *);

/* dies dient als lookup table bei dem jede
   ein-buchstabe funktion die gefragt ist
     [ie '+', '-', 'p', 'd', 'f', ...]
   durch ein key-value pair symbol-functionspointer
   ausgedrueckt wird.
*/
typedef struct
{
	char *ids; /* keys (symbole) */
	op_fn *ops; /* values (functionspointer) */
	uint i, c; /* i == bufferende, c == capacitaet */
} opmap_t;

void opmap_init(opmap_t *);
void opmap_set(opmap_t *, char, op_fn);
op_fn opmap_get(const opmap_t *, char);
void opmap_delete(opmap_t *);

#endif

