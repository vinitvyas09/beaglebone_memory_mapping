#############################################################################
# Config BBB uploads
#############################################################################
BBB_IP 		:= 192.168.7.2
BBB_USER	:= root
BBB_PASS	:= '\'
BBB_PATH	:= /foldername

#############################################################################
# Make config
#############################################################################            
LDFLAGS 		:= -lrt -lm -lpthread -lv4l2
CFLAGS 			:= -O3 -g3 -Wall -c -pthread -march=armv7-a -mtune=cortex-a8 -mfpu=neon -ftree-vectorize -ffast-math -mfloat-abi=softfp
CC 				:= gcc  
BUILD_DIR 		:= Debug/
BUILD_OBJ := $(BUILD_DIR)obj/

# adjust the build path for either 'bbb' or 'upload' targets
ifeq (bbb, $(findstring bbb,$(MAKECMDGOALS)))
CC 			:= arm-linux-gnueabi-gcc-4.7
#use arm-linux-gnueabi-gcc-4.7 for Angstrom
#use arm-linux-gnueabihf-gcc-4.7 for Debian
BUILD_DIR 	:= Beaglebone/
BUILD_OBJ	:= $(BUILD_DIR)obj/
endif
ifeq ($(MAKECMDGOALS),upload)
CC 			:= arm-linux-gnueabi-gcc-4.7
#use arm-linux-gnueabi-gcc-4.7 for Angstrom
#use arm-linux-gnueabihf-gcc-4.7 for Debian
BUILD_DIR 	:= Beaglebone/
BUILD_OBJ	:= $(BUILD_DIR)obj/
endif

#############################################################################
# Add folders with include files to this PATH
#############################################################################            
INCLUDE_PATH 	:= -I"(*folder1 path*)" -I"(*folder2 path*)" 

#############################################################################
# Add .c files that have a main() function here
#############################################################################            	
BINARIES +=	src/test/testStepper.c
#############################################################################
# Add all other .c files here
#############################################################################            

SOURCES +=	*folder1 path*/gpio.c
			
#############################################################################
# Add .h files here
#############################################################################            

HEADERS +=	*folder2 path*/gpio.h

#############################################################################
# TARGETS
#############################################################################            
OBJECTS := $(SOURCES:.c=.o)
PROGRAMS := $(OBJECTS) $(BINARIES:.c=.o)
BINARIES := $(BINARIES:.c=)

.PHONY: clean upload mkd all bbb

all: mkd $(SOURCES) $(BINARIES)

bbb: mkd $(SOURCES) $(BINARIES)

#create output build dir if not exists
mkd:
	@mkdir -p $(BUILD_OBJ)	
	
	
#linker	
$(BINARIES): $(PROGRAMS)
	@echo ' '
	$(CC) $(addprefix $(BUILD_OBJ), $(notdir $(OBJECTS))) \
	      $(BUILD_OBJ)$(notdir $(basename $@)).o $(LDFLAGS) \
	      -o $(BUILD_DIR)$(subst main,$(notdir $(basename $@)))

#compiler
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $< -o $(BUILD_OBJ)$(notdir $@)

#clean 
clean:
	rm -r $(BUILD_DIR)*

#BBB upload
upload: bbb    
	@echo 'Uploading...' 
	@echo "put -p Beaglebone/*" | sshpass -p $(BBB_PASS)  sftp $(BBB_USER)@$(BBB_IP):$(BBB_PATH) 
	@echo 'Uploading done.' 
 
