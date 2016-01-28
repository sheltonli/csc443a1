#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
  int uid1;
  int uid2;
} Record;

Record convertString(char * line);
void write_blocks(FILE * fp, int block_size);

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

  if (!(fp_write = fopen ("records.dat", "wb" )))
    exit(EXIT_FAILURE);  

  int i = 0;
  int j = 0;
  while ((read = getline(&line, &len, fp)) > 0) {
    Record rec = convertString(line);
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
      buffer[j] = rec;
      j+=sizeof(Record);
      i++;
    }
  }
  if (line)
    free(line);
  fclose(fp_write);
  free(buffer); 
}

