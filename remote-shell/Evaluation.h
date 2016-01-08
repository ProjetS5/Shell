#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Shell.h"

typedef void (*function)(char**);
extern int evaluer_expr(Expression *e);
extern int choisir_fonction(char *s, function *f);

#endif
