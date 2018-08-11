TARGET = libae.a
INSTALL_DIR = ../build

DEFS = -DHAVE_EPOLL   	#HAVE_SELECT  HAVE_EPOLL HAVE_KQUEUE HAVE_EVPORT
OBJ = ae.o

%.o:%.c
	@gcc -o $@ -c $<   $(DEFS)
all:$(OBJ)
	@ar rcs $(TARGET) $(OBJ)
	install -d -m0755 $(INSTALL_DIR)
	cp -fpR $(TARGET) $(INSTALL_DIR)
	cp -fpR ae.h $(INSTALL_DIR)
clean:
	@rm -f *.o $(TARGET)
	@rm -f $(INSTALL_DIR)/$(TARGET) $(INSTALL_DIR)/ae.h
