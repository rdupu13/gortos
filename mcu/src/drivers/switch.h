#ifndef SWITCH_H
#define SWITCH_H

#define SWITCH0_DIR     P5DIR
#define SWITCH0_REN     P5REN
#define SWITCH0_OUT     P5OUT
#define SWITCH0_IES     P5IES
#define SWITCH0_IE      P5IE
#define SWITCH0_IFG     P5IFG
#define SWITCH0_PIN     BIT1

#define SWITCH1_DIR     P5DIR
#define SWITCH1_REN     P5REN
#define SWITCH1_OUT     P5OUT
#define SWITCH1_IES     P5IES
#define SWITCH1_IE      P5IE
#define SWITCH1_IFG     P5IFG
#define SWITCH1_PIN     BIT0

void switch_init();

#endif
