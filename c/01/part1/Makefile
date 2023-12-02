CC:=gcc
LDFLAGS:=
CFLAGS:=-g
BINARY:=sum.out

OBJ_FILES:=dynstring.o main.o

all: $(BINARY)

$(BINARY): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_FILES): %.o: %.c

clean: 
	rm $(BINARY) $(OBJ_FILES)

.PHONY: all clean
