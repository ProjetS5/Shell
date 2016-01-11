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
  int pid, pd[2], size, tmp, tmp2, fd;
  
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
    size = 20;
    if(!(arg = malloc(size*sizeof(arg))))
       exit(EXIT_FAILURE);
    n = arg;
    pipe(pd);
    tmp = dup(STDOUT_FILENO);
    dup2(pd[1],STDOUT_FILENO);
    evaluer_expr(e->gauche);
    dup2(tmp, STDOUT_FILENO);
    if(close(pd[1]))
      close_erreur();
    if(close(tmp))
      close_erreur();
    while(read(pd[0],&c,1)>0){
      *n = c;
      n++;
      if((n-arg)>size){
	size *= 2;
	if(!(arg = realloc(arg, size*sizeof(arg))))
	  exit(EXIT_FAILURE);
      }
    }
    (e->droite)->arguments=AjouterArg((e->droite)->arguments, arg);
    evaluer_expr(e->droite);
    if(close(pd[0]))
      close_erreur();
    free(arg);
    break;
  case REDIRECTION_I :
    fd = open(e->arguments[0], O_RDONLY);
    size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    if(read(fd, &c, size)>0)
      (e->gauche)->arguments=AjouterArg((e->gauche)->arguments, &c);
    if(!(size >= 30) && close(fd))
      close_erreur();
    evaluer_expr(e->gauche);
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
    if(close(pd[1]))
      close_erreur();
    while(read(pd[0], &c, 1)>0)
      write(fd, &c, 1);
    if(close(pd[0]))
       close_erreur();
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
    if(close(pd[1]))
      close_erreur();
    while(read(pd[0], &c, 1)>0)
      write(fd, &c, 1);
    if(close(pd[0]))
      close_erreur();
    break;
  case SOUS_SHELL :
    size = 0;
    if(!(a = malloc(sizeof((e->gauche)->arguments) + 20*sizeof(char*))))
       exit(EXIT_FAILURE);
    *a = "./Shell\0";
    while((a[1+size] = (e->gauche)->arguments[size]) != NULL)
      size++;
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
  else if(strcmp(s, "history") == 0) *f = &history;
  else{
    fprintf(stderr, "cette fonctionnalité n'est pas implementé : %s \n", s);
    return EXIT_FAILURE;
  }
  return 0;
}

void
close_erreur(){
  perror("close");
  exit(EXIT_FAILURE);
}
