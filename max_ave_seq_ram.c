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

Max_average max_ave_seq_ram(FILE * fp_read);
/* allocate buffer for 1 block */

int main(int argc, char* argv[]){
  FILE* fp;

  if (argc != 2){
    printf("Usage: <input filename>\n");
    return 1;
  }

  if(!(fp = fopen(argv[1], "rb"))){
    exit(EXIT_FAILURE);
  }

  max_ave_seq_ram(fp);
  fclose(fp);

  return 0;
}

Max_average max_ave_seq_ram(FILE * fp_read){
  fseek(fp_read, 0, SEEK_END);
  long fsize = ftell(fp_read);
  fseek(fp_read, 0, SEEK_SET);

  printf("%lu\n", fsize);

  Record * buffer = (Record *) calloc (fsize, sizeof(Record));
  /* read records into buffer */
  Max_average ma;
  int current_conns = 0; 
  int max_conns = 0;
  int total_conns = 0;
  int current_id= -1;
  int last_id = -1;
  int total_ids = 0;

  int result = fread (buffer, sizeof(Record), fsize, fp_read);
  int count = 0;

  /* Go through each block */
  for(int i = 0; i < fsize; i++){
    count++;
    /* First loop */
    // if (current_id == -1){
    //   current_id = buffer[0].uid1;
    //   last_id = current_id;
    // }
    /*  */ 
    current_id = buffer[i].uid1;
    if (current_id == 0){
      break;
    }
    printf("current_id: %d\n", current_id);
//     if (current_id == last_id){
//       current_conns ++;
//     }
//     else{
//       total_ids ++;
//       last_id = current_id;
//       printf("Switch of ids. last_id: %d has %d connections. \t current_id: %d \n", last_id, current_conns, current_id);
//       if (current_conns > max_conns){
//         max_conns = current_conns;
//         current_conns = 1;
//       }
//     }
   }
   printf("%d\n", count);
//   total_conns ++;
//   result = fread (buffer, sizeof(Record), records_per_block, fp_read);

//  ma.avg = total_conns/total_ids;
//  ma.max = max_conns;
//  printf("Average number of connections: %d \t; Maximum number of connections: %d \n", ma.avg, ma.max);
  return ma;
}
