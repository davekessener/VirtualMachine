#ifndef DAV_GSP_1_STACK_H
#define DAV_GSP_1_STACK_H

#include "common.h"

/* pushe eine zahl auf den stack */
int stack_push(MXT_STYPE);
/* gepopte zahl wird zur gepassten adresse geschrieben */
int stack_pop(MXT_STYPE *);
int stack_empty(void);

#endif

