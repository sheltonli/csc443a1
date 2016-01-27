#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
  int uid1;
  int uid2;
} Record;

void write_lines(FILE * file_name);

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

  if (!(fp_write = fopen ("records_lines.data", "wb" )))
    exit(EXIT_FAILURE); 

  while ((read = getline(&line, &len, fp)) != -1) {
    fwrite(line, sizeof(line), 1, fp_write);
    /* Force  data to disk */
    fflush (fp_write);
  }
  if (line)
    free(line);
  fclose(fp_write);  
}