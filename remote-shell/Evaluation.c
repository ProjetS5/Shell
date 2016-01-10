#include "Shell.h"
#include "Evaluation.h"
#include "Commandes_Internes.h"
#include "string.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int
evaluer_expr(Expression *e)
{
  function f;
  char **a, c, *arg, *n;
  int pid, pd[2], cmp, tmp, tmp2, fd;
  
  switch(e->type){  

  case VIDE :
    printf("Je pense que t'as oublié de mettre une expression là ^^' \n");
    break;
  case SIMPLE :
    if(choisir_fonction(e->arguments[0], &f))
      return EXIT_FAILURE;
    a = e->arguments;
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
  case SEQUENCE_OU :
    if(evaluer_expr(e->gauche))
      evaluer_expr(e->droite);
    break;
  case BG :
    if(!(pid =fork())){
      (evaluer_expr(e->gauche));
      exit(0);
    }
    printf("[%d]\n", pid);
    break;
  case PIPE :
    cmp = 20;
    if(!(arg = malloc(cmp*sizeof(arg))))
       exit(EXIT_FAILURE);
    n = arg;
    pipe(pd);
    tmp = dup(STDOUT_FILENO);
    dup2(pd[1],STDOUT_FILENO);
    evaluer_expr(e->gauche);
    dup2(tmp, STDOUT_FILENO);
    close(pd[1]);
    close(tmp);
    while(read(pd[0],&c,1)>0){
      *n = c;
      n++;
      if((n-arg)>cmp){
	cmp *= 2;
	if(!(arg = realloc(arg, cmp*sizeof(arg))))
	  exit(EXIT_FAILURE);
      }
    }
    (e->droite)->arguments=AjouterArg((e->droite)->arguments, arg);
    evaluer_expr(e->droite);
    close(pd[0]);
    free(arg);
    break;
  case REDIRECTION_I :
    fd = open(e->arguments[0], O_RDONLY);
    cmp = lseek(fd, 0, SEEK_END);   
    lseek(fd, 0, SEEK_SET);
    if(read(fd, &c, cmp)>0)
      (e->gauche)->arguments=AjouterArg((e->gauche)->arguments, &c);
    close(fd);
    evaluer_expr(e->gauche);
    free(arg);
    break;
  case REDIRECTION_O :
    fd = open(e->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  case REDIRECTION_A :
    if(e->type == REDIRECTION_A)
      fd = open(e->arguments[0], O_WRONLY | O_CREAT | O_APPEND, 0666);
    pipe(pd);
    tmp = dup(1);
    dup2(pd[1],1);
    evaluer_expr(e->gauche);
    dup2(tmp,1);
    close(pd[1]);
    while(read(pd[0], &c, 1)>0)
      write(fd, &c, 1);
    close(pd[0]);
    break;
  case REDIRECTION_EO :
  case REDIRECTION_E :
    fd = open(e->arguments[0], O_WRONLY | O_CREAT | O_TRUNC, 0666);
    pipe(pd);
    tmp = dup(2);
    if(e->type == REDIRECTION_EO){
      tmp2 = dup(1);
      dup2(pd[1],1);
    }
    dup2(pd[1],2);
    evaluer_expr(e->gauche);
    if(e->type == REDIRECTION_EO)
      dup2(tmp2,1);
    dup2(tmp,2);
    close(pd[1]);
    while(read(pd[0], &c, 1)>0)
      write(fd, &c, 1);
    close(pd[0]);
    break;
  case SOUS_SHELL :
    cmp = 0;
    if(!(a = malloc(sizeof((e->gauche)->arguments) + 20*sizeof(char*))))
       exit(EXIT_FAILURE);
    *a = "./Shell\0";
    while((a[1+cmp] = (e->gauche)->arguments[cmp]) != NULL)
      cmp++;
    if(!fork())
      execv(*a, a);
    free(a);
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
    fprintf(stderr, "cette fonctionnalité n'est pas implementé : %s \n", s);
    return EXIT_FAILURE;
  }
  return 0;
}
