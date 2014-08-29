
#ifndef __TIMER_H__
#define __TIMER_H__

#define LF_FREQUENCY 32768UL

#define SECONDS(x) ((uint32_t)((LF_FREQUENCY * x) + 0.5))
#define MILLISECONDS(x) ((uint32_t)(((LF_FREQUENCY * x) / 1000.0) + 0.5))

extern void timer_init(void);

#endif/*__TIMER_H__*/
