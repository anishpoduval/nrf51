/*
 * store.h
 *
 *  Created on: Sep 3, 2014
 *      Author: valdo
 */

#ifndef STORE_H_
#define STORE_H_

void store_save(const ProtoEnvelope *env, const int8_t rssi);
void store_get(ProtoEnvelope *env);
void store_init(void);

#endif /* STORE_H_ */
