CROSS=arm-linux-

target: all

all:server.c
	$(CROSS)gcc -o server server.c
	$(CROSS)strip server
clean:
	@rm -vf server *.o *~
