#ifndef __XXTEA_H__
#define __XXTEA_H__

static inline uint16_t htons(uint16_t x) {
	__asm__ ("rev16 %0, %1": "=r" (x):"r" (x));
	return x;
}

static inline uint32_t htonl(uint32_t x) {
	__asm__ ("rev %0, %1": "=r" (x):"r" (x));
	return x;
}

extern void xxtea_decode(uint32_t * v, uint32_t length,
		const uint32_t * tea_key);
extern void xxtea_encode(uint32_t * v, uint32_t length,
		const uint32_t * tea_key);

#endif/*__XXTEA_H__*/
