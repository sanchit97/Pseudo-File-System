#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int readData( int disk, int blockNum, void* block) // returns the num. of bytes read and read them into a buffer pointed by block.
{
	int flag1=lseek(disk, blockNum*block_size, SEEK_SET);
	if(flag1==-1){
		printf("Error in seek!....");
		return -1;
	}
	else {
		printf("--%d Read block at %d\n", disk,flag1);
		int flag2=read(disk,block,block_size);
		if(flag2!=block_size){
			printf("Error in read");
			printf("**%d %s %d\n", disk,(char *)block,flag2);
			return -2;
		} else {
			return block_size;
		}
	}
}

int writeData(int disk, int blockNum, void* block) //writes that data in the block to the disk block blockNum.
{
	int flag1=lseek(disk, blockNum*block_size, SEEK_SET);
	if(flag1==-1){
		printf("Error in seek!....");
		return -1;
	}
	else {
		int flag2=write(disk,block,block_size);
		if(flag2!=block_size){
			printf("Error in write");
			return -2;
		} else {
			return block_size;
		}
	}
}

int createSFS( char* filename, int nbytes) // returns a fileSystemId
{
	int i;
	int sfs_file_descriptor=open( filename, O_RDWR | O_CREAT ,S_IRUSR | S_IWUSR );	//Creating SFS in the specified file
	if (sfs_file_descriptor==-1)
	{
		perror("Cannot create SFS \nExiting....\n");
		exit(1);
	}

	char c= '\0';

	i=nbytes;
	printf("%d\n",sfs_file_descriptor );
	while(i>0)
	{
		int val = write(sfs_file_descriptor,&c, 1);
		if (val<=0)
		{
			printf("Cant Initialize the file\n");
			exit(1);
		}
		i--;
	}
	char* s=(char*)malloc(sizeof(char)*4*1024);
	strcpy(s,"SFS File created by Sanchit and Dhruva");

	writeData(sfs_file_descriptor,0,(void*)s);
	for(i=0;i<512;i++)
	{
		s[i]='0';
	}
	writeData(sfs_file_descriptor,1,(void*)s);
	writeData(sfs_file_descriptor,2,(void*)s);
	return sfs_file_descriptor;
}

int writeFile(int disk, char* filename, void* block) //write the buffer pointed by block. The same data should be recoverable by the file name.
{
	int i,j;
	char* ibmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,1,(void*)ibmap);
	char* dbmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,2,(void*)dbmap);
	char* inode_data=(char*)malloc(3*sizeof(char)*block_size);
	readData(disk,3,(void*)inode_data);
	readData(disk,4,(void*)(inode_data+4*1024));
	readData(disk,5,(void*)(inode_data+4*1024 +4*1024));	// same as readfile
	int found=-1;
	for(i=0;i<512;i++)
	{
		if(ibmap[i]=='0')
		{
			 char *c=(char*)malloc(24);
			 for(j=0;j<24;j++)
			 {
			 	c[j]=inode_data[i*24+j];
			 }
			 struct inode *inode_cur=(struct inode *)c;
			 strcpy(inode_cur->name,filename);
			 inode_cur->start_block=-1;
			 inode_cur->no_of_blocks=0;
			 inode_cur->size_of_file=0;
			 for(j=0;j<1024;j++)
			 {
			 	if(dbmap[j]=='0')
				{
					writeData(disk,6+j,block);
					inode_cur->size_of_file=block_size;
					inode_cur->start_block=j;
					inode_cur->no_of_blocks=1;
					break;
				}
			 }
			 if(inode_cur->no_of_blocks==0)
			 {
			 	printf("No Data block empty.\n");
			 	return 0;
			 }
			 for(j=0;j<24;j++)
			 {
			 	inode_data[i*24+j]=c[j];
			 }
			writeData(disk,3,(void*)inode_data);
			writeData(disk,4,(void*)(inode_data+4*1024));
			writeData(disk,5,(void*)(inode_data+4*1024 +4*1024));
			ibmap[i]='1';
			dbmap[j]='1';
			writeData(disk,1,(void*)ibmap);
			writeData(disk,2,(void*)dbmap);
			


		}

	}
	if(found!=1)
	{
		printf("No Inode empty.\n");
	}

	return 0;
}

