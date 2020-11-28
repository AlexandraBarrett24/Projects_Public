#include <unistd.h>
#include <stdio.h>
#define K 10

void main(){
  m_t msg;
  int id;
  //fork processes and create pipes
  int ends[K][2];
  for(int i=0; i<K; i++){
    pipe(ends[i]);
    if((pid = fork()) == 0){
      break;
    }
  }
  //child code
  if (pid == 0){
    close(ends[pid][1]);
    read(ends[pid][0], msg, 20);
    do_something_with_msg(msg);
    exit(0);
  }
  //parent code
  else if(pid >0){
    while(get_next_msg_to_send(&msg, &id) != 0){
      close(ends[*id][0]);
      write(ends[*id][1], msg, 20);
      wait(NULL);
    }
    exit(0)
  }
}
