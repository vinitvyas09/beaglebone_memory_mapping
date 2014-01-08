/****************************************************************************//**\file
 * GPIO Library
 *
 * Vinit Vyas
 ****************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

/******************************************************************************
		 Clock Module Peripheral base address and registers offsets
*******************************************************************************/

#define CM_WKUP                 0x44E00400  			//for enabling GPIO0 clk
#define CM_WKUP_GPIO0_CLKCTRL   0x08
 
#define CM_PER                  0x44E00000  			//for enabling GPIO 1,2,3 clks
#define CM_PER_GPIO1_CLKCTRL    0xAC
#define CM_PER_GPIO2_CLKCTRL    0xB0
#define CM_PER_GPIO3_CLKCTRL    0xB4
#define CM_MAP_SIZE             2048

/******************************************************************************/

#define GPIO0_START_ADDR		0x44e07000
#define GPIO0_END_ADDR			0x44e08FFF
#define GPIO1_START_ADDR 		0x4804C000
#define GPIO1_END_ADDR 			0x4804DFFF
#define GPIO2_START_ADDR 		0x481AC000
#define GPIO2_END_ADDR 			0x481ADFFF
#define GPIO3_START_ADDR 		0x481AE000
#define GPIO3_END_ADDR 			0x481AFFFF

#define GPIO_SIZE 				(GPIO1_END_ADDR - GPIO1_START_ADDR)
#define GPIO_OE 				0x134
#define GPIO_SETDATAOUT 		0x194
#define GPIO_CLEARDATAOUT		0x190
#define GPIO_DATAIN 			0x138

#define USR0_LED 				(1<<21)
#define USR1_LED 				(1<<22)
#define USR2_LED 				(1<<23)
#define USR3_LED 				(1<<24)


enum PinValue_t
{
    LOW=0,
    HIGH=1
};

enum PinDirection_t
{
    INPUT=0,
    OUTPUT=1
};

int gpioInit();
int gpioSetPinDirection(unsigned int pinNo, enum PinDirection_t);
void gpioWritePin(unsigned int pinNo, enum PinValue_t value);
enum PinValue_t gpioReadPin(unsigned int pinNo);
int mapping(unsigned int);
void cleanup();

#endif

