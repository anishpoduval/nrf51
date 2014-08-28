/*
 * radio.h
 *
 *  Created on: Aug 28, 2014
 *      Author: valdo
 */

#ifndef RADIO_H_
#define RADIO_H_

#define TRACK_CHANNEL 7UL
#define RADIO_TXPOWER (RADIO_TXPOWER_TXPOWER_Pos << RADIO_TXPOWER_TXPOWER_Pos)
#define RADIO_TRACK_TXADDRESS 0
#define NRF_MAC_SIZE          5UL
#define NRF_TRACK_SIZE sizeof(ProtoEnvelope)
#define RADIO_TRACK_PCNF1 \
		(RADIO_PCNF1_WHITEEN_Disabled << RADIO_PCNF1_WHITEEN_Pos) |\
		(RADIO_PCNF1_ENDIAN_Big       << RADIO_PCNF1_ENDIAN_Pos)  |\
		((NRF_MAC_SIZE-1UL)           << RADIO_PCNF1_BALEN_Pos)   |\
		(NRF_TRACK_SIZE               << RADIO_PCNF1_STATLEN_Pos) |\
		(NRF_TRACK_SIZE               << RADIO_PCNF1_MAXLEN_Pos)


ProtoEnvelope g_env ALIGN4;
uint8_t g_enc[sizeof(g_env)] ALIGN4;

void radio_init(void);

#endif /* RADIO_H_ */
