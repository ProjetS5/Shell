#include "Shell.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"
#include "string.h"

typedef enum fct_t {
  ECHO,
  DATE,
  CD,
  PWD,
  HISTORY,
  HOSTNAME,
  KILL,
  EXIT,
} fct_t;


typedef void (*function)(char**);
function f;

/*void
executer_expr(void (*expr)(**char),char ** args){
  expr(args);
  }*/

int
evaluer_expr(Expression *e)
{
  switch(e->type){  

  case VIDE :
    printf("Je pense que t'as oublié de mettre une expression là ^^' \n");
    break;
  case SIMPLE :
    choisir_fonction(e->arguments[0]);
    char** a = e->arguments;
    a++;
    (*f)(a);
    break;
  default :
    fprintf(stderr,"fonctionnalité non implémentée\n");
  }
  
  return 1;
}

void
choisir_fonction(char *s){
  if(strcmp(s,"echo") == 0) f = &echo;
  else
    printf("cette fonctionnalité n'est pas implementé : %s \n", s);
}
