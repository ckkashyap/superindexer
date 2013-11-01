#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>


#ifdef WINDOWS
#define BINARY |O_BINARY
#else
#define BINARY ,S_IRWXU
#endif



typedef struct {
	unsigned int data;
	unsigned int offsets[37];//A-Z0-9_
} BLOCK;


void allocateNewBlock(int fd,BLOCK *block){
	BLOCK newblock={0};
	*block=newblock;
	lseek(fd,0,SEEK_END);
	write(fd,&newblock,sizeof(BLOCK));
}

void addWord(char *word,char *filename,int fd){
	BLOCK block;
	FILE *fp;
	int ret;
	unsigned int ch;
	unsigned int currentOffset;
	char totalFileName[512];
	int fileNameLen=strlen(filename);
	currentOffset=lseek(fd,0,SEEK_SET);
	read(fd,&block,sizeof(BLOCK));

	while(ch=*word++){
		if(ch>='0' && ch <='9'){
			ch-='0';
		}else if(ch>='A' && ch <='Z'){
			ch-='A';
			ch+=10;//0-9
		}else if(ch=='_'){
			ch=36;
		}
		if( block.offsets[ch]==0){
			block.offsets[ch]=lseek(fd,0,SEEK_END);
			lseek(fd,currentOffset,SEEK_SET);
			//the following write is  basically updating the
			//block address for the current character.
			write(fd,&block,sizeof(BLOCK));
			//this is adding the new block.
			allocateNewBlock(fd,&block);
			currentOffset=lseek(fd,-(sizeof(BLOCK)),SEEK_CUR);
		}else{
			currentOffset=lseek(fd,block.offsets[ch],SEEK_SET);
			read(fd,&block,sizeof(BLOCK));
			currentOffset=lseek(fd,-(sizeof(BLOCK)),SEEK_CUR);
		}
	}


	if(block.data==0){
		block.data=lseek(fd,0,SEEK_END);
		lseek(fd,currentOffset,SEEK_SET);
		write(fd,&block,sizeof(BLOCK));
		lseek(fd,0,SEEK_END);
		write(fd,&fileNameLen,4);
		write(fd,filename,fileNameLen);
		fileNameLen=0;
		write(fd,&fileNameLen,4);
	}else{
		int len,off;
		write(fd,&block,sizeof(BLOCK));
		off=block.data;
		while(off!=0){
			lseek(fd,off,SEEK_SET);
			read(fd,&len,4);
			currentOffset=lseek(fd,len,SEEK_CUR);
			read(fd,&off,4);
		}
		off=lseek(fd,0,SEEK_END);
		lseek(fd,currentOffset,SEEK_SET);
		write(fd,&off,4);
		lseek(fd,0,SEEK_END);

		write(fd,&fileNameLen,4);
		write(fd,filename,fileNameLen);
		fileNameLen=0;
		write(fd,&fileNameLen,4);
	}
	lseek(fd,0,SEEK_END);
}

int initializeIndex(char *name){
	int fd;
	BLOCK newblock={0};
	fd=open(name,O_RDWR BINARY);
	if(fd<0){
		fd=open(name,O_RDWR|O_CREAT BINARY);
		if(fd<0){
			perror("Error opening index file");
			exit(0);
		}
		write(fd,&newblock,sizeof(BLOCK));
	}
	return fd;
}

int main(){
	char word[256];
	char fileName[512];
	int fd;
	fd=initializeIndex("index.dat");
	while(1){
		scanf("%s",word);
		scanf("%s",fileName);
		if(!strcmp(fileName,"ENDOFINPUT"))break;
		addWord(word,fileName,fd);
	}
	close(fd);
	//addWord("012","SomeFile",fd);
	//addWord("01","SomeOtherFile",fd);
	//addWord("KASHYAP","SomeOtherFile",fd);
	//addWord("KASHYAP","DINGO",fd);
	//addWord("KASHYAP1","DINGO",fd);
	//addWord("RISHI","DINGO",fd);
//	char number[108];
//	char printer[108];
//	int i;
//	for(i=0;i<108;i++)number[i]=0;
//	for(i=0;i<10000;i++){
//		getNextNumber(number);
//		getNameFromNumber(number,printer);
//		printf("%s\n",printer);
//	}
}




