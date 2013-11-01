#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>

typedef struct {
	unsigned int data;
	unsigned int offsets[37];//A-Z0-9_
} BLOCK;

char stack[512];
int sp=0;
void push(char ch){
	//printf("#Tracking %c\n",ch);
	stack[sp]=ch;
	sp++;
}
char pop(){
	int ch;
	ch=stack[sp];
	sp--;
	return ch;
}
void print_stack(){
	int i;
	for(i=0;i<sp;i++){
		printf("%c",stack[i]);
	}
	printf("\n");
}

void dumpNames(int fd,unsigned int offset){
	int len;
	int off=offset;
	char name[512];

	while(off){
		if(fd!=3){
			printf("ERROR\n");
			exit(0);
		}
		read(fd,&len,4);
		read(fd,name,len);
		name[len]=0;
		printf("%s\n",name);
		read(fd,&off,4);
		lseek(fd,off,SEEK_SET);
	}

}

int count;
void dumpBlock(int fd,unsigned int offset){
	int i;
	BLOCK block;
	lseek(fd,offset,SEEK_SET);
	read(fd,&block,sizeof(BLOCK));
	if(block.data!=0){
		print_stack();
		dumpNames(fd,block.data);
		count++;
	}
	for(i=0;i<37;i++){
		if(block.offsets[i]!=0){
			if(i<=9)push(i+'0');
			if(i>9&&i<=35)push(i+'A'-10);
			if(i==36)push('_');
			dumpBlock(fd,block.offsets[i]);
			pop();
		}
	}
}

int main(){
	int fd;
	fd=open("index.dat",O_RDONLY);
	dumpBlock(fd,0);
	printf("TOTAL=%d\n",count);
	close(fd);
}




