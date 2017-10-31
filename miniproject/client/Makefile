# program executable name
TARGET = /export/nfs/root/G15/main

# compiler flags
CFLAGS = -g -Wall

# linker flags
LDFLAGS = -g -lrt -pthread -lm

# list of sources
SOURCES = $(shell find -name "*.c")

# default rule, to compile everything
all: $(TARGET)

# define object files
OBJECTS = $(SOURCES:.c=.o)

# link programs
$(TARGET): $(OBJECTS)
	/home/student/Documents/G15/Ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc $^ -o $@ $(LDFLAGS)

# compile
%.o : %.c
	/home/student/Documents/G15/Ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-gcc $(CFLAGS) -c -o $@ $<

# cleaning

clean:
	rm -f $(TARGET) $(OBJECTS)
