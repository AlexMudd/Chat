CC=gcc
CFLAGS=-Wall

BIN=client.exe server.exe

all: clean $(BIN)

%.exe:%.c
	@echo "Build $@"
	@$(CC) $(CFLAGS) -o $@ $<
	
clean:
	@rm -fv *.exe
