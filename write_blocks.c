#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct record {
  int uid1;
  int uid2;
} Record;

Record convertString(char * line);
void write_blocks(FILE * fp, int block_size);

const int MB = 1024 * 1024;

int main(int argc, char* argv[]){
  if (argc != 3){
    printf("Usage: <input filename> <block size>\n");
    return 1;
  }

  FILE* fp;
  int block_size = atoi(argv[2]);

  if(block_size % sizeof(Record)){
    return 1;
  }
  if(!(fp = fopen(argv[1], "r"))){
    exit(EXIT_FAILURE);
  }

  write_blocks(fp, block_size);
  fclose(fp);
  return 0;
}

Record convertString(char * line){
  //printf("line: %s", line);
  char * pch;
  pch = strtok (line," ,");
  Record rec;
  rec.uid1 = atoi(pch);
  rec.uid2 = atoi(strtok(NULL, ",")); 
  return rec;
}

void write_blocks(FILE * fp, int block_size){
  char * line = NULL;
  size_t len = 0;
  size_t read;
  FILE* fp_write;

  int records_per_block = block_size / sizeof(Record);
  /* Allocate buffer for 1 block */
  Record * buffer = (Record *) calloc (records_per_block, sizeof(Record));

  if (!(fp_write = fopen ("records.data", "wb" )))
    exit(EXIT_FAILURE); 

  clock_t begin, end;
  double time_spent;
  long total_records = 0;

  int i = 0;
  int j = 0;

  begin = clock();
  /* code to be timed */
  while ((read = getline(&line, &len, fp)) != -1) {
    Record rec = convertString(line);
    total_records++;
    if(i < records_per_block){
      buffer[j] = rec;
      j+=sizeof(Record);
      i++;
    }
    else{
      fwrite(buffer, sizeof(Record), records_per_block, fp_write);
      /* Force  data to disk */
      fflush (fp_write);
      i = 0;
      j = 0;
    }
  }
  end = clock();

  if (line)
    free(line);
  fclose(fp_write);
  free(buffer); 

  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  /* result in MB per second */
  printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/time_spent)/MB);
}

