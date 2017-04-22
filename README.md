# Pseudo-File-System
A simple file system. File System is developed on top of the file.

APIs: 

1. int createSFS( char* filename, int nbytes) – returns a fileSystemId 
2. int readData( int disk, int blockNum, void* block) – returns the num. of bytes read and read them into a buffer pointed by block. 
3. int writeData(int disk, int blockNum, void* block) – writes that data in the block to the disk block blockNum. 

File System API: 

4. int writeFile(int disk, char* filename, void* block): write the buffer pointed by block. The same data should be recoverable by the file name. 
5. int readFile(int disk, char* filename, void* block): returns the data stored by the given file name. 

Organization: Used the inode design with bitmaps for your organization. 

Diagnostic APIs: 

6. void print_inodeBitmaps(int fileSystemId): prints the bitmaps of inodes 
7. void print_dataBitmaps(int fileSystemId): prints the bitmaps of data block 
8. void print_FileList(int fileSystemId): prints all the files in the file system 
