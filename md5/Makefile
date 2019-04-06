TARGET = libmd5.a
INSTALL_DIR = ../build
OBJ = $(patsubst %.c,%.o,$(wildcard ./*.c))

%.o:%.c
	@gcc -o $@ -c $<   $(DEFS)
all:$(OBJ)
	@ar rcs $(TARGET) $(OBJ)
	install -d -m0755 $(INSTALL_DIR)
	cp -fpR $(TARGET) $(INSTALL_DIR)
	cp -fpR md32_common.h $(INSTALL_DIR)
	cp -fpR md5_locl.h $(INSTALL_DIR)
	cp -fpR md5.h $(INSTALL_DIR)
clean:
	@rm -f *.o $(TARGET)
	@rm -f $(INSTALL_DIR)/$(TARGET) $(INSTALL_DIR)/md32_common.h $(INSTALL_DIR)/md5_locl.h $(INSTALL_DIR)/md5.h
