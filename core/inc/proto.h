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
	uint32_t rec;
	uint8_t  batt;
	int8_t   temp;
	int8_t   rssi;
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

} PACKED ProtoEnvelope;

#endif /* PROTO_H_ */
