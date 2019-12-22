#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#
#	Modified by ya0201: 2019/12/22
#==========================================

CC=gcc
CFLAGS=-O -Wall

all: be

be: be.c be.h
	$(CC) $(CFLAGS) -o be be.c -lncurses

clean:
	rm -f be

distclean: clean
