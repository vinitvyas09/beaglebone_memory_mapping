
/**********************************************************

This is a test program that tests the I/O library.
When the Receiver detects that the LASER has been intercepted,
it changes the direction (orientation) of the Stepper Motor 

***********************************************************/


#include"gpio.c" 
#include"gpio.h"
			

int main(int argc, char *argv[])
{
	int i=0,t=1;   
 	static unsigned int k = 0, n = 0, step = 0, dir = 0;
    int toggle = 0;
    static unsigned long span = 0;
	unsigned int DIR = 45, STEP=44, LASER=27;
	if (gpioInit()!=0)
    {
        perror("GPIO Initialization Error");
        exit(1);
    }


    if(gpioSetPinDirection(DIR,OUTPUT)!=0)
    {
        perror("Error setting the PIN Direction");
        exit(1);
    }


    if(gpioSetPinDirection(STEP,OUTPUT)!=0)
    {
        perror("Error setting the PIN Direction");
        exit(1);
    }

    if(gpioSetPinDirection(LASER,INPUT)!=0)
    {
        perror("Error setting the PIN Direction");
        exit(1);
    }

    while(1)
	{
		gpioWritePin( STEP, LOW );
		usleep(40);
		gpioWritePin( STEP, HIGH );
		usleep(40);
		if(t==1)
		{
			if (gpioReadPin(LASER))
			{
				printf("ON\n");
				gpioWritePin( DIR, toggle % 2 == 0 ? HIGH : LOW );
				toggle++;
				usleep(200);
				if(toggle>1001) toggle=0;
				t=500;
			}
			else
			{
				printf("OFF\n");
				t=500;
			}
		}
		t--;
		if(t<1) t=1;
	}
	cleanup();
    return 0;
}
