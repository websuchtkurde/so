#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// Funkcja do wczytywania atrybutów terminala
struct termios wczytajAtrybutyTerminala() {
	struct termios terminal;
	tcgetattr(STDIN_FILENO, &terminal);
	return terminal;
}

// Funkcja do ustawiania atrybutów terminala
void ustawAtrybutyTerminala(struct termios terminal) {
	tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

// Funkcja do przełączania flagi ECHO
void przełączFlageEcho(struct termios terminal) {
	int atrybut = terminal.c_lflag & ECHO;

	if(atrybut) {
		//ustawienie flagi ECHO na wyłączoną
		terminal.c_lflag &= ~ECHO;
		ustawAtrybutyTerminala(terminal);
		printf("Wyłączono echo\n");
	} else {
		//ustawienie flagi ECHO na włączoną
		terminal.c_lflag |= ECHO;
		ustawAtrybutyTerminala(terminal);
		printf("Włączono echo\n");
	}
}

int main() {
	// Utworzenie struktury do przechowywania atrybutów terminala
	struct termios terminal = wczytajAtrybutyTerminala();
	
	przełączFlageEcho(terminal);
	
	return 0;
}

