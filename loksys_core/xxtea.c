#include <stdint.h>
#include "xxtea.h"

static const uint32_t DELTA = 0x9e3779b9UL;

#define MX ( (((z>>5)^(y<<2))+((y>>3)^(z<<4)))^((sum^y)+(tea_key[(p&3)^e]^z)) );

static void xxtea_shuffle(uint32_t * v, uint32_t length) {
	while (length--) {
		*v = htonl(*v);
		v++;
	}
}

void xxtea_encode(uint32_t * v, uint32_t length, const uint32_t * tea_key) {
	uint32_t z, y, sum, e, p, q;

	if (!v || !tea_key)
		return;

	xxtea_shuffle(v, length);

	q = 6 + 52 / length;
	sum = 0;
	z = v[length - 1];
	do {
		sum += DELTA;
		e = (sum >> 2) & 3;

		for (p = 0; p < (length - 1); p++)
			y = v[p + 1], z = v[p] += MX
		;

		y = v[0];
		z = v[length - 1] += MX
		;
	} while (--q);

	xxtea_shuffle(v, length);
}

void xxtea_decode(uint32_t * v, uint32_t length, const uint32_t * tea_key) {
	uint32_t z, y, sum, e, p, q;

	if (!v || !tea_key)
		return;

	xxtea_shuffle(v, length);

	q = 6 + 52 / length;
	sum = q * DELTA;
	y = v[0];
	while (sum) {
		e = (sum >> 2) & 3;
		for (p = length - 1; p > 0; p--)
			z = v[p - 1], y = v[p] -= MX
		;

		z = v[length - 1];
		y = v[0] -= MX
		;
		sum -= DELTA;
	}

	xxtea_shuffle(v, length);
}
