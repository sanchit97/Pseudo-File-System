#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//file api functions here
#include "fileapi.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// SFS FILE STRUCTURE DEFINITION IN Bytes
#define  start_super_block 0 // super block size is 4KB
#define  start_i_bitmap 4*1024 // Inode Bit Map - 4KB Size
#define  start_d_bitmap 4*1024 + 4*1024 // Data Bit Map - 4KB Size
#define  start_i_data 4*1024 + 4*1024 + 4*1024	// Inode Data - No of Inode* Size of Inode =80
#define  start_data 4*1024 + 4*1024 + 4*1024 +				// Rest of the defined filesystem

#define  block_size 4*1024


int createSFS( char* filename, int nbytes) // returns a fileSystemId
{
	int i;

	int sfs_file_descriptor=open(filename, O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP );//Creating SFS in the specified file
	if (sfs_file_descriptor==-1)
	{
		perror("Cannot create SFS \nExiting....\n");
		exit(1);
	}

	i=nbytes;

	while(i>0)
	{
		int val = write(sfs_file_descriptor,'\0', 1);				// Initialize the file with null;
		if (val<=0)
		{
			printf("Cant Initialize the file\n");
			exit(1);
		}
	}
	return sfs_file_descriptor;
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
