/*
 * proto.h
 *
 *  Created on: Aug 23, 2014
 *      Author: valdo
 */

#ifndef PROTO_H_
#define PROTO_H_

#define RFPROTO_TRACK 0x01
#define RFPROTO_PROXY 0x02

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

	uint32_t seq;

} PACKED ProtoTracker;

typedef union {

	ProtoTracker tracker;
	ProtoProxy proxy;

} PACKED ProtoData;

typedef struct {

	uint8_t   proto;
	uint32_t  oid;
	uint8_t   batt;
	int8_t   temp;
	ProtoData data;
	uint8_t   signature[SIGNATURE_SIZE];

} PACKED ProtoEnvelope;

#define PRINT_ENVELOPE(e) printf("ProtoEnvelope: oid=%u, batt=%u, temp=%d, proto=%u, seq=%u\r\n", \
												 e.oid,  e.batt,  e.temp, e.proto, e.data.tracker.seq);

#endif /* PROTO_H_ */
