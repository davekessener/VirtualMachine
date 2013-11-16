#ifndef __NUMEVAL_H
#define __NUMEVAL_H

#include "include.h"
#include "evaluate.h"
#include "logger.h"
#include "parameter.h"
#include "params.h"
#include "dstack.h"

#define SM_LIB
#include "stackmachine.h"
#undef SM_LIB

int executeNumberEvaluator(const char *);

#endif

