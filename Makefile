SHELL=/bin/bash

COMPILER=gcc
PROJECTNAME=list_todos
SRCPATH=./src
OBJPATH=./obj
EXEPATH=.
EXT=c
EXE=.out
EXECUTABLE=$(PROJECTNAME)$(EXE)

all: $(EXEPATH)/$(EXECUTABLE)

$(OBJPATH):
	mkdir -p $@

$(EXEPATH)/$(EXECUTABLE): $(OBJPATH)/list_todos.o
	$(COMPILER) $^ -o $@

$(OBJPATH)/list_todos.o: $(SRCPATH)/list_todos.c $(SRCPATH)/parser.h | $(OBJPATH)
	$(COMPILER) -c $(SRCPATH)/list_todos.c -o $@

clean:
	rm $(EXEPATH)/$(EXECUTABLE) $(OBJPATH)/*
	rmdir $(OBJPATH)
