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
  
  int j = 0;
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
    I think what was happening since we were using i, we were checking if the number of things written
    were less than the number of records per block. This doesn't work, as seen in this example:
    block size is 1048576
    records_per_block is 131072
    Before, we said as long as i < records_per_block
    But at i = 16384, since we advance j by size of Record which is 8, j is already 131072
    So in the next iteration of i, we will seg fault as we're going over the buffer limit

    So I fixed this by removing i, and only using j
    */
    if(j < records_per_block){
      /* When the block size is > 1024 the line below segfaults. 
      All the parameters are valid, so it seems as if not 
      enough space is allocated in the array. I can not 
      figure out what the issue is...*/
      buffer[j] = rec;
      j+=sizeof(Record);
    }
    else{
      count ++;
      printf("COUNT: %d\n",count);
      fwrite(buffer, sizeof(Record), records_per_block, fp_write);
      /* Force  data to disk */
      fflush (fp_write);
      j = 0;
      buffer[j] = rec;
      j+=sizeof(Record);
    }
  }
  if (line)
    free(line);
  fclose(fp_write);
  /* Why does this cause problems????? 
  blocks(3116,0x7fff789ba300) malloc: *** error for object 0x7fe3ea001600: incorrect checksum for freed object - object was probably modified after being freed.
  *** set a breakpoint in malloc_error_break to debug
  Abort trap: 6
  */
  //free(buffer); 
}