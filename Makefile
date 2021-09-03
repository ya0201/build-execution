#==========================================
#    Makefile: makefile for sl 5.1
#	Copyright 1993, 1998, 2014
#                 Toyoda Masashi
#		  (mtoyoda@acm.org)
#	Last Modified: 2014/03/31
#
#	Modified by ya0201: 2019/12/22~
#==========================================

CC=g++
CFLAGS=-O -Wall

.PHONY: all
all: be install

se.o: data/se.wav
	gobjcopy -S -I binary -O mach-o-x86-64 data/se.wav se.o

be.o: be.cpp be.h
	# $(CC) $(CFLAGS) -o be be.cpp -lncurses -framework OpenAL
	$(CC) $(CFLAGS) -c be.cpp

be: be.o se.o
	$(CC) $(CFLAGS) -o be be.o se.o -lncurses -framework OpenAL

.PHONY: install
install:
	mkdir -p ${HOME}/.local/bin
	mv be ${HOME}/.local/bin/be
	echo 'Installed to ${HOME}/.local/bin'

.PHONY: clean
clean:
	rm -f be ${HOME}/.local/bin/be

distclean: clean
