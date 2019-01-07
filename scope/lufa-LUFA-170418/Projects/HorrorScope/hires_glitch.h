
#ifndef _HIRES_GLITCH_H_
#define _HIRES_GLITCH_H_

void hires_init(void);
void hires_glitch(void);
void hires_stop_timer(void);
void hires_set_counter(uint8_t counter);
void hires_set_period(uint8_t period);
void hires_set_compare(uint8_t compare);
void init_timers(void);

#endif // _HIRES_GLITCH_H_
