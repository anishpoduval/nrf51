/*
 * temp.h
 *
 *  Created on: Aug 29, 2014
 *      Author: valdo
 */

#ifndef TEMP_H_
#define TEMP_H_

extern void temp_init(void);
extern void temp_start(void);
extern uint8_t temp_get(bool wait);

#endif /* TEMP_H_ */
