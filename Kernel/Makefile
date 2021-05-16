include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_MEMORY_MANAGER=$(wildcard memory_manager/*.c)
SOURCES_TASK_MANAGER=$(wildcard task_manager/*.c)
SOURCES_SYNC=$(wildcard sync/*.c)
SOURCES_IPC=$(wildcard inter_process_comun/*.c)



OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_MEMORY_MANAGER=$(SOURCES_MEMORY_MANAGER:.c=.o)
OBJECTS_TASK_MANAGER=$(SOURCES_TASK_MANAGER:.c=.o)
OBJECTS_SYNC=$(SOURCES_SYNC:.c=.o)
OBJECTS_IPC=$(SOURCES_IPC:.c=.o)


LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=
#NO SSE EN MAKEFILE.INC DESACTIVA PUTNOS FLOTANTES
all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_IDT) $(OBJECTS_DRIVERS) $(OBJECTS_LIBRARIES) $(OBJECTS_MEMORY_MANAGER) $(OBJECTS_TASK_MANAGER) $(OBJECTS_SYNC) $(OBJECTS_IPC)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin libraries/*.o IDT/*.o drivers/*.o memory_manager/*.o task_manager/*.o sync/*.o

.PHONY: all clean
