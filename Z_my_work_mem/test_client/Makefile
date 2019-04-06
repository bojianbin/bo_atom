
TARGET = ae_client
BUILD_DIR = ../../build
CFLAGS = -I$(BUILD_DIR)
LDFLAGS = -L$(BUILD_DIR) -lae -llog -lmd5 -lnetool -ldict -lm


OBJ = $(patsubst %.c,%.o,$(wildcard ./*.c))

%.o:%.c
	@gcc -o $@ -c $< $(CFLAGS) 

all:$(OBJ)
	@gcc -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS) -pthread
	@cp -f $(TARGET) ../

clean:
	@rm -f *.o $(TARGET) ../$(TARGET)
