CFLAGS += -Wall -Wextra

.phony: all clean
all: png2chr chr2png sametiles tilecoords nes2hex pngreorder png2gba \
	palstat coord2metasprite nesasmc nsf2data text2data \
	coord2lz4

png2chr: png2chr.o common.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -lpng

chr2png: chr2png.o common.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -lpng

clean:
	rm -f *.o png2chr chr2png
