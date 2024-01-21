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

// Funkcja do wyświetlania informacji o terminalu
void wyswietlInformacjeOTerminalu(struct termios terminal) {
	printf("Nazwa pliku terminala: %s\n", ttyname(STDIN_FILENO));
	printf("Flaga trybu wejścia: %d\n", terminal.c_iflag);
	printf("Flaga trybu wyjścia: %d\n", terminal.c_oflag);
	printf("Flaga trybu sterowania: %d\n", terminal.c_cflag);
	printf("Flaga trybu lokalnego: %d\n", terminal.c_lflag);
}

int main() {
	// Utworzenie struktury do przechowywania atrybutów terminala
	struct termios terminal = wczytajAtrybutyTerminala();
	
	if(isatty(STDIN_FILENO)) {
		wyswietlInformacjeOTerminalu(terminal);
	}
	
	return 0;
}

