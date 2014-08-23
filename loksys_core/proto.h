/*
 * proto.h
 *
 *  Created on: Aug 23, 2014
 *      Author: valdo
 */

#ifndef PROTO_H_
#define PROTO_H_

#define PACKED  __attribute__((__packed__))

#define RFPROTO_TRACK 0x01
#define RFPROTO_PROXY 0x02

#define PROXY_TAGS_LENGTH 8

typedef struct {

	uint32_t oid;
	uint16_t batt;
	uint8_t  rssi;

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

	uint8_t proto;
	uint32_t oid;
	uint16_t batt;
	ProtoData data;

} PACKED ProtoEnvelope;

#endif /* PROTO_H_ */
