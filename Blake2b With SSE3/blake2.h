#pragma once
#ifndef __BLAKE2_H__
#define __BLAKE2_H__

#include <stddef.h>
#include <stdint.h>


#define BLAKE2_API
#define BLAKE2_PRIVATE


  enum blake2b_constant
  {
    BLAKE2B_BLOCKBYTES = 128,
    BLAKE2B_OUTBYTES   = 64,
    BLAKE2B_KEYBYTES   = 64,
    BLAKE2B_SALTBYTES  = 16,
    BLAKE2B_PERSONALBYTES = 16
  };

#pragma pack(push, 1)
  typedef struct __blake2b_param
  {
    uint8_t  digest_length; // 1
    uint8_t  key_length;    // 2
    uint8_t  fanout;        // 3
    uint8_t  depth;         // 4
    uint32_t leaf_length;   // 8
    uint64_t node_offset;   // 16
    uint8_t  node_depth;    // 17
    uint8_t  inner_length;  // 18
    uint8_t  reserved[14];  // 32
    uint8_t  salt[BLAKE2B_SALTBYTES]; // 48
    uint8_t  personal[BLAKE2B_PERSONALBYTES];  // 64
  } blake2b_param;

  typedef struct __blake2b_state
  {
    uint64_t h[8];
    uint64_t t[2];
    uint64_t f[2];
    uint8_t  buf[2 * BLAKE2B_BLOCKBYTES];
    uint32_t buflen;
    uint8_t  outlen;
    uint8_t  last_node;
  } blake2b_state;

  typedef struct __blake2bp_state
  {
    blake2b_state S[4][1];
    blake2b_state R[1];
    uint8_t  buf[4 * BLAKE2B_BLOCKBYTES];
    uint32_t buflen;
    uint8_t  outlen;
  } blake2bp_state;
#pragma pack(pop)


int blake2b_init( blake2b_state *S, size_t outlen );
int blake2b_init_key( blake2b_state *S, size_t outlen, const void *key, size_t keylen );
int blake2b_init_param( blake2b_state *S, const blake2b_param *P );
int blake2b_update( blake2b_state *S, const uint8_t *in, size_t inlen );
int blake2b_final( blake2b_state *S, uint8_t *out, size_t outlen );

int blake2bp_init( blake2bp_state *S, size_t outlen );
int blake2bp_init_key( blake2bp_state *S, size_t outlen, const void *key, size_t keylen );
int blake2bp_update( blake2bp_state *S, const uint8_t *in, size_t inlen );
int blake2bp_final( blake2bp_state *S, uint8_t *out, size_t outlen );

#endif

