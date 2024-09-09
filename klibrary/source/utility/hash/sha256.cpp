#include "klibrary.h"


#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))

#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))

struct SHA256Context
{
	uint8_t data[64] = {};
	uint8_t data_length = 0;
	uint32_t state[8] = {};
	uint32_t bit_length[2] = {};

	constexpr SHA256Context()
	{
		state[0] = 0x6a09e667;
		state[1] = 0xbb67ae85;
		state[2] = 0x3c6ef372;
		state[3] = 0xa54ff53a;
		state[4] = 0x510e527f;
		state[5] = 0x9b05688c;
		state[6] = 0x1f83d9ab;
		state[7] = 0x5be0cd19;
	}
};

static void transform_context(SHA256Context* context, const uint8_t* data)
{
	static uint32_t hash_keys[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
	};

	uint32_t padded[64] = {};
	for (uint8_t i = 0; i < 16; i++) {
		padded[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);
	}
	for (uint8_t i = 16; i < 64; i++) {
		padded[i] = SIG1(padded[i - 2]) + padded[i - 7] + SIG0(padded[i - 15]) + padded[i - 16];
	}

	uint32_t t[2] = {};
	uint32_t state[8] = {
		context->state[0],
		context->state[1],
		context->state[2],
		context->state[3],
		context->state[4],
		context->state[5],
		context->state[6],
		context->state[7],
	};
	for (uint8_t i = 0; i < 64; i++) {
		t[0] = state[7] + EP1(state[4]) + CH(state[4], state[5], state[6]) + padded[i] + hash_keys[i];
		t[1] = EP0(state[0]) + MAJ(state[0], state[1], state[2]);
		state[7] = state[6];
		state[6] = state[5];
		state[5] = state[4];
		state[4] = state[3] + t[0];
		state[3] = state[2];
		state[2] = state[1];
		state[1] = state[0];
		state[0] = t[0] + t[1];
	}
	for (uint8_t i = 0; i < 8; i++) {
		context->state[i] += state[i];
	}
}

static void update_context(SHA256Context* context, const uint8_t* data, const uint64_t data_size)
{
	for (uint64_t i = 0; i < data_size; ++i) {
		context->data[context->data_length] = data[i];
		context->data_length += 1;

		if (context->data_length == 64) {
			transform_context(context, context->data);

			if (context->bit_length[0] > (0xffffffff - 512)) {
				context->bit_length[1] += 1;
			}
			context->bit_length[0] += 512;
			context->data_length = 0;
		}
	}
}

static kl::Hash finalize_context(SHA256Context* context)
{
	uint8_t index = context->data_length;
	if (context->data_length < 56) {
		context->data[index++] = 0x80;
		while (index < 56) {
			context->data[index++] = 0;
		}
	}
	else {
		context->data[index++] = 0x80;
		while (index < 64) {
			context->data[index++] = 0;
		}

		transform_context(context, context->data);
		memset(context->data, 0, 56);
	}

	if (context->bit_length[0] > (0xffffffff - (context->data_length * 8))) {
		context->bit_length[1] += 1;
	}
	context->bit_length[0] += (context->data_length * 8);

	context->data[63] = context->bit_length[0];
	context->data[62] = context->bit_length[0] >> 8;
	context->data[61] = context->bit_length[0] >> 16;
	context->data[60] = context->bit_length[0] >> 24;
	context->data[59] = context->bit_length[1];
	context->data[58] = context->bit_length[1] >> 8;
	context->data[57] = context->bit_length[1] >> 16;
	context->data[56] = context->bit_length[1] >> 24;
	transform_context(context, context->data);

	kl::Hash result = {};
	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 8; j++) {
			result[i + ((size_t) j * 4)] = (context->state[j] >> (24 - (i * 8))) & 0x000000ff;
		}
	}
	return result;
}

kl::Hash kl::hash(const void* data, const uint64_t data_size)
{
	SHA256Context context{};
	update_context(&context, reinterpret_cast<const uint8_t*>(data), data_size);
	return finalize_context(&context);
}

kl::Hash kl::hash_str(const std::string_view& data)
{
	return hash(data.data(), data.size());
}
