include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin

SOURCES= $(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_APPLICATIONS=$(wildcard applications/*.c applications/shell/*.c applications/calculator/*.c)
SOURCES_TEST_PROC=$(wildcard test_process/*.c)
SOURCES_TEST_MM=$(wildcard test_memory/*.c)
SOURCES_TEST_SYNC=$(wildcard test_sync/*.c)



OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_APPLICATIONS=$(SOURCES_APPLICATIONS:.c=.o)
OBJECTS_TEST_PROC=$(SOURCES_TEST_PROC:.c=.o)
OBJECTS_TEST_MM=$(SOURCES_TEST_MM:.c=.o)
OBJECTS_TEST_SYNC=$(SOURCES_TEST_SYNC:.c=.o)



all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_LIBRARIES) $(OBJECTS_APPLICATIONS)  $(OBJECTS_TEST_PROC) $(OBJECTS_TEST_MM) $(OBJECTS_TEST_SYNC)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o applications/*.o applications/calculator/*.o applications/shell/*.o asm/*.o *.bin libraries/*.o test_process/*.o

.PHONY: all clean print
