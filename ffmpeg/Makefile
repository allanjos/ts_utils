#!/bin/sh

CC=gcc

CFLAGS := -I /usr/include/ffmpeg

LDFLAGS := -lavcodec -lavutil -lavfilter -lavformat -lavresample -lavdevice

decode_ts: decode_ts.o
	$(CC) $?  $(LDFLAGS) -o $@

decode_ts.o: decode_ts.c

clean:
	rm -f decode_ts
	rm *.o