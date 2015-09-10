TARGET = build_in.o
#COMOBJS = ../common/proto.o   #公共模块
CC = armv5l-linux-gcc
LD = armv5l-linux-ld
#CC = gcc
#LD = ld
CCFLAGS += -c 

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))  #子目录obj

$(TARGET) : $(OBJS) $(COMOBJS)
	$(LD) -r $^ -o $@ $(LIBS) $(LDFLAGS) 
%.o : %.c
	$(CC) $< -fPIC -o  $@ $(CCFLAGS) $(INCLUDES)
.PHONY:clean
clean:
	rm -rf *.o
	rm -rf $(TARGET) 
	rm -rf Makefile

