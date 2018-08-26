TARGET = rbtree
FILTER_OUT = 
OBJ_s = $(patsubst %.c,%.o,$(wildcard ./*.c))
CFLAGS = 
LDFLAGS = 

OBJ=$(filter-out $(FILTER_OUT),$(OBJ_s))


%.o:%.c
	@gcc -o $@ -c $<  $(CFLAGS)
all:$(OBJ)
	@gcc -o $(TARGET) $(OBJ) $(CFLAGS) $(LDFLAGS)
	@echo Compile done ,Enjoy~~~
clean:
	@rm -f *.o $(TARGET)
