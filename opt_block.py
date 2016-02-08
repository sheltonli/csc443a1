import subprocess 


SECTOR = 512
KB = 1024
S1 = 4 * KB
S2 = 8 * KB
S3 = 16 * KB
S4 = 32 * KB
MB = 1024*KB
S6 = 2 * MB
S7 = 4 * MB



sizes = [SECTOR, KB, S1, S2, S3, S4, MB, S6, S7]
opt_block = 1024

for size in sizes:
	print 'Rate of writing blocks to disk (Block size = {0})'.format(size)
	subprocess.call (["write_blocks", "g_plusAnonymized.csv", str(size)])
	print '\n'
 
 
for size in sizes:
	print 'Rate of writing lines to disk'
	subprocess.call (["write_lines", "g_plusAnonymized.csv"])
	print '\n'
	
print 'Comparing sequential read rate with reading in ram'
print 'Rate of sequential disk read: '
subprocess.call (["max_ave_seq_disk", "edges.dat", str(opt_block)])
print 'Rate of RAM read'
subprocess.call (["max_ave_seq_ram", "edges.dat"])
	 

  
