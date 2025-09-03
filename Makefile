.PHONY:  setup compile run

CC = gcc
SRC = src/*
INCLUDE = include/
OF = diy-compiler


default: setup compile run

setup:
	@mkdir -p build/bin

compile: src/
	$(CC) -I$(INCLUDE) -o build/bin/$(OF) src/*.c

run:
	@echo running
	@build/bin/$(OF)
	@printf "\nreturn ${$status}"

