#ifndef __CRC16_H__
#define __CRC16_H__

extern uint16_t crc16_continue (uint16_t crc, const uint8_t * buffer, uint32_t size);
extern uint16_t crc16 (const uint8_t * buffer, uint32_t size);
extern uint16_t icrc16 (const uint8_t * buffer, uint32_t size);

#endif/*__CRC16_H__*/
