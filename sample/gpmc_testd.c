/*
 *  Sample gpmc_testd for iolib with CPS-MCS341.
 *
 *  Copyright (C) 2015 Syunsuke Okamoto.<okamoto@contec.jp>
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "cps-iolib.h"

#define DEVNAME "/dev/cps-iolib0"
#define SAMP_VER "1.0.0"

void para_err(){
	printf("*******************************************\n");
	printf(" gpmc_testd  cps-iolib version %s\n",SAMP_VER);
	printf("*******************************************\n");
	printf( "WORD access \n" );
	printf( " gpmc_testd -r address  \n" );
	printf( " gpmc_testd -w address data \n" );
	printf( "BYTE access \n" );
	printf( " gpmc_testd -r1 address  \n" );
	printf( " gpmc_testd -w1 address data \n" );
}

int main(int argc, char *argv[]){
	int fd;
	unsigned long from,num,port;
	unsigned int adr, dat;
	int w_flg=0;
	int b_flg=0;
	struct cpsio_ioctl_arg arg;

	if( argc<3 ){
		para_err();
		exit(1);
	} 
	if( memcmp( argv[1], "-w", 2 )==0 ){
		if( argc < 4 ){
			para_err();
			exit(1);
		}
		if( memcmp( argv[1], "-w1", 3 )==0 )b_flg=1;
		sscanf( argv[2], "%x", &adr );
		sscanf( argv[3], "%x", &dat );
		w_flg=1;
	}else if( memcmp( argv[1], "-r", 2 )==0){
		if( memcmp( argv[1], "-r1", 3 )==0 )b_flg=1;
		sscanf( argv[2], "%x", &adr );
	}else{
		para_err();
		exit(1);
	}

	/* デバイスをopenする */
	fd=open(DEVNAME, O_RDWR);
	if(fd<=0){
		perror(DEVNAME);
		exit(1);
	}

	if( w_flg ){
		arg.addr	= adr;
		arg.val		= dat;
		
		if( b_flg ){
			printf( "write adr[0x%x] = [%x]\n", adr, dat & 0xff  );
			ioctl( fd, IOCTL_CPSIO_OUTBYTE, &arg);	
		}else{
			printf( "write adr[0x%x] = [%x]\n", adr, dat & 0xffff  );
			ioctl( fd, IOCTL_CPSIO_OUTWORD, &arg);	
		}
	}else{
		arg.addr = adr;
		if( b_flg ){
			ioctl( fd, IOCTL_CPSIO_INPBYTE, &arg);
			printf( "read adr[0x%x] = [%x]\n", arg.addr,arg.val ); 
		}else{
			ioctl( fd, IOCTL_CPSIO_INPWORD, &arg);
			printf( "read adr[0x%x] = [%x]\n", arg.addr, arg.val ); 
		}
	}


	/* デバイスを閉じる*/
	close(fd);
	return 0;
}

