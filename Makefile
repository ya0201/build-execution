#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#
#	Modified by ya0201: 2019/12/22
#==========================================

CC=g++
CFLAGS=-O -Wall

all: be

be: be.cpp be.h
	$(CC) $(CFLAGS) -o be be.cpp -lncurses -framework OpenAL

clean:
	rm -f be

distclean: clean
