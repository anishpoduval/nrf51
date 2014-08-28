#ifndef __XXTEA_H__
#define __XXTEA_H__

extern void xxtea_decode(uint32_t * v, uint32_t length,
		const uint32_t * tea_key);
extern void xxtea_encode(uint32_t * v, uint32_t length,
		const uint32_t * tea_key);

#endif/*__XXTEA_H__*/
