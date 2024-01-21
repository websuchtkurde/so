#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// Funkcja do wczytywania atrybutów terminala
struct termios wczytajAtrybutyTerminala() {
	struct termios terminal;
	tcgetattr(0, &terminal);
	return terminal;
}

// Funkcja do ustawiania atrybutów terminala
void ustawAtrybutyTerminala(struct termios terminal) {
	tcsetattr(0, TCSANOW, &terminal);
}

// Funkcja do wyświetlania menu
void wyswietlMenu() {
	printf("Wybierz opcję:\n");
	printf("1 - opcja pierwsza\n");
	printf("2 - opcja druga\n");
	printf("3 - opcja trzecia\n");
	printf("0 - wyjscie z programu\n");
}

// Funkcja do obsługi wyboru użytkownika
void obsluzWybor(int znak) {
	if(znak == '1') {
		printf("\nWybrano opcję pierwszą\n");
	} else if(znak == '2') {
		printf("\nWybrano opcję drugą\n");
	} else if(znak == '3') {
		printf("\nWybrano opcję trzecią\n");
	} else if(znak == '0') {
		printf("\nZakończono działanie programu\n");
	} else {
		printf("\nPodano nieprawidłowy znak\n");
	}
}

// Funkcja zwracająca kod wciśniętego przycisku
int pobierzZnak() {
	char klawisz;
	read(0, &klawisz, 1);
	return klawisz;
}

int main () {
	// Utworzenie struktur przechowujących atrybuty terminala
	struct termios terminal_backup = wczytajAtrybutyTerminala();
	struct termios terminal_nowy = terminal_backup;
	
	// Ustawienie odpowiednich flag w atrybutach struktury
	terminal_nowy.c_lflag &= ~(ICANON | ECHO | ECHONL);
	terminal_nowy.c_cc[VTIME] = 0;
	terminal_nowy.c_cc[VMIN] = 1;

	// Zastosowanie struktury atrybutów na aktywnym terminalu
	ustawAtrybutyTerminala(terminal_nowy);
	
	wyswietlMenu();
	
	int znak = 0;
	while(znak != '0') {
		znak = pobierzZnak();
		obsluzWybor(znak);
	}
	
	// Przywrócenie domyślnego trybu działania terminala
	ustawAtrybutyTerminala(terminal_backup);
	
	return 0;
}
