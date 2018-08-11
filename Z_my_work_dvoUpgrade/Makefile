TARGET =dvoUpgrade
BUILD_DIR = ../build
FILTER_OUT = ./dvo_ipc_update.o
OBJ_s = $(patsubst %.c,%.o,$(wildcard ./*.c))
CFLAGS = -I$(BUILD_DIR)
LDFLAGS = -L$(BUILD_DIR) -lae -llog -lmd5 -lnetool -ldict -lpthread -lm

OBJ=$(filter-out $(FILTER_OUT),$(OBJ_s))


%.o:%.c
	@gcc -o $@ -c $<  $(CFLAGS)
all:$(OBJ)
	@gcc -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)
	@echo Compile done ,Enjoy~~~
clean:
	@rm -f *.o $(TARGET)
