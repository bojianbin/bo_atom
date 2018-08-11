TARGET = libdict.a
INSTALL_DIR = ../build

OBJ = $(patsubst %.c,%.o,$(wildcard ./*.c))

%.o:%.c
	@gcc -o $@ -c $<   $(DEFS)
all:$(OBJ)
	@ar rcs $(TARGET) $(OBJ)
	install -d -m0755 $(INSTALL_DIR)
	cp -fpR $(TARGET) $(INSTALL_DIR)
	cp -fpR dict.h $(INSTALL_DIR)
clean:
	@rm -f *.o $(TARGET)
	@rm -f $(INSTALL_DIR)/$(TARGET) $(INSTALL_DIR)/dict.h
