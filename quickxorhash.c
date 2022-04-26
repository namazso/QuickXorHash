#include "quickxorhash.h"
#include <string.h>

void qxhash_init(struct qxhash* ctx)
{
  memset(ctx, 0, sizeof(*ctx));
}

static void qxhash_block(struct qxhash* ctx, const uint8_t* data)
{
  // good compilers will vectorize this
  for (size_t i = 0; i < QUICKXORHASH_BLOCK_SIZE; ++i)
    ctx->bytes[i] ^= data[i];
}

void qxhash_update(struct qxhash* ctx, const uint8_t* data, size_t size)
{
  size_t i = 0;
  while (ctx->size % QUICKXORHASH_BLOCK_SIZE != 0 && i < size)
  {
    ctx->bytes[ctx->size % QUICKXORHASH_BLOCK_SIZE] ^= data[i];
    ++ctx->size;
    ++i;
  }
  if (i == size)
    return;
  for (; size - i >= QUICKXORHASH_BLOCK_SIZE; i += QUICKXORHASH_BLOCK_SIZE, ctx->size += QUICKXORHASH_BLOCK_SIZE)
    qxhash_block(ctx, &data[i]);
  while (i < size)
  {
    ctx->bytes[ctx->size % QUICKXORHASH_BLOCK_SIZE] ^= data[i];
    ++ctx->size;
    ++i;
  }
}

void qxhash_final(const struct qxhash* ctx, uint8_t* out)
{
  uint8_t hash[21] = {0};
  for (size_t i = 0; i < QUICKXORHASH_BLOCK_SIZE; ++i)
  {
    const size_t shift = (i * 11) % 160;
    const size_t shift_bytes = shift / 8;
    const size_t shift_bits = shift % 8;
    const size_t shifted = (size_t)ctx->bytes[i] << shift_bits;
    hash[shift_bytes] ^= shifted & 0xFF;
    hash[shift_bytes + 1] ^= (shifted >> 8) & 0xFF;
  }
  hash[0] ^= hash[20];
  for (size_t i = 0; i < 20; ++i)
    out[i] = hash[i];

  out[12] ^= (ctx->size >> 0) & 0xFF;
  out[13] ^= (ctx->size >> 8) & 0xFF;
  out[14] ^= (ctx->size >> 16) & 0xFF;
  out[15] ^= (ctx->size >> 24) & 0xFF;
  out[16] ^= (ctx->size >> 32) & 0xFF;
  out[17] ^= (ctx->size >> 40) & 0xFF;
  out[18] ^= (ctx->size >> 48) & 0xFF;
  out[19] ^= (ctx->size >> 56) & 0xFF;
}
