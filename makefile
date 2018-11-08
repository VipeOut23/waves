CC = gcc
CFLAGS = -Wall -o waves
SRC = waves.c

waves: sources $(SRC)
	$(CC) $(CFLAGS) $(SRC) sources.c
tags:
	etags -a `(ls *.[ch])`
sources:
	@tar -cvzf src.tgz $(SRC) makefile sources.h
	@echo -en 'unsigned char __SOURCETAR_BYTES[] = {\n' > sources.c
	@cat src.tgz | xxd -i >> sources.c
	@echo -e '};\nunsigned int SOURCETAR_LEN = '`cat src.tgz | wc -c `';\nunsigned char *SOURCETAR_BYTES = __SOURCETAR_BYTES;' >> sources.c
	@rm src.tgz
clean:
	rm -f *.o
	rm sources.c
