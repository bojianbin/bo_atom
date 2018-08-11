TARGET = ae_main
BUILD_DIR = ../build
FILTER_OUT = ./test_client.o
OBJ_s = $(patsubst %.c,%.o,$(wildcard ./*.c))
CFLAGS = -I$(BUILD_DIR)
LDFLAGS = -L$(BUILD_DIR) -lae -llog -lmd5 -lnetool -ldict -lpthread

OBJ=$(filter-out $(FILTER_OUT),$(OBJ_s))


%.o:%.c
	@gcc -o $@ -c $<  $(CFLAGS)
all:$(OBJ)
	@gcc -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)
	@echo Compile done ,Enjoy~~~
clean:
	@rm -f *.o $(TARGET)
