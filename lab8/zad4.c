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

// Funkcja do przełączania flagi OLCUC
void przełączFlageOLCUC(struct termios terminal) {
	int atrybut = terminal.c_oflag & OLCUC;

	if(atrybut) {
		//ustawienie (w strukturze) flagi OLCUC na wyłączoną
		terminal.c_oflag &= ~OLCUC;
		ustawAtrybutyTerminala(terminal);
		printf("Wyłączono duże litery\n");
	} else {
		//ustawienie (w strukturze) flagi OLCUC na włączoną
		terminal.c_oflag |= OLCUC;
		ustawAtrybutyTerminala(terminal);
		printf("Włączono DUŻE LITERY\n");
	}
}

int main() {
	// Utworzenie struktury do przechowywania atrybutów terminala
	struct termios terminal = wczytajAtrybutyTerminala();
	
	przełączFlageOLCUC(terminal);
	
	return 0;
}
