#ifndef _PINCONTROL_H__
#define _PINCONTROL_H__

#define pincontrol_set_pin(reg,pin) reg |= (1<<pin)
#define pincontrol_reset_pin(reg,pin) reg &= ~(1<<pin)
#define pincontrol_output_dir_pin(reg,pin) reg |= (1<<pin)
#define pincontrol_input_dir_pin(reg,pin) reg &= ~(1<<pin)

#endif
