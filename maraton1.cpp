#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define NUMPREGUNTAS 63

/*	Estructura para almacenar las preguntas
	Contiene la frase (preguntas y opciones) y la respuesta (A, B o C)
	y una pregunta siguiente
*/
typedef struct pregunta {
	char *frase;
	int respuesta;
	struct pregunta *siguiente;
} Pregunta;
Pregunta *primerElemento = NULL; // lista de preguntas

// Prototipos de funciones usadas
Pregunta *crearPregunta(char *, int);
void agregarPregunta(Pregunta **, char *, int);
void imprimirPregunta(char *);
void leerArchivo();
int buscaRepetido(int *, int, int);
void obtenerPregunta(Pregunta *, Pregunta *, int);
int defineGanador(int *, int);

/*
	main: Función principal, lleva el control del juego
*/
int main() {
	int nJug, jugadores[4], i, tiro, x = 0, c = 1, y, z = 0;
	int numeros[NUMPREGUNTAS], num;
	int res, ignorancia = 0, a;
	Pregunta aux;

	leerArchivo();

	printf("\nIngrese el numero de jugadores (maximo 4): ");
	scanf("%i", &nJug);

	for(i = 0; i < 4; ++i)
		jugadores[i] = 0;

	srand(time(NULL));

	printf("\nInicio del juego\n\n");
	while(jugadores[0] < 42 && jugadores[1] < 42 && jugadores[2] < 42 && jugadores[3] < 42 && ignorancia < 42) {
		for(i = 0; i < nJug; ++i) {
			printf("\nPresione ENTER para continuar\n\n");
			getchar(); getchar();
			printf("\n\n\nTurno Jugador %i:\n", i + 1);
			printf("Puntaje: %i\n\n\n", jugadores[i]);
			tiro = 1 + rand() % (6);
			printf("Valor del dado: %i\n", tiro);

			while(c == 1) {
				num = 1 + rand() % (27);
				c = buscaRepetido(numeros, num, z);
			}
			c = 1;
			numeros[z] = num;
			obtenerPregunta(primerElemento, &aux, num);
			imprimirPregunta(aux.frase);

			printf("\nRespuesta: ");
			scanf("%i", &res);

			if(res == aux.respuesta) {
				printf("\nCorrecto\n");
				jugadores[i] += tiro;
				x = 1;
			}
			else {
				printf("\nIncorrecto\n");
				for(y = 0; y < nJug - 1; y ++) {
					printf("\n\n\nOportunidad Jugador %i:\n\n", ((i + y + 1) % nJug) + 1);
					printf("\nRespuesta: ");
					scanf("%i", &res);
					if(res == aux.respuesta) {
						printf("\nCorrecto\n");
						jugadores[(i + y + 1) % nJug] += tiro;
						x = 1;
						break;
					} else
						printf("\nIncorrecto\n");
				}
			}

			if(x == 0) ignorancia += tiro;

			if (z < NUMPREGUNTAS)
				z++;
			else
				z = 0;
		}
	}

	printf("\n\n\nPuntaje Final:\n\n");
	for (i = 0; i < nJug; ++i)
		printf("\nJugador %i: %i", i + 1, jugadores[i]);
	printf("\nIgnorancia: %i\n", ignorancia);

	int ganador = defineGanador(jugadores, ignorancia);
	if (ganador == 5)
		printf("\n\nGANADOR: La ignorancia");
	else
		printf("\n\nGANADOR: Jugador %i\n", ganador);
	
	return 0;
}

int defineGanador(int *jugadores, int ign) {
	int i, max = 0, pos = 1;
	for(i = 0; i < 4; ++i) {
		if(max < jugadores[i]) {
			max = jugadores[i];
			pos = i;
		}
	}

	if (jugadores[pos] > ign)
		return (pos + 1);
	else
		return 5;
}

/*	crearPregunta: recibe frase y respuesta y crea un dato de tipo Pregunta
	Recibe: char frase[]
			char respuesta[]
	Regresa: Pregunta p
*/
Pregunta *crearPregunta(char *frase, int respuesta) {
	Pregunta *p;
	p = (Pregunta *) malloc(sizeof(Pregunta));
	p->frase = (char *) malloc(sizeof(char) * (strlen(frase) + 1));
	strcpy(p->frase, frase);
	(*p).respuesta = respuesta;
	p->siguiente = NULL;
	return p;
}

/*	agregarPregunta: agrega una pregunta a la lista
	Recibe: Pregunta **lista
			char frase[]
			char respuesta[]
*/
void agregarPregunta(Pregunta **lista, char *frase, int respuesta) {
	Pregunta *nuevo;
	nuevo = crearPregunta(frase, respuesta);
	nuevo->siguiente = *lista;
	(*lista) = nuevo;
}

/*	imprimirPregunta: imprime una Pregunta con un formato legible
	Recibe: Pregunta preg
*/
void imprimirPregunta(char *frase) {
	char *ptr;
	ptr = strtok(frase,"#");
	printf("%s\n", ptr);
	while((ptr = strtok(NULL,"#")) != NULL)
		printf("%s\n", ptr);
}

/*
	leerArchivo: lee un archivo con un formato definido y crea la lista de preguntas
*/
void leerArchivo() {
	FILE *arch;
	char caracteres[200];
	char *ptr, *a;
	int b;
	arch = fopen("preguntas.txt", "r");
	if(arch == NULL)
		exit(1);
	else {
		while(!feof(arch)) {
			fgets(caracteres,200,arch);
			ptr = strtok(caracteres,"|");
			a = ptr;
			while((ptr = strtok(NULL,"|")) != NULL)
				b = atoi(ptr);
			agregarPregunta(&primerElemento, a, b);
		}
	}
	fclose(arch);
}

/*	buscaRepetido: busca un numero en un array
	Recibe: int numeros[]
			int n
			int i
	Regresa: int (1 si se encontró, 0 en caso contrario)
*/
int buscaRepetido(int *numeros, int n, int x) {
	int i;
	for (i = 0; i < x; ++i)
		if(numeros[i] == n) {
			return 1;
		}
	return 0;
}

/*	obtenerPregunta: busca una pregunta en la posicion dada
	Recibe: Pregunta *lista
			Pregunta *dato (aqui se almacena lo obtenido)
			int n
*/
void obtenerPregunta(Pregunta *lista, Pregunta *dato, int n) {
	int i;
	Pregunta *actual;
	actual = lista;
	for(i = 0; i < (n-1); i++) 
		actual = actual->siguiente;
	(*dato).frase = (char *) malloc(sizeof(char) * (strlen((*actual).frase) + 1));
	strcpy((*dato).frase, (*actual).frase);
	(*dato).respuesta = (*actual).respuesta;
}

