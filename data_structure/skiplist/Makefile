TARGET = libskiplist.a
TARGET_test = skiplist
INSTALL_DIR = ../../build

FILTER_OUT_test = 
FILTER_OUT = main.o
OBJ_s = $(patsubst %.c,%.o,$(wildcard ./*.c))
CFLAGS = 
LDFLAGS = 

OBJ_test=$(filter-out $(FILTER_OUT_test),$(OBJ_s))
OBJ=$(filter-out $(FILTER_OUT),$(OBJ_s))


%.o:%.c
	@gcc -o $@ -c $<  $(CFLAGS)
all:$(OBJ)
	@ar rcs $(TARGET) $(OBJ)
	install -d -m0755 $(INSTALL_DIR)
	cp -fpR $(TARGET) $(INSTALL_DIR)
	cp -fpR skiplist.h $(INSTALL_DIR)

test:$(OBJ_test)
	@gcc -o $(TARGET_test) $(OBJ_test) $(CFLAGS) $(LDFLAGS)
	@echo Compile done ,Enjoy~~~

clean:
	@rm -f *.o $(TARGET) $(TARGET_test)
	@rm -f $(INSTALL_DIR)/$(TARGET) $(INSTALL_DIR)/skiplist.h
