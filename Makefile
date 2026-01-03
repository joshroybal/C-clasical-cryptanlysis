CFLAGS = -ansi -pedantic-errors -Wall -Wextra -Werror -g -Og
LFLAGS =
BIN = vigenere_encrypt vigenere_attack simplesub ss_attack

all: $(BIN)

simplesub: simplesub.o crypt.o
	$(CC) -o $@ $^ $(LFLAGS)

ss_attack: ss_attack.o stats.o crypt.o
	$(CC) -o $@ $^ $(LFLAGS)

vigenere_encrypt: vigenere_encrypt.o crypt.o
	$(CC) -o $@ $^ $(LFLAGS)

vigenere_attack: vigenere_attack.o crypt.o stats.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c %.h
	$(CC) -c $< $(CFLAGS)

.PHONY : clean
clean:
	$(RM) *.o $(BIN) *~
