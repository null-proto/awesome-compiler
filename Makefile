.PHONY:  setup compile run

CC = gcc
FILE = memexec


default: setup compile run

setup:
	@mkdir -p build/bin

compile: src/
	$(CC) -o build/bin/$(FILE) src/$(FILE).c

run:
	@echo running
	@build/bin/$(FILE)
	@printf "\nreturn $?"

