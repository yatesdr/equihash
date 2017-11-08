#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "blake2.h"


unsigned short* set_data(char* str){
	int size=sizeof(&str);
	printf("Size: %d", size);
	unsigned short* mem = malloc(ceil(size));
	return mem;
}

int cmpfunc (const void * a, const void * b) {

	uint32_t first= *(uint8_t*)a*65535+*(uint8_t*)(a+1)*256+*(uint8_t*)(a+2);
	uint32_t second= *(uint8_t*)b*65535+*(uint8_t*)(b+1)*256+*(uint8_t*)(b+2);
	
	if (first>second) return 1;
	if (second>first) return -1;
	if (first==second) return 0;
	
	printf("Nothing happened");
	
	return 0;

   //return ( *(uint16_t*)a - *(uint16_t*)b );
}

/*
char* hex_encode(char* asc, int size){
	
	char* output;
	output=malloc((size-1)*2+1);
	
	for(int i=0;i<size;i++){
		sprintf(output+i*2,"%02X",asc[i]);
	}
	return output;	
}
*/

static inline int blake2b_param_set_personal( blake2b_param *P, const uint8_t personal[BLAKE2B_PERSONALBYTES] )
{
  memcpy( P->personal, personal, BLAKE2B_PERSONALBYTES );
  return 0;
}

uint8_t * zcash_powtag(void){
	// Clean out the memory.
	uint8_t* powtag=(uint8_t*)malloc(16);
	memset(powtag,0,16);
	
	const uint8_t zcpow[]="ZcashPoW";
	memcpy(powtag,zcpow,8);

	// Load the other two memory positions manually. n=200,k=9.
	// Load at 8,12 instead of 11,14 because little-endian
	powtag[8]=0xc8; //n=200	
	powtag[12]=0x09; //k=9

	printf("\n");
	for (int i=0;i<16;i++){
		printf("%02X",powtag[i]);
	}
	return powtag;
}

int main(void){
	
	// Blake2b-L(p,x) - unkeyed, sequential mode, output digest length of L/8 bytes, 16-byte personalization string p, 
	// input x of 
	// Personal bytes:  "ZcashPoW" (may need endainness checks)
	// Needs to be 
	// Let powtag := [64-bit "ZCashPoW"]+[32-bits]+[32-Bits]
	
	//const unsigned short powtag = []
	//powtag = "ZcashPoW"
	//powcount=32-bit g
	//powheader=	[32-bit nVersion][256-bit hashPrevBlock][256-bit hashMerkleRoot]
	//				[256-bit hashReserved][32-bit nTime][32-bit nBits][256-bit nOnce]
	
	printf("In main...\n");
	printf("\nPress Enter to start..");
	fflush(stdout);
	getchar();
	
	// Set up the initializing parameters for ZCash
	blake2b_param P;
	P.digest_length=50; //50 for zcash
	P.key_length=0; //0
	P.fanout=1; //1
	P.depth=1; //1
	P.leaf_length=(uint32_t)0; //0
	P.node_offset=(uint64_t)0; //0
	P.node_depth=0; //0
	P.inner_length=0; //0

	// Set the salt to zero.
	printf("memcpy");
	uint8_t salt[16]={0};

	memcpy(P.salt,salt,16);
	printf("Salt: >");
	for (int q=0;q<16;q++){
		printf("%02x",P.salt[q]);
	}
	// Build the powtag and load it in the Parameters.
	uint8_t * powtag; // 16-byte powtag for Zcash
	powtag=zcash_powtag();
	memcpy( P.personal, powtag, 16 );

	// Set up initial State -->  Has been verified to match a known good state from reference coder
	// Endianness is backwards but it matches.
	blake2b_state S;
	blake2b_init_param(&S,&P);
	
	//Make 140 bytes of zero for initial testing.
	//uint8_t mydata[140]={0};
	
/* 	for(int i=0;i<140;i++){
		mydata[i]=0;
	} */

	
	printf("Dumping initialized state>>\n");
	uint8_t * dump1=(uint8_t*)&S;
	
	//printf("");
	for(int i=0;i<64;i++){

		if (i%8==0 && i!=0){
			printf("*\n");
		}
		printf("%02X",dump1[i]);
	}
	

	
	printf("\nDumping Little-Endian initialized state...>\n");
	fflush(stdout);
	
    for (int j=1;j<=8;j++){
		for (int i=1;i<=8;i++){
			printf("%02X",dump1[8*j-i]);
		}
			printf("*\n");
	}

	
	/*)
	int outsize=0;
	uint8_t *output;
	output=malloc(52);
	printf("Copying mem");
	uint8_t d=0;
	for (int i=0;i<50;i++){
		memcpy(&output+i,&d,1);
	}
	printf("done allocating");
	
	for(int i=0;i<40;i++){
		printf("%02x",output[i]);
	}
	fflush(stdout);
	*/
	//uint8_t output[50];
	uint8_t* output;
	uint8_t* rstart;
	output = malloc(50*1048577*sizeof(uint8_t));
	rstart=output;
	//output[10][50];
	
	#pragma pack(push,1)
	struct __data {
		uint8_t p[140];
		uint32_t index;
	} data;
	#pragma pack(pop)
	
	for(int e=0;e<140;e++){
		data.p[e]=0;
	}
	data.index=0;
	printf("\nData dumper:>");
	uint8_t* dumper = (uint8_t*)&data;
	for (int w=0;w<144;w++){
		printf("%02x",dumper[w]);
	}
	
	printf("\nSizeof Data: %I64d\n",sizeof(data));
	fflush(stdout);
	time_t start=clock();
	for (uint32_t i=0;i<1048576;i++){
		blake2b_update(&S,(uint8_t*)&data,144);	
		blake2b_final(&S,output,50);
		data.index+=1;
		output+=50;	
	}
	
	printf("\nData.index ->%d",data.index);
	fflush(stdout);
	time_t stop = clock()-start;
	printf("\nHash time: %I64d\n",stop);
	printf("\nSeconds: %f\n",(float) stop/CLOCKS_PER_SEC);

	//Print the first 10 hashes to see them.
	for(int j=0;j<10;j++){
		printf("\nHash %d>",j);
		for(int i=0;i<50;i++){
			printf("%02x",*(rstart+j*50+i));
		}
	}
	
		//Print the first 10 hashes to see them.
	for(int j=0;j<10;j++){
		printf("\nHash %d>",j);
		for(int i=0;i<25;i++){
			printf("%02x",*(rstart+j*25+i));
		}
	}
	
	qsort(rstart,1048576*2,25,cmpfunc);

	//Print the first 10 hashes to see them.
	for(int j=0;j<10;j++){
		printf("\nHash %d>",j);
		for(int i=0;i<25;i++){
			printf("%02x",*(rstart+j*25+i));
		}
	}


	return 0;
	
}
