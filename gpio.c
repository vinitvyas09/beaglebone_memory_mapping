/****************************************************************************//**\file
 * This file contains the mapping function - mmap()
 *
 * Vinit Vyas
 ****************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"
#include "math.h"
#include "signal.h"
#include "pthread.h"
#include "mqueue.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "gpio.h"

#ifndef MAPPINGFUNCTION_H
#define MAPPINGFUNCTION_H


int fd;
volatile void *gpio_addr0 = NULL, *gpio_addr1 = NULL;
volatile void *gpio_addr2 = NULL, *gpio_addr3 = NULL;
volatile unsigned int *data_in_addr0=NULL, *data_in_addr1=NULL;
volatile unsigned int *data_in_addr2=NULL, *data_in_addr3=NULL;
volatile unsigned int *gpio_oe_addr0 = NULL,*gpio_oe_addr1 = NULL;
volatile unsigned int *gpio_oe_addr2 = NULL,*gpio_oe_addr3 = NULL;
volatile unsigned int *gpio_setdataout_addr0 = NULL, *gpio_setdataout_addr1 = NULL;
volatile unsigned int *gpio_setdataout_addr2 = NULL, *gpio_setdataout_addr3 = NULL;
volatile unsigned int *gpio_cleardataout_addr0 = NULL,*gpio_cleardataout_addr1 = NULL;
volatile unsigned int *gpio_cleardataout_addr2 = NULL,*gpio_cleardataout_addr3 = NULL;
volatile unsigned int *gpio_datain_addr0=NULL,*gpio_datain_addr1=NULL;
volatile unsigned int *gpio_datain_addr2=NULL,*gpio_datain_addr3=NULL;
volatile unsigned int *clock_base=NULL;

int gpioInit()
{

    fd = open("/dev/mem", O_RDWR);
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(1);
    }

    gpio_addr0 = mmap
    (
          0,                    //Any address in our space will do
          GPIO_SIZE,            //Map length
          PROT_READ|PROT_WRITE,            // Enable reading & writing to mapped memory
          MAP_SHARED,           //Shared with other processes
          fd,                   //File to map
          GPIO0_START_ADDR      //Offset to GPIO peripheral
    );

    if (gpio_addr0 == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping the file - GPIO0");
        exit(1);
    }

    gpio_oe_addr0 = gpio_addr0 + GPIO_OE;
    gpio_setdataout_addr0 = gpio_addr0 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr0 = gpio_addr0 + GPIO_CLEARDATAOUT;
    gpio_datain_addr0=gpio_addr0 + GPIO_DATAIN;   

    gpio_addr1 = mmap
    (
          0,                    //Any address in our space will do
          GPIO_SIZE,            //Map length
          PROT_READ|PROT_WRITE,            // Enable reading & writing to mapped memory
          MAP_SHARED,           //Shared with other processes
          fd,                   //File to map
          GPIO1_START_ADDR      //Offset to GPIO peripheral
    );

    if (gpio_addr1 == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping the file - GPIO1");
        exit(1);
    }

    gpio_oe_addr1 = gpio_addr1 + GPIO_OE;
    gpio_setdataout_addr1 = gpio_addr1 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr1 = gpio_addr1 + GPIO_CLEARDATAOUT;
    gpio_datain_addr1=gpio_addr1 + GPIO_DATAIN;


    gpio_addr2 = mmap
    (
          0,                    //Any address in our space will do
          GPIO_SIZE,            //Map length
          PROT_READ|PROT_WRITE,            // Enable reading & writing to mapped memory
          MAP_SHARED,           //Shared with other processes
          fd,                   //File to map
          GPIO2_START_ADDR      //Offset to GPIO peripheral
    );
    if (gpio_addr2 == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping the file - GPIO2");
        exit(1);
    }

    gpio_oe_addr2 = gpio_addr2 + GPIO_OE;
    gpio_setdataout_addr2 = gpio_addr2 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr2 = gpio_addr2 + GPIO_CLEARDATAOUT;
    gpio_datain_addr2=gpio_addr2 + GPIO_DATAIN;


    gpio_addr3 = mmap
    (
          0,                    //Any address in our space will do
          GPIO_SIZE,            //Map length
          PROT_READ|PROT_WRITE,            // Enable reading & writing to mapped memory
          MAP_SHARED,           //Shared with other processes
          fd,                   //File to map
          GPIO3_START_ADDR      //Offset to GPIO peripheral
    );

    if (gpio_addr3 == MAP_FAILED)
    {
        close(fd);
        perror("Error mapping the file - GPIO1");
        exit(1);
    }

    gpio_oe_addr3 = gpio_addr3 + GPIO_OE;
    gpio_setdataout_addr3 = gpio_addr3 + GPIO_SETDATAOUT;
    gpio_cleardataout_addr3 = gpio_addr3 + GPIO_CLEARDATAOUT;
    gpio_datain_addr3=gpio_addr3 + GPIO_DATAIN;

/******************************************************************************************
	Enable clock for GPIO peripherals. In BBB GPIO clocks are not enabled by default!
	// See https://groups.google.com/forum/#!msg/beagleboard/OYFp4EXawiI/aPDQO22O-AwJ
	// Clock registers are in the AM335x datasheet: p544 (CM_PER) and p608 (CM_WKUP)
*******************************************************************************************/

	clock_base  = mmap
	(
		  0,                    //Any address in our space will do
		  CM_MAP_SIZE,          //Map length
		  PROT_READ|PROT_WRITE, //Enable reading & writing to mapped memory
		  MAP_SHARED,           //Shared with other processes
		  fd,                   //File to map
		  CM_PER                //Offset to Clock register CM_PER, CM_WKUP is just 1KB behind
	);
	 
	if (clock_base == MAP_FAILED)
	{
		close(fd);
		perror("Error enabling clocks in GPIO peripherals");
		exit(1);
	}
	 
	// Enable GPIO0 Clocks
	clock_base[ (CM_WKUP - CM_PER + CM_WKUP_GPIO0_CLKCTRL ) / 4 ] = 2;
	clock_base[ CM_PER_GPIO1_CLKCTRL / 4 ] = 2;
	clock_base[ CM_PER_GPIO2_CLKCTRL / 4 ] = 2;
	clock_base[ CM_PER_GPIO3_CLKCTRL / 4 ] = 2;

	if (munmap((void*)((int)clock_base),CM_MAP_SIZE)!=0)
	{
		close(fd);
		perror("Error unmapping the clocks registers");
		exit(1);
	}


    return(0);
}

