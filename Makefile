CFLAGS = -std=c99 -O3 -Wall -Wshadow -Wvla -pedantic -g
GCC = gcc
OBJS_COMPRESS = main.c compress.c tree.c list.c 
OBJS_DECOMPRESS = main.c decompress.c tree.c list.c 
DECOMPRESS_F = -DDECOMPRESS -o $(DECOMPRESS_EXEC)
COMPRESS_F = -DCOMPRESS -o $(COMPRESS_EXEC)
COMPRESS_EXEC = compress
DECOMPRESS_EXEC = decompress

$(DECOMPRESS_EXEC):
	$(GCC) $(CFLAGS) $(OBJS_DECOMPRESS) $(DECOMPRESS_F)

$(COMPRESS_EXEC):
	$(GCC) $(CFLAGS) $(OBJS_COMPRESS) $(COMPRESS_F)

clean:
	/bin/rm -f compress
	/bin/rm -f decompress
	/bin/rm -f *.cmp
	/bin/rm -f *.dcmp
