#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLCOK_SIZE 1 /*1MB*/

typedef struct record {
  int uid1;
  int uid2;
} Record;

Record convertString(char * line);
void parser(FILE* fp);


void write_blocks(char * file_name, int block_size){
  char * line = NULL;
  size_t len = 0;
  size_t read;
  FILE* fp_write;
  FILE* fp;

  int records_per_block = block_size / sizeof(Record);
  /* Allocate buffer for 1 block */
  Record * buffer = (Record *) calloc (records_per_block, sizeof(Record));

  if (!(fp_write = fopen ("records.data", "wb" )))
    exit(EXIT_FAILURE); 

  if(!(fp = fopen(file_name, "r")))
    exit(EXIT_FAILURE);

  int i = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    Record rec = convertString(line);
    if(i < records_per_block){
      printf("Record #: %d \t Data: %d, %d \n", i, rec.uid1, rec.uid2);
      buffer[i] = rec;
      i++;
    }
    else{
      /* flush buffer */ 
      fwrite(buffer, sizeof(Record), total_records, fp_write);
      /* Force  data to disk */
      fflush (fp_write);
      fclose(fp_write);
      free(buffer); 
    }
  }
  if (line)
    free(line);
  fclose(fp);

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

int main(void)
{
  write_blocks("./g_plusAnonymized.csv", 1024*1024);
  return 0;
}