int gpioSetPinDirection(unsigned int pinNo, enum PinDirection_t value01) /*set the pin direction*/
/*  All the PINS are, by default, configured as an input.
    Hence, we only need to set the direction if we want to configure it as an output.
    However, this function will do both
*/
{
    unsigned int bankNo = pinNo/32;
    pinNo%=32;
    pinNo=(1<<pinNo);
	printf("Direction : %d\n",pinNo);
	unsigned int gpio_reg;

    if(bankNo==0)
    {
        if(value01==OUTPUT)
        {
            gpio_reg = *gpio_oe_addr0;
            gpio_reg &= (~(pinNo));
            *gpio_oe_addr0 = gpio_reg;
        }
        else if (value01==INPUT)
        {
            gpio_reg = *gpio_oe_addr0;
            gpio_reg |= (pinNo);
            *gpio_oe_addr0 = gpio_reg;
        }
    }

    if(bankNo==1)
    {
        if(value01==OUTPUT)
        {
            gpio_reg = *gpio_oe_addr1;
            gpio_reg &= (~(pinNo));
            *gpio_oe_addr1 = gpio_reg;
        }
        else if (value01==INPUT)
        {
            gpio_reg = *gpio_oe_addr1;
            gpio_reg |= (pinNo);
            *gpio_oe_addr1 = gpio_reg;
        }
    }

    if(bankNo==2)
    {
        if(value01==OUTPUT)
        {
            gpio_reg = *gpio_oe_addr2;
            gpio_reg &= (~(pinNo));
            *gpio_oe_addr2 = gpio_reg;
        }
        else if (value01==INPUT)
        {
            gpio_reg = *gpio_oe_addr2;
            gpio_reg |= (pinNo);
            *gpio_oe_addr2 = gpio_reg;
        }
    }

    if(bankNo==3)
    {
        if(value01==OUTPUT)
        {
            gpio_reg = *gpio_oe_addr3;
            gpio_reg &= (~(pinNo));
            *gpio_oe_addr3 = gpio_reg;
        }
        else if (value01==INPUT)
        {
            gpio_reg = *gpio_oe_addr3;
            gpio_reg |= (pinNo);
            *gpio_oe_addr3 = gpio_reg;
        }
    }


    return (0);
}

