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
	printf("\nPress enter to start...");
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
	
	// Build the powtag and load it in the Parameters.
	uint8_t * powtag; // 16-byte powtag for Zcash
	powtag=zcash_powtag();
	memcpy( P.personal, powtag, 16 );

	// Set up initial State -->  Has been verified to match a known good state from reference coder
	// Endianness is backwards but it matches.
	blake2b_state S;
	blake2b_init_param(&S,&P);
	
	//Make 140 bytes of zero for initial testing.
	//char mydata[140]={0};
	uint8_t mydata_w_index[144]={0};
	
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
	uint8_t output[50];
	printf("timing...");
	
	time_t start=clock();
	for (int i=0;i<1048576;i++){
		blake2b_update(&S,mydata_w_index,144);	
		blake2b_final(&S,output,50);
		mydata_w_index[i%50]+=1;
	}
	time_t stop = clock()-start;
	printf("\nHash time: %I64d\n",stop);
	printf("\nSeconds: %f\n",(float) stop/CLOCKS_PER_SEC);
	fflush(stdout);
	
	printf("Output Hash: >");
	for(int i=0;i<50;i++){
		printf("%02x",output[i]);
		
	}
	


	return 0;
	
}
