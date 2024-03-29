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
CFLAGS=-O -Wall -Wno-c++11-compat-deprecated-writable-strings -Wno-deprecated-declarations

.PHONY: all
all: build

.PHONY: sounds
sounds:
	./scripts/xxd-sounds.zsh

.PHONY: build
build: be.cpp be.h sounds
	$(CC) $(CFLAGS) -o be be.cpp -lncurses -framework OpenAL

.PHONY: install
install: build
	mkdir -p ${HOME}/.local/bin
	mv be ${HOME}/.local/bin/be
	@echo 'Installed to ${HOME}/.local/bin'

.PHONY: clean
clean:
	rm -f be ${HOME}/.local/bin/be

distclean: clean