void gpioWritePin(unsigned int pinNo, enum PinValue_t value02)          /*set the pin status*/
{
    unsigned int bankNo = pinNo/32;
    pinNo%=32;
    pinNo=(1<<pinNo);

    if(bankNo==0)
    {
        if (value02==HIGH)
        {
            *gpio_setdataout_addr0= (pinNo);
            //printf("Writing HIGH\n");
        }
        else
        {
            *gpio_cleardataout_addr0 = (pinNo);
            //printf("Writing LOW;\n");
        }
    }

    if(bankNo==1)
    {
        if (value02==HIGH)
        {
            *gpio_setdataout_addr1= (pinNo);
            //printf("Writing HIGH\n");
        }
        else
        {
            *gpio_cleardataout_addr1 = (pinNo);
            //printf("Writing LOW;\n");
        }
    }

    if(bankNo==2)
    {
        if (value02==HIGH)
        {
            *gpio_setdataout_addr2= (pinNo);
            //printf("Writing HIGH\n");
        }
        else
        {
            *gpio_cleardataout_addr2 = (pinNo);
            //printf("Writing LOW;\n");
        }
    }

    if(bankNo==3)
    {
        if (value02==HIGH)
        {
            *gpio_setdataout_addr3= (pinNo);
            //printf("Writing HIGH\n");
        }
        else
        {
            *gpio_cleardataout_addr3 = (pinNo);
            //printf("Writing LOW;\n");
        }
    }

}


enum PinValue_t gpioReadPin(unsigned int pinNo)          /*read the pin status*/
{
    unsigned int bankNo = pinNo/32;
    pinNo%=32;
    pinNo=(1<<pinNo);
   
    
    if(bankNo==0)
    {
        pinNo&=*gpio_datain_addr0;
    }
    else if(bankNo==1)
    {
        pinNo&=*gpio_datain_addr1;
    }
    else if(bankNo==2)
    {
        pinNo&=*gpio_datain_addr2;
    }
    else if(bankNo==3)
    {
        pinNo&=*gpio_datain_addr3;
    }

    if (pinNo)
        return(LOW);
    else
        return(HIGH);
}



void cleanup()
{


    if (munmap((void*)((int)gpio_addr0),GPIO_SIZE)!=0)
{
    close(fd);
    perror("Error unmapping the file - GPIO0");
    exit(1);
}
	if (munmap((void*)((int)gpio_addr1),GPIO_SIZE)!=0)
    {
        close(fd);
        perror("Error unmapping the file - GPIO1");
        exit(1);
    }
	if (munmap((void*)((int)gpio_addr2),GPIO_SIZE)!=0)
    {
        close(fd);
        perror("Error unmapping the file - GPIO2");
        exit(1);
    }
	if (munmap((void*)((int)gpio_addr3),GPIO_SIZE)!=0)
    {
        close(fd);
        perror("Error unmapping the file - GPIO3");
        exit(1);
    }

	close(fd);
}

#endif

