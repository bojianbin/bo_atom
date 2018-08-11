TARGET = ae_main
BUILD_DIR = ../build
OBJ = $(patsubst %.c,%.o,$(wildcard ./*.c))
CFLAGS = -I$(BUILD_DIR)
LDFLAGS = -L$(BUILD_DIR) -lae -llog -lmd5 -lnetool -ldict

%.o:%.c
	@gcc -o $@ -c $<  $(CFLAGS)
all:$(OBJ)
	@gcc -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)
	@make -C test_client all
	@echo Compile done ,Enjoy~~~
clean:
	@rm -f *.o $(TARGET)
	@make -C test_client clean

