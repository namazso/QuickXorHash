#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QUICKXORHASH_BLOCK_SIZE (20 * 11 * 8)

struct qxhash
{
  uint8_t bytes[QUICKXORHASH_BLOCK_SIZE];
  uint64_t size;
};

void qxhash_init(struct qxhash* ctx);
void qxhash_update(struct qxhash* ctx, const uint8_t* data, size_t size);
void qxhash_final(const struct qxhash* ctx, uint8_t* out);

#define QUICKXORHASH_SIZE 20

#ifdef __cplusplus
}
#endif
