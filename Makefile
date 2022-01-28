all: Erase Compilar Clear Run

Compilar: main.c
	gcc main.c -o Compilador

Erase: Compilador
	rm Compilador T_H1_H2 T_H2_H1

Clear:
	clear

Run: Compilador
	./Compilador