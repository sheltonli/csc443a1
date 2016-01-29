#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct record {
  int uid1;
  int uid2;
} Record;

void write_lines(FILE * file_name);

const int MB = 1024 * 1024;

int main(int argc, char* argv[]){
  FILE* fp;

  if (argc != 2){
    printf("Usage: <input filename>\n");
    return 1;
  }  
  if(!(fp = fopen(argv[1], "r"))){
    exit(EXIT_FAILURE);
  }
  write_lines(fp);
  fclose(fp);
  return 0;
}

void write_lines(FILE * fp){
  char * line = NULL;
  size_t len = 0;
  size_t read;
  FILE* fp_write;

  if (!(fp_write = fopen ("records_lines.txt", "wb" )))
    exit(EXIT_FAILURE);

  clock_t begin, end;
  double time_spent;
  long total_size = 0;

  begin = clock();
  /* code to be timed */
  while ((read = getline(&line, &len, fp)) != -1) {
    fwrite(line, strlen(line), 1, fp_write);
    total_size += strlen(line);
    /* Force  data to disk */
    fflush (fp_write);
  }
  end = clock();

  if (line)
    free(line);
  fclose(fp_write); 

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  /* result in MB per second */
  printf ("Data rate: %.3f MBPS\n", (total_size/time_spent)/MB);

}