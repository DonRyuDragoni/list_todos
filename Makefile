SHELL=/bin/bash

COMPILER=gcc
FILENAME=list_todos
EXT=c
OUT=out

all: $(FILENAME).$(OUT)

$(FILENAME).$(OUT): $(FILENAME).$(EXT)
	$(COMPILER) $^ -o $@

run: $(FILENAME).$(OUT)
	./$^

clean:
	rm $(FILENAME).$(OUT)
