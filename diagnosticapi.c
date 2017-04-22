#include "diagnosticapi.h"
#include ""

// SFS FILE STRUCTURE DEFINITION IN Bytes 
#define  start_super_block 0 // super block size is 4KB
#define  start_i_bitmap 4*1024 // Inode Bit Map - 4KB Size
#define  start_d_bitmap 4*1024 + 4*1024 // Data Bit Map - 4KB Size
#define  start_i_data 4*1024 + 4*1024 + 4*1024	// Inode Data - No of Inode* Size of Inode 
#define  start_data 4*1024 + 4*1024 + 4*1024 +				// Rest of the defined filesystem

#define  block_size 4*1024 


int createSFS( char* filename, int nbytes) // returns a fileSystemId 
{

}
int readData( int disk, int blockNum, void* block) // returns the num. of bytes read and read them into a buffer pointed by block. 
{

}

int writeData(int disk, int blockNum, void* block) //writes that data in the block to the disk block blockNum. 
{

}

int writeFile(int disk, char* filename, void* block) //write the buffer pointed by block. The same data should be recoverable by the file name. 
{

}

int readFile(int disk, char* filename, void* block) //returns the data stored by the given file name.
{
	
}
