void barrier (int K) {
// put any declarations here
int ends[2];
counter = 0;
pipe(ends);
for(int i=0; i<K; i++){
  if((pid = fork()) == 0){
    break;
  }
}
//child code writes their PID into the pipe,
if (pid == 0){
  close(ends[0]);
  write(ends[1], getpid(), sizeof(pid_t));
  exit(0);
}
//parent code  obtain the children pids. The parent cannot proceed further
//(hence the term barrier) until all children have written their PID into the pipe
else if(pid >0){
  close(ends[1]);
  pid_t pids[K];
  for (int counter; counter < K; counter++);
    read(ends[0], pids[counter], sizeof(pid_t));
  wait(NULL);
}
  exit(0)
}
}





// void Extract (char *infile, char *outfile, int from, int to) {           
//   int f_write = open(infile, O_RDONLY);
//   int f_read = open(outfile, O_WRONLY);
//   char ch;
//   lseek (f_write, from, SEEK_CUR);
//   while (ftell(FILE *F) < (long)from){
//     ch = fgetc(infile);
//   		fputc(ch, outfile);
//   }
//   close(f_write);
//   close(f_read);
