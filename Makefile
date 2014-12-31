SHELL=/bin/bash

COMPILER=gcc
PROJECTNAME=list_todos
SRCPATH=./src
OBJPATH=./obj
EXEPATH=.
EXT=c
EXE=out

all: $(EXEPATH)/$(PROJECTNAME).$(EXE)

$(OBJPATH):
	mkdir -p $@

$(EXEPATH)/$(PROJECTNAME).$(EXE): $(OBJPATH)/list_todos.o
	$(COMPILER) $^ -o $@

$(OBJPATH)/list_todos.o: $(SRCPATH)/list_todos.c | $(OBJPATH)
	$(COMPILER) -c $^ -o $@

$(OBJPATH)/parser.o: $(SRCPATH)/parser.c | $(OBJPATH)
	$(COMPILER) -c $^ -o $@

clean:
	rm $(EXEPATH)/$(PROJECTNAME).$(EXE) $(OBJPATH)/*
	rmdir $(OBJPATH)
