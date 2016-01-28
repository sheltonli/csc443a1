#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct max_avg{
  int avg;
  int max;
} Max_average;

typedef struct record {
  int uid1;
  int uid2;
} Record;

Max_average max_ave_seq_disk(FILE * fp_read, int block_size);
/* allocate buffer for 1 block */

int main(int argc, char* argv[]){
  FILE* fp;
  int block_size = atoi(argv[2]);

  if (argc != 3){
    printf("Usage: <input filename> <block size>\n");
    return 1;
  }
  if(block_size % sizeof(Record)){
    return 1;
  }
  if(!(fp = fopen(argv[1], "rb"))){
    exit(EXIT_FAILURE);
  }

  max_ave_seq_disk(fp, block_size);
  fclose(fp);

  return 0;
}

Max_average max_ave_seq_disk(FILE * fp_read, int block_size){
  Max_average ma;
  int records_per_block = block_size/sizeof(Record);
  Record * buffer = (Record *) calloc (records_per_block, sizeof(Record));  
  printf("Size of buffer: %d\n", sizeof(buffer));
  int current_conns = 0; 
  int max_conns = 0;
  int total_conns = 0;
  int current_id;
  int last_id;
  int total_ids;

  int result = fread (buffer, sizeof(Record), records_per_block, fp_read);

  last_id = buffer[0].uid1;
  if (last_id){
    total_ids++;
  }

  while(result){
    for(int i = 0; i < block_size; i += sizeof(Record)){
      current_id = buffer[i].uid1;
      if (last_id != current_id){
        if (current_conns > max_conns){
          max_conns = current_conns;
        }
        total_ids++;
        current_conns = 0;
        last_id = current_id;
      }
      total_conns++;
      current_conns++;
    }
    result = fread (buffer, sizeof(Record), records_per_block, fp_read);
  }
  ma.avg = total_conns/total_ids;
  ma.max = max_conns;
  printf("Average number of connections: %d \t; Maximum number of connections: %d \n", ma.avg, ma.max);
  return ma;
}