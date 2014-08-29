/*
 * temp.h
 *
 *  Created on: Aug 29, 2014
 *      Author: valdo
 */

#ifndef TEMP_H_
#define TEMP_H_

#define TEMP_MASK_SIGN (0x00000200UL)
#define TEMP_MASK_SIGN_EXTENSION (0xFFFFFC00UL)

// Workaround for PAN_028 rev1.1 anomaly 24 - TEMP:
// Negative measured values are not represented correctly
#define TEMP_CALC(t) (((t & TEMP_MASK_SIGN) != 0) ? (t | TEMP_MASK_SIGN_EXTENSION) : (t))

extern void temp_init(void);
extern void temp_start(void);
extern int8_t temp_get(bool wait);

#endif /* TEMP_H_ */
