#include <stddef.h>
#include <stdint.h>
#include <string.h>


enum blake2b_constant
{
BLAKE2B_BLOCKBYTES = 144, //128
BLAKE2B_OUTBYTES   = 50, //64
BLAKE2B_KEYBYTES   = 0, //64
BLAKE2B_SALTBYTES  = 16, //16
BLAKE2B_PERSONALBYTES = 16 //16
};

static inline uint64_t load64( const void *src )
{
  const uint8_t *p = ( uint8_t * )src;
  uint64_t w = *p++;
  w |= ( uint64_t )( *p++ ) <<  8;
  w |= ( uint64_t )( *p++ ) << 16;
  w |= ( uint64_t )( *p++ ) << 24;
  w |= ( uint64_t )( *p++ ) << 32;
  w |= ( uint64_t )( *p++ ) << 40;
  w |= ( uint64_t )( *p++ ) << 48;
  w |= ( uint64_t )( *p++ ) << 56;
  return w;
}

static inline void store32( void *dst, uint32_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
}

static inline void store64( void *dst, uint64_t w )
{
  uint8_t *p = ( uint8_t * )dst;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w; w >>= 8;
  *p++ = ( uint8_t )w;
}

static inline uint64_t rotr64( const uint64_t w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 64 - c ) );
}

/* prevents compiler optimizing out memset() */
static inline void secure_zero_memory(void *v, size_t n)
{
  static void *(*const volatile memset_v)(void *, int, size_t) = &memset;
  memset_v(v, 0, n);
}

// Packed structs
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


// Function Definitions
int blake2b_init( blake2b_state *S, size_t outlen );
int blake2b_init_key( blake2b_state *S, size_t outlen, const void *key, size_t keylen );
int blake2b_init_param( blake2b_state *S, const blake2b_param *P );
int blake2b_update( blake2b_state *S, const uint8_t *in, size_t inlen );
int blake2b_final( blake2b_state *S, uint8_t *out, size_t outlen );


