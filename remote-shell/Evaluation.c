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

void choisir_fonction(char *s);

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
  else if(strcmp(s,"date") == 0) f = &date;
  else if(strcmp(s,"hostname") == 0) f = &hostname;
  else if(strcmp(s,"cd") == 0) f = &cd;
  else if(strcmp(s,"pwd") == 0) f = &pwd;
  else if(strcmp(s,"kill") == 0) f = &kill2;
  else if(strcmp(s,"exit") == 0) f = &exit2;
  else
    printf("cette fonctionnalité n'est pas implementé : %s \n", s);
}
