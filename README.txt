This code library enables a user to implement memory mapping on the ARM Processor.
It is tested on the Beaglebone Black but can be used on other such boards with an ARM Processor.

The files included are:
gpio.h        : A header file containing all the necessary MACROS used & function declarations
gpio.c 	      : This is the most important file that contains the function where mmap() is implemented.
Makefile      : A sample Makefile
patch-tree.sh : This shell script enables the Device Tree overlay (can be used if required)
testStepper.c : A sample code where we test the mmap() by controlling a stepper motor connected at the PORTS specified in the code