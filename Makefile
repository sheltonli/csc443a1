CC = gcc

all: write_blocks write_lines max_ave_seq_disk max_ave_seq_ram
 
write_blocks: write_blocks.c 
	$(CC) -o $@ $<
	
write_lines: write_lines.c 
	$(CC) -o $@ $<

max_ave_seq_disk: max_ave_seq_disk.c 
	$(CC) -o $@ $<

max_ave_seq_ram: max_ave_seq_ram.c
	$(CC) -o $@ $< 	
	
	
clean: 
	rm write_blocks write_lines max_ave_seq_disk max_ave_seq_ram edges.dat
	
