.PHONY:  setup compile run

CC = g++
SRC = src/*.cxx
INCLUDE = include/
OF = diy-compiler


default: setup compile run

setup:
	@mkdir -p build/bin

compile: $(SRC)
	$(CC) -I$(INCLUDE) -o build/bin/$(OF) $(SRC)

run:
	@echo running
	@build/bin/$(OF)
	@printf "\nreturn ${$status}"