int readFile(int disk, char* filename, void* block) //returns the data stored by the given file name.
{
	int i,j;
	char* ibmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,1,(void*)ibmap);
	char* dbmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,2,(void*)dbmap);
	char* inode_data=(char*)malloc(3*sizeof(char)*block_size);
	readData(disk,3,(void*)inode_data);
	readData(disk,4,(void*)(inode_data+4*1024));
	readData(disk,5,(void*)(inode_data+4*1024 +4*1024));
	int found=-1;
	for(i=0;i<512;i++)
	{
		if(ibmap[i]=='1')
		{
			 char *c=(char*)malloc(24);
			 for(j=0;j<24;j++)
			 {
			 	c[j]=inode_data[i*24+j];
			 }
			 struct inode *inode_cur=(struct inode *)c;
			 if(strcmp(inode_cur->name,filename)==0)
			 {
			 	found=1;
			 	for(j=0;j<inode_cur->no_of_blocks&&(j<10);j++)
			 	{
			 		readData(disk,j+inode_cur->start_block+6,(void*)(inode_data+j*4*1024));
			 	}

			 	return j*block_size;
			 }



		}

	}
	if(found!=1)
	{
		printf("No File of this name exists.\n");
	}
	return 0;
}
void print_inodeBitmaps(int fileSystemId)
{

	char* i_bmap= (char*) malloc(sizeof(char)*block_size);
	readData(fileSystemId,1,(void*)i_bmap);
	int i=0;
	int j=0;
	printf("Inode BitMap\n" );
	for(i=0;i<16;i++)
	{
		for(j=0;j<32;j++)
		{
			printf("%c ",i_bmap[i*32+j] );
		}	

		printf("\n");
	}	


}
void print_dataBitmaps(int fileSystemId)
{
	char* d_bmap= (char*) malloc(sizeof(char)*block_size);
	readData(fileSystemId,2,(void*)d_bmap);
	int i=0;
	int j=0;
	printf("Data BitMap\n" );
	for(i=0;i<16;i++)
	{
		for(j=0;j<32;j++)
		{
			printf("%c ",d_bmap[i*32+j] );
		}	

		printf("\n");
	}	

}
void print_FileList(int fileSystemId)
{
	int i,j;
	char* ibmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,1,(void*)ibmap);
	char* dbmap=(char*)malloc(sizeof(char)*block_size);
	readData(disk,2,(void*)dbmap);
	char* inode_data=(char*)malloc(3*sizeof(char)*block_size);
	readData(disk,3,(void*)inode_data);
	readData(disk,4,(void*)(inode_data+4*1024));
	readData(disk,5,(void*)(inode_data+4*1024 +4*1024));
	int count=0;
	char* res[512];
	for(i=0;i<512;i++)
	{
		res[i]=(char*)malloc(sizeof(char)*15);
		if(ibmap[i]=='1')
		{
			count++;
			char *c=(char*)malloc(24);
			for(j=0;j<24;j++)
			{
			c[j]=inode_data[i*24+j];
			}
			struct inode *inode_cur=(struct inode *)c;
			strcpy(res[count],inode_cur->name)
		}

	}
	printf("File List count:%d\n",count );
	for(i=0;i<count;i++)
	{
		printf("%s\n",res[i] );
	}
}
int main()
{
	printf("Pseudo File System Menu\n");
	int file_desc=createSFS("SFS",4*1024);
	printf("File Created");
	print_inodeBitmaps(file_desc);
	print_dataBitmaps(file_desc);
	int choice=0;
	printf("File Functions");
	printf("1..........Create a new file");
	printf("2..........Display a file");
	printf("Diagnostic Functions");
	printf("3..........Display names of all files");
	printf("4..........Print Inode Bitmap");
	printf("5..........Print Data Bitmap");
	printf("6..........Exit");
	printf("Enter Choice\n");
	while(choice>=6)
	{
		if(choice == 1)
		{
			printf("Enter name of new file");
			char fnm[10];
			scanf("%s",fnm);
			printf("Enter information in new file");
			char stuff[100];
			scanf("%s",stuff);
			writeFile(file_desc,name,(void*)stuff);
		}
		if(choice == 2)
		{
			printf("Enter name of new file");
			char fnm[10];
			scanf("%s",fnm);
			char stuff[100];
			printf("Contents of file---\n");
			readFile(file_desc,fnm,(void*)stuff);
			printf("%s",stuff);
		}
		if(choice == 3)
		{
			print_FileList(file_desc);	
		}
		if(choice == 4)
		{
			print_inodeBitmaps(file_desc);
		}
		if(choice == 5)
		{
			print_dataBitmaps(file_desc);
		}
	}
	return 0;
}+
