#include <diagnosticapi.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// SFS FILE STRUCTURE DEFINITION IN Bytes
#define  start_super_block 0 // super block size is 4KB
#define  start_i_bitmap 4*1024 // Inode Bit Map - 4KB Size
#define  start_d_bitmap 4*1024 + 4*1024 // Data Bit Map - 4KB Size
#define  start_i_data 4*1024 + 4*1024 + 4*1024	// Inode Data - No of Inode 512 * Size of Inode 24 = 3*4*1024= 12KB
#define  start_data 4*1024 + 4*1024 + 4*1024 +	12*1024			// Rest of the defined filesystem
#define  block_size 4*1024

struct inode     //size is 24 bytes
{
	char name[12];
	int  start_block;  // data block starting from address start_data 0-...
	int no_of_blocks;
	int size_of_file; //in bytes
};
int createSFS( char* filename, int nbytes) // returns a fileSystemId
{
	int i;
	int sfs_file_descriptor=open( filename, O_WRONLY | O_CREAT | O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP );	//Creating SFS in the specified file
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
	int flag1=lseek(disk, blockNum*block_size, SEEK_SET);
	if(flag==-1){
		printf("Error in seek!....");
	}
	else {
		int flag2=read(disk,block,block_size);
		if(flag2!=block_size){
			printf("Error in read");
		} else {
			return block_size;
		}
	}
}

int writeData(int disk, int blockNum, void* block) //writes that data in the block to the disk block blockNum.
{
	int flag1=lseek(disk, blockNum*block_size, SEEK_SET);
	if(flag==-1){
		printf("Error in seek!....");
	}
	else {
		int flag2=write(disk,block,block_size);
		if(flag2!=block_size){
			printf("Error in write");
		} else {
			return block_size;
		}
	}
}

int writeFile(int disk, char* filename, void* block) //write the buffer pointed by block. The same data should be recoverable by the file name.
{

}

int readFile(int disk, char* filename, void* block) //returns the data stored by the given file name.
{

}
