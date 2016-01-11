/*#include <libssh/libssh.h>
#include <stdlib.h>

char** name;

void remote_add(char** s){
  name=malloc(4*sizeof(*name));
  for(int j = 0; j < 4 ; j++){
    name[i] = malloc(16 * sizeof(char));
  }
  int pid1 = fork();
  int pid2 = fork();
  
  int tube1[2];
  int tube2[2];
  pipe(tube1);
  pipe(tube2);
  int status;
  
  if(!pid1){     //FILS ENVOIS
    
    dup2(tube2[1], STDIN_FILENO);
    execlp("ssh", "ssh", s[0], bash);
    
    
  }
  if(!pid2){     // FILS RECEPTION
    dup2(tube2[0], STDOUT_FILENO);
    
  }
}

void remote_remove(){
  int i = 0;
  for(i; i<4 ; i++){
    free(name[i]);
  }
  free name;
}*/



