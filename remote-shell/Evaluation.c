#include "Shell.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"
#include "string.h"

int
evaluer_expr(Expression *e)
{ 
  switch(e->type){  

  case VIDE :
    printf("Je pense que t'as oublié de mettre une expression là ^^' \n");
    break;
  case SIMPLE :
    ;
    function f;
    if(choisir_fonction(e->arguments[0], &f))
      return EXIT_FAILURE;
    char** a = e->arguments;
    a++;
    (*f)(a);
    break;
  case SEQUENCE :
    evaluer_expr(e->gauche);
    evaluer_expr(e->droite);
    break;
  case SEQUENCE_ET :
    if(evaluer_expr(e->gauche))
      return EXIT_FAILURE;
    evaluer_expr(e->droite);
    break;
  default :
    fprintf(stderr,"fonctionnalité non implémentée\n");
    return EXIT_FAILURE; 
  }
  
  return 0;
}

int
choisir_fonction(char *s, function *f){
  if(strcmp(s,"echo") == 0) *f = &echo;
  else if(strcmp(s,"date") == 0) *f = &date;
  else if(strcmp(s,"hostname") == 0) *f = &hostname;
  else if(strcmp(s,"cd") == 0) *f = &cd;
  else if(strcmp(s,"pwd") == 0) *f = &pwd;
  else if(strcmp(s,"kill") == 0) *f = &kill2;
  else if(strcmp(s,"exit") == 0) *f = &exit2;
  else if(strcmp(s,"ls") == 0) *f = &ls;
  else{
    printf("cette fonctionnalité n'est pas implementé : %s \n", s);
    return EXIT_FAILURE;
  }
  return 0;
}
