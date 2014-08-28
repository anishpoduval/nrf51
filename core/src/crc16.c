#include <stdint.h>
#include "crc16.h"

uint16_t crc16_continue(uint16_t crc, const uint8_t * buffer, uint32_t size) {
	if (buffer && size)
		while (size--) {
			crc = (crc >> 8) | (crc << 8);
			crc ^= *buffer++;
			crc ^= ((unsigned char) crc) >> 4;
			crc ^= crc << 12;
			crc ^= (crc & 0xFF) << 5;
		}
	return crc;
}

uint16_t crc16(const uint8_t * buffer, uint32_t size) {
	return crc16_continue(0xFFFF, buffer, size);
}

uint16_t icrc16(const uint8_t * buffer, uint32_t size) {
	return crc16(buffer, size) ^ 0xFFFF;
}
