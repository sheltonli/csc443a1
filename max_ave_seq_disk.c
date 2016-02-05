#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct max_avg{
  float avg;
  int max;
} Max_average;

typedef struct record {
  int uid1;
  int uid2;
} Record;

const int MB = 1024 * 1024;

void max_ave_seq_disk(FILE * fp_read, int block_size);
/* allocate buffer for 1 block */

int main(int argc, char* argv[]){
  FILE* fp;

  if (argc != 3){
    printf("Usage: <input filename> <block size>\n");
    return 1;
  }

  int block_size = atoi(argv[2]);

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

void max_ave_seq_disk(FILE * fp_read, int block_size){
  Max_average ma;
  int records_per_block = block_size/sizeof(Record);
  Record * buffer = (Record *) calloc (records_per_block, sizeof(Record));  

  int result = fread (buffer, sizeof(Record), records_per_block, fp_read);
  int current_conns = 0; 
  int max_conns = 0;
  int total_conns = 0;
  int current_id= -1;
  int last_id = -1;
  int total_ids = 0;
  int i;

  clock_t begin, end;
  double time_spent;

  begin = clock();
  while(result > 0){
    printf("RESULT: %d\n", result);

    /* Go through each block */
    for(i = 0; i < records_per_block; i ++){
      current_id = buffer[i].uid1;

      /* First loop */
      if (last_id == -1){
        last_id = current_id;
        total_ids = 1;
      }
      if (current_id == last_id){
        current_conns ++;
      }
      else{
        printf("Switch of ids\n id: %d with %d connections \n" , last_id, current_conns);
        total_conns += current_conns;
        total_ids ++;
        last_id = current_id;
        if (current_conns > max_conns){
          max_conns = current_conns;
        }
        current_conns = 1;
      }
    }
    result = fread (buffer, sizeof(Record), records_per_block, fp_read);
  }  
  end = clock();

  printf("total connections: %d\n", total_conns);
  printf("total ids: %d\n", total_ids);

  ma.avg = total_conns/total_ids;
  ma.max = max_conns;
  printf("Average number of connections: %d \t; Maximum number of connections: %d \n", ma.avg, ma.max);
  
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf ("Data rate: %d MBPS\n", ((total_conns*sizeof(Record))/time_spent)/MB);
}