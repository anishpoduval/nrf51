/*
 * radio.h
 *
 *  Created on: Aug 20, 2014
 *      Author: valdo
 */

#ifndef RADIO_H_
#define RADIO_H_

#define PACKED __attribute__((packed))

#define SIGNATURE_SIZE    5
#define PROXY_TAGS_LENGTH 3

typedef struct {

	uint32_t oid;
	uint8_t  batt;
	uint8_t  rssi;
	uint32_t seq;

} PACKED ProtoProxyTag;

typedef struct {

	ProtoProxyTag tags[PROXY_TAGS_LENGTH];

} PACKED ProtoProxy;

typedef struct {

} PACKED ProtoTracker;

typedef union {

	ProtoTracker tracker;
	ProtoProxy proxy;

} PACKED ProtoData;

typedef struct {

	uint8_t   proto;
	uint32_t  oid;
	uint8_t   batt;
	int8_t    temp;
	uint32_t  seq;
	ProtoData data;
	uint8_t   signature[SIGNATURE_SIZE];

} PACKED Packet;

void radio_init(void);
void radio_send(void);
bool radio_receive(void);

#endif /* RADIO_H_ */
