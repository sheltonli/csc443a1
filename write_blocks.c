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
int count = 0;
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
  while ((read = getline(&line, &len, fp)) != -1) {
    /*
    Try printing lines for block size = 1032. The last line 
    printed before segfault is "Q". Extreemly odd. Maybe you
    can figure out what's going on..
    */
    printf("line: %s\n", line);

    //printf("Number of caracter read: %d\n", read);
    Record rec = convertString(line);

    /*
    so basically skipping by 8 screws everything up, thats why we had garbage values in the buffer
    dont need to increment by 8
    i removed j cuz its not needed
    */

    if(i < records_per_block){
      buffer[i] = rec;
      i++;
    }
    else{
      count ++;
      printf("COUNT: %d\n",count);
      fwrite(buffer, sizeof(Record), records_per_block, fp_write);
      /* Force  data to disk */
      fflush (fp_write);
      i = 0;
      buffer[i] = rec;
      i++;
    }
  }

  /*
  problem of say loop ends when buffer is not full, those remaining records aren't written
  you could put a write at the end of the loop, but it'll write more than we want since we 
  don't ever clear the buffer, we just make it point to other lines.
  preferably want to clear buffer everytime it writes so we can do one final write outside the loop.
  */

  if (line)
    free(line);
  fclose(fp_write);
  free(buffer); 
}