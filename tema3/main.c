//Tunsoiu Dan-Andrei 315CA 2022-2023
#define _GNU_SOURCE
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef struct {
	int tipimag;
	int nrculor;
	int n, m;
	int x1, x2, y1, y2;
	int ***matriceculor;
	//aceasta este structura imaginii, n si m sunt inaltime respectiv
	//lungimea imaginii
	//x1, x2, y1, y2 sunt folosite pentru select
} imagini;

int alocarematrice(int n, int m, int nrculor, int ****matrice)
{
	(*matrice) = malloc(n * sizeof(int **));
	if (!(*matrice)) {
		printf("EROARE\n");
		return -1;
	}
	for (int i = 0; i < n; i++) {
		(*matrice)[i] = malloc(m * sizeof(int *));
		if (!((*matrice)[i])) {
			printf("EROARE\n");
			for (int j = 0; j < i; j++)
				free((*matrice)[j]);
			return -1;
		}
		for (int j = 0; j < m; j++) {
			(*matrice)[i][j] = malloc(nrculor * sizeof(int));
			if (!((*matrice)[i][j])) {
				printf("EROARE\n");
				for (int k = 0; k < j; k++)
					free((*matrice)[i][j]);
				for (int j = 0; j < i; j++)
					free((*matrice)[j]);
				return -1;
			}
		}
	}
	return 0;
	//alocarea dinamica a matricilor cu programare defensiva
}

void dealocarematrice(int n, int m, int ****matrice)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++)
			free((*matrice)[i][j]);
		free((*matrice)[i]);
	}
	free((*matrice));
}

int aflarenumer(FILE *f, char *carac)
{
	int nr = 0;
	while (!(*carac >= 48 && *carac <= 57))
		fread(carac, sizeof(char), 1, f);
	while (*carac >= 48 && *carac <= 57) {
		nr = nr * 10 + ((*carac) - 48);
		fread(carac, sizeof(char), 1, f);
	}
	return nr;
	//functia transforma un sir de caractere din fisier care contine doar cifre
	//intr-un numar natural
}

void matricechar(imagini *imagine, FILE *f)
{
	char carac;
	int ****mat = &((*imagine).matriceculor), n, m;
	n = (*imagine).n;
	m = (*imagine).m;
	if (alocarematrice(n, m, (*imagine).nrculor, mat) == -1)
		return;
	for (int i = 0; i < (*imagine).n; i++)
		for (int j = 0; j < (*imagine).m; j++)
			for (int k = 0; k < (*imagine).nrculor; k++) {
				fread(&carac, sizeof(char), 1, f);
				((*imagine).matriceculor)[i][j][k] = carac;
				if (((*imagine).matriceculor)[i][j][k] < 0)
					((*imagine).matriceculor)[i][j][k] += 256;
	}
}

void matriceint(imagini *imagine, FILE *f)
{
	char carac;
	int ****mat = &((*imagine).matriceculor), n, m;
	n = (*imagine).n;
	m = (*imagine).m;
	if (alocarematrice(n, m, (*imagine).nrculor, mat) == -1)
		return;
	for (int i = 0; i < (*imagine).n; i++)
		for (int j = 0; j < (*imagine).m; j++)
			for (int k = 0; k < (*imagine).nrculor; k++) {
				fread(&carac, sizeof(char), 1, f);
				((*imagine).matriceculor)[i][j][k] = aflarenumer(f, &carac);
			}
}

void comentarii(FILE *f, char *c)
{
	while (*c != 10)
		fread(c, sizeof(char), 1, f);
	fread(c, sizeof(char), 1, f);
	if (*c == 35)
		comentarii(f, c);
	//citeste tot ce se afla pe o linie care are la incepu #
}

void load(imagini *imagine, char *copielin, int *existaimagin)
{
	char carac, *numefisier;
	FILE *f;
	numefisier = strtok(copielin, "\n ");
	numefisier = strtok(NULL, "\n ");
	f = fopen(numefisier, "rb");
	if (!f) {
		printf("Failed to load %s\n", numefisier);
		(*existaimagin) = 0;
		return;
	}
	fread(&carac, sizeof(char), 1, f);
	fread(&carac, sizeof(char), 1, f);
	(*imagine).tipimag = carac - 48;
	fread(&carac, sizeof(char), 1, f);
	(*imagine).n = (*imagine).m = 0;
	fread(&carac, sizeof(char), 1, f);
	if (carac == 35)
		comentarii(f, &carac);
	(*imagine).m = aflarenumer(f, &carac);
	fread(&carac, sizeof(char), 1, f);
	if (carac == 35)
		comentarii(f, &carac);
	(*imagine).n = aflarenumer(f, &carac);
	fread(&carac, sizeof(char), 1, f);
	if (carac == 35)
		comentarii(f, &carac);
	while (carac != 10)
		fread(&carac, sizeof(char), 1, f);
	fread(&carac, sizeof(char), 1, f);
	if (carac == 35)
		comentarii(f, &carac);
	fseek(f, -1, SEEK_CUR);
	if ((*imagine).tipimag % 3 == 0)
		(*imagine).nrculor = 3;
	else
		(*imagine).nrculor = 1;
	//In functie de tipul imaginii aflam daca aceasta este graysacle sau rgb
	if ((*imagine).tipimag < 4)
		matriceint(imagine, f);
	else
		matricechar(imagine, f);
	//In functie de tipul imaginii stim daca aceasta a fost compresata sau nu
	//Avem doua fnctii diferite pentru citirea matricei de pixeli in functie
	//de compresia imaginii
	(*imagine).x1 = (*imagine).y1 = 0;
	(*imagine).x2 = (*imagine).m;
	(*imagine).y2 = (*imagine).n;
	//Dupa citirea unei imaginii aceasta este selectata complet
	fclose(f);
	printf("Loaded %s\n", numefisier);
	(*existaimagin) = 1;
}

void swap(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

int nr(char *argument, int *nr)
{
	for (size_t i = 0; i < strlen(argument); i++) {
		if (argument[i] < 48 || argument[i] > 57)
			if (argument[i] != 45 && i == 0) {
				printf("Invalid command\n");
				return -1;
			}
		if (argument[i] != 45)
			(*nr) = (*nr) * 10 + (argument[i] - 48);
	}
	if (argument[0] == 45)
		*nr = (-1) * (*nr);
	//Functia primeste un sir de caractere iar daca acesta
	//contine doar cifre si -
	//atunci afla numarul corespunzator sirului de caractere,
	//altfel returneaza -1
	return 0;
}

void selectimag(imagini *imagine, char *copielin, int existaimagin)
{
	char *argument;
	int x1, x2, y1, y2;
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	if (!existaimagin) {
		printf("No image loaded\n");
		return;
	}
	argument = strtok(copielin, "\n ");
	argument = strtok(NULL, "\n ");
	if (!argument) {
		printf("Invalid command\n");
		return;
	}
	if (!strcmp(argument, "ALL")) {
		(*imagine).x1 = (*imagine).y1 = 0;
		(*imagine).x2 = (*imagine).m;
		(*imagine).y2 = (*imagine).n;
		printf("Selected ALL\n");
	} else {
		if (!argument) {
			printf("Invalid command\n");
			return;
		}
		if (nr(argument, &x1) == -1)
			return;
		if (x1 < 0 || x1 > (*imagine).m) {
			printf("Invalid set of coordinates\n");
			return;
		}
		argument = strtok(NULL, "\n ");
		if (!argument) {
			printf("Invalid command\n");
			return;
		}
		if (nr(argument, &y1) == -1)
			return;
		if (y1 < 0 || y1 > (*imagine).n) {
			printf("Invalid set of coordinates\n");
			return;
		}
		argument = strtok(NULL, "\n ");
		if (!argument) {
			printf("Invalid command\n");
			return;
		}
		if (nr(argument, &x2) == -1)
			return;
		if (x2 < 0 || x2 > (*imagine).m) {
			printf("Invalid set of coordinates\n");
			return;
		}
		argument = strtok(NULL, "\n ");
		if (!argument) {
			printf("Invalid command\n");
			return;
		}
		if (nr(argument, &y2) == -1)
			return;
		if (y2 < 0 || y2 > (*imagine).n || !(x1 - x2) || !(y1 - y2)) {
			printf("Invalid set of coordinates\n");
			return;
		}
		if (x2 < x1)
			swap(&x2, &x1);
		if (y2 < y1)
			swap(&y2, &y1);
		(*imagine).x1 = x1;
		(*imagine).x2 = x2;
		(*imagine).y1 = y1;
		(*imagine).y2 = y2;
		printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
	}
}

void histograma(imagini imagine, char *copielin, int existaimagin)
{
	char *argument;
	int x, y, interval, v[256] = {0}, max = 0, nrstel2;
	double nrstel;
	if (existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	argument = strtok(copielin, "\n ");
	argument = strtok(NULL, "\n ");
	if (!argument) {
		printf("Invalid command\n");
		return;
	}
	x = 0;
	for (size_t i = 0; i < strlen(argument); i++) {
		if (argument[i] < 48 || argument[i] > 57) {
			printf("Invalid command\n");
			return;
		}
		x = x * 10 + (argument[i] - 48);
	}
	argument = strtok(NULL, "\n ");
	if (!argument) {
		printf("Invalid command\n");
		return;
	}
	y = 0;
	for (size_t i = 0; i < strlen(argument); i++) {
		if (argument[i] < 48 || argument[i] > 57) {
			printf("Invalid command\n");
			return;
		}
		y = y * 10 + (argument[i] - 48);
	}
	if (256 % y != 0) {
		printf("Invalid set of parameters\n");
		return;
	}
	argument = strtok(NULL, "\n ");
	if (argument) {
		printf("Invalid command\n");
		return;
	}
	if (imagine.nrculor == 3) {
		printf("Black and white image needed\n");
		return;
	}
	interval = 256 / y;
	for (int i = 0; i < imagine.n; i++)
		for (int j = 0; j < imagine.m; j++)
			v[(int)(imagine.matriceculor)[i][j][0]]++;
	for (int i = 0; i < 256; i = i + interval) {
		nrstel2 = 0;
		for (int j = 0; j < interval; j++)
			nrstel2 = nrstel2 + v[i + j];
		if (nrstel2 > max)
			max = nrstel2;
	}
	//La histograma mai inati o sa calculam intr-un vector de frecventa
	//de cate ori apare fiecare valoare poxibila a unui pixel, apoi o sa
	//aflam valoarea maxima al unui interval cu dimensiunile specificate
	//iar la final o sa calculam numarul de stelute si o sa afisam histograma
	//propriuzisa
	for (int i = 0; i < 256; i = i + interval) {
		nrstel = 0;
		for (int j = 0; j < interval; j++)
			nrstel = nrstel + v[i + j];
		nrstel = (nrstel * x) / max;
		nrstel2 = trunc(nrstel);
		printf("%d\t|\t", nrstel2);
		while (nrstel2) {
			printf("*");
			nrstel2--;
		}
		printf("\n");
	}
}

void equalize(imagini *imagine, int existaimagin)
{
	int v[256] = {0}, noanta;
	double pixel;
	if (existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	if ((*imagine).nrculor == 3) {
		printf("Black and white image needed\n");
		return;
	}
	for (int i = 0; i < (*imagine).n; i++)
		for (int j = 0; j < (*imagine).m; j++) {
			noanta = ((*imagine).matriceculor)[i][j][0];
			v[noanta]++;
		}
	for (int i = 1; i < 256; i++)
		v[i] = v[i] + v[i - 1];
	//Pentru equalize dupa aflarea vectorului de frecventa o sa retinem
	//pe fiecare pozitie a acestuia suma tuturor aparitilor pixelilor
	//cu o valoare mai mica sau egala cu numarul pozitiei pentru a salva
	//timp in calculul valorilor noilor pixeli
	for (int i = 0; i < (*imagine).n; i++)
		for (int j = 0; j < (*imagine).m; j++) {
			noanta = ((*imagine).matriceculor)[i][j][0];
			pixel = (255 * v[noanta]) / ((*imagine).n * (*imagine).m);
			if (pixel > 255)
				pixel = 255;
			if (pixel < 0)
				pixel = 0;
			((*imagine).matriceculor)[i][j][0] = round(pixel);
		}
	printf("Equalize done\n");
}

void rotirepatrt(imagini *imagine, int x1, int y1, int x2, int y2, int unghi)
{
	int ***matriceaux, pixel;
	if (alocarematrice(y2 - y1, x2 - x1, (*imagine).nrculor, &matriceaux) == -1)
		return;
	while (unghi) {
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				for (int k = 0; k < (*imagine).nrculor; k++) {
					pixel = ((*imagine).matriceculor)[i][j][k];
					matriceaux[j - x1][y2 - i - 1][k] = pixel;
				}
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				for (int k = 0; k < (*imagine).nrculor; k++) {
					pixel = matriceaux[i - y1][j - x1][k];
					((*imagine).matriceculor)[i][j][k] = pixel;
				}
		unghi = unghi - 90;
	}
	dealocarematrice(y2 - y1, x2 - x1, &matriceaux);
}

void rotireimagine(imagini *imagine, int unghi, int *existaimagin)
{
	int ***matriceaux, pixel, ****p, n, m;
	while (unghi) {
		p = &matriceaux;
		m = (*imagine).m;
		n = (*imagine).n;
		if (alocarematrice(m, n, (*imagine).nrculor, p) == -1)
			return;
		for (int i = 0; i < (*imagine).n; i++)
			for (int j = 0; j < (*imagine).m; j++)
				for (int k = 0; k < (*imagine).nrculor; k++) {
					pixel = ((*imagine).matriceculor)[i][j][k];
					matriceaux[j][(*imagine).n - i - 1][k] = pixel;
				}
		p = &((*imagine).matriceculor);
		dealocarematrice((*imagine).n, (*imagine).m, p);
		if (alocarematrice(m, n, (*imagine).nrculor, p) == -1) {
			printf("Imagine pierduta\n");
			*existaimagin = 0;
			dealocarematrice((*imagine).n, (*imagine).m, &matriceaux);
			return;
		}
		swap(&((*imagine).n), &((*imagine).m));
		swap(&((*imagine).x1), &((*imagine).y1));
		swap(&((*imagine).x2), &((*imagine).y2));
		for (int i = 0; i < (*imagine).n; i++)
			for (int j = 0; j < (*imagine).m; j++)
				for (int k = 0; k < (*imagine).nrculor; k++) {
					pixel = matriceaux[i][j][k];
					((*imagine).matriceculor)[i][j][k] = pixel;
				}
		unghi = unghi - 90;
		dealocarematrice((*imagine).n, (*imagine).m, &matriceaux);
	}
}

void rotate(imagini *imagine, char *copielin, int *existaimagin)
{
	char *argument;
	int unghi, x1, x2, y1, y2, copieunghi;
	if (*existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	argument = strtok(copielin, "\n ");
	argument = strtok(NULL, "\n ");
	if (!argument) {
		printf("Invalid command\n");
		return;
	}
	unghi = 0;
	if (nr(argument, &unghi) == -1)
		return;
	copieunghi = unghi;
	unghi = (360 + unghi) % 360;
	if (unghi % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	x1 = (*imagine).x1;
	x2 = (*imagine).x2;
	y1 = (*imagine).y1;
	y2 = (*imagine).y2;
	if (x1 || y1 || x2 != (*imagine).m || y2 != (*imagine).n) {
		if ((*imagine).x2 - (*imagine).x1 != (*imagine).y2 - (*imagine).y1) {
			printf("The selection must be square\n");
		return;
		}
		rotirepatrt(imagine, x1, y1, x2, y2, unghi);
	} else {
		rotireimagine(imagine, unghi, existaimagin);
	}
	//Daca imaginea nu este selectata complet verificam daca
	//selectia este patratica
	//si apoi utilizam functia de rotate pentru un patrat,
	//altfel daca imaginea este
	//selectata complet avem o functie diferita si mai complexa
	printf("Rotated %d\n", copieunghi);
}

void crop(imagini *imagine, int *existaimagin)
{
	int pixel, x1, x2, y1, y2, ***matriceaux, ****p;
	if (*existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	x1 = (*imagine).x1;
	x2 = (*imagine).x2;
	y1 = (*imagine).y1;
	y2 = (*imagine).y2;
	if (alocarematrice(y2 - y1, x2 - x1, (*imagine).nrculor, &matriceaux) == -1)
		return;
	for (int i = y1; i < y2; i++)
		for (int j = x1; j < x2; j++)
			for (int k = 0; k < (*imagine).nrculor; k++) {
				pixel = ((*imagine).matriceculor)[i][j][k];
				matriceaux[i - y1][j - x1][k] = pixel;
			}
	p = &((*imagine).matriceculor);
	dealocarematrice((*imagine).n, (*imagine).m, p);
	(*imagine).n = y2 - y1;
	(*imagine).m = x2 - x1;
	(*imagine).x1 = (*imagine).y1 = 0;
	(*imagine).x2 = (*imagine).m;
	(*imagine).y2 = (*imagine).n;
	if (alocarematrice(y2 - y1, x2 - x1, (*imagine).nrculor, p) == -1) {
		printf("Imagine pierduta\n");
		*existaimagin = 0;
		dealocarematrice(y2 - y1, x2 - x1, &matriceaux);
		return;
	}
	for (int i = 0; i < (*imagine).n; i++)
		for (int j = 0; j < (*imagine).m; j++)
			for (int k = 0; k < (*imagine).nrculor; k++) {
				pixel = matriceaux[i][j][k];
				((*imagine).matriceculor)[i][j][k] = pixel;
			}
	dealocarematrice(y2 - y1, x2 - x1, &matriceaux);
	printf("Image cropped\n");
}

void save(imagini imagine, char *copielin, int existaimagin)
{
	char *numefisier, *argumentextra, caracter, dimen[50];
	if (existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	numefisier = strtok(copielin, "\n ");
	numefisier = strtok(NULL, "\n ");
	argumentextra = strtok(NULL, "\n ");
	FILE *f;
	f = fopen(numefisier, "wt");
	if (!argumentextra) {
		if (imagine.tipimag < 4)
			caracter = (char)(imagine.tipimag + 51);
		else
			caracter = (char)(imagine.tipimag + 48);
		fprintf(f, "P%c\n", caracter);
		sprintf(dimen, "%d %d\n", imagine.m, imagine.n);
		fprintf(f, "%s255\n", dimen);
		for (int i = 0; i < imagine.n; i++)
			for (int j = 0; j < imagine.m; j++)
				for (int k = 0; k < imagine.nrculor; k++)
					fprintf(f, "%c", (imagine.matriceculor)[i][j][k]);
	} else {
		if (!strcmp(argumentextra, "ascii")) {
			if (imagine.tipimag > 3)
				caracter = (char)(imagine.tipimag + 45);
			else
				caracter = (char)(imagine.tipimag + 48);
			fprintf(f, "P%c\n", caracter);
			sprintf(dimen, "%d %d\n", imagine.m, imagine.n);
			fprintf(f, "%s255\n", dimen);
			for (int i = 0; i < imagine.n; i++) {
				for (int j = 0; j < imagine.m; j++)
					for (int k = 0; k < imagine.nrculor; k++) {
						sprintf(dimen, "%d", (imagine.matriceculor)[i][j][k]);
						fprintf(f, "%s", dimen);
						fprintf(f, " ");
					}
				if (i != imagine.n)
					fprintf(f, "\n");
				}
			} else {
				printf("Invalid command\n");
			}
	}
	//Daca exista un al doilea argument la comanda de save
	//si acesta este ascii atunci salvarea
	//o sa se faca fara compresie, iar daca nu atunci cu ea
	fclose(f);
	printf("Saved %s\n", numefisier);
}

void edge(imagini *imagine, int ****matriceaux, int i, int j, int k)
{
	double pixel = 0;
	pixel += (-1) * ((*imagine).matriceculor)[i - 1][j - 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i - 1][j][k];
	pixel += (-1) * ((*imagine).matriceculor)[i - 1][j + 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i][j - 1][k];
	pixel += 8 * ((*imagine).matriceculor)[i][j][k];
	pixel += (-1) * ((*imagine).matriceculor)[i][j + 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i + 1][j - 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i + 1][j][k];
	pixel += (-1) * ((*imagine).matriceculor)[i + 1][j + 1][k];
	if (pixel > 255)
		pixel = 255;
	if (pixel < 0)
		pixel = 0;
	(*matriceaux)[i][j][k] = round(pixel);
}

void sharpen(imagini *imagine, int ****matriceaux, int i, int j, int k)
{
	double pixel = 0;
	pixel += 0 * ((*imagine).matriceculor)[i - 1][j - 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i - 1][j][k];
	pixel += 0 * ((*imagine).matriceculor)[i - 1][j + 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i][j - 1][k];
	pixel += 5 * ((*imagine).matriceculor)[i][j][k];
	pixel += (-1) * ((*imagine).matriceculor)[i][j + 1][k];
	pixel += 0 * ((*imagine).matriceculor)[i + 1][j - 1][k];
	pixel += (-1) * ((*imagine).matriceculor)[i + 1][j][k];
	pixel += 0 * ((*imagine).matriceculor)[i + 1][j + 1][k];
	if (pixel > 255)
		pixel = 255;
	if (pixel < 0)
		pixel = 0;
	(*matriceaux)[i][j][k] = round(pixel);
}

void blur(imagini *imagine, int ****matriceaux, int i, int j, int k)
{
	double pixel = 0;
	pixel += 1 * ((*imagine).matriceculor)[i - 1][j - 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i - 1][j][k];
	pixel += 1 * ((*imagine).matriceculor)[i - 1][j + 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i][j - 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i][j][k];
	pixel += 1 * ((*imagine).matriceculor)[i][j + 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i + 1][j - 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i + 1][j][k];
	pixel += 1 * ((*imagine).matriceculor)[i + 1][j + 1][k];
	pixel = pixel / 9;
	if (pixel > 255)
		pixel = 255;
	if (pixel < 0)
		pixel = 0;
	(*matriceaux)[i][j][k] = round(pixel);
}

void gaussian_blur(imagini *imagine, int ****matriceaux, int i, int j, int k)
{
	double pixel = 0;
	pixel += 1 * ((*imagine).matriceculor)[i - 1][j - 1][k];
	pixel += 2 * ((*imagine).matriceculor)[i - 1][j][k];
	pixel += 1 * ((*imagine).matriceculor)[i - 1][j + 1][k];
	pixel += 2 * ((*imagine).matriceculor)[i][j - 1][k];
	pixel += 4 * ((*imagine).matriceculor)[i][j][k];
	pixel += 2 * ((*imagine).matriceculor)[i][j + 1][k];
	pixel += 1 * ((*imagine).matriceculor)[i + 1][j - 1][k];
	pixel += 2 * ((*imagine).matriceculor)[i + 1][j][k];
	pixel += 1 * ((*imagine).matriceculor)[i + 1][j + 1][k];
	pixel = pixel / 16;
	if (pixel > 255)
		pixel = 255;
	if (pixel < 0)
		pixel = 0;
	(*matriceaux)[i][j][k] = round(pixel);
}

void apply(imagini *imagine, char *copielin, int existaimagin)
{
	char *argument;
	int ok = 0, i, j, ***matriceaux, n, m;
	if (existaimagin == 0) {
		printf("No image loaded\n");
		return;
	}
	argument = strtok(copielin, "\n ");
	argument = strtok(NULL, "\n ");
	if (!argument) {
		printf("Invalid command\n");
		return;
	}
	if ((*imagine).tipimag % 3) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	if ((*imagine).x1 < 1)
		j = 1;
	else
		j = (*imagine).x1;
	if ((*imagine).y1 < 1)
		i = 1;
	else
		i = (*imagine).y1;
	n = (*imagine).n;
	m = (*imagine).m;
	if (alocarematrice(n, m, (*imagine).nrculor, &matriceaux) == -1)
		return;
	for (; i < (*imagine).y2 && i < (*imagine).n - 1; i++) {
		for (; j < (*imagine).x2 && j < (*imagine).m - 1; j++)
			for (int k = 0; k < (*imagine).nrculor; k++) {
				if (!strcmp(argument, "EDGE")) {
					edge(imagine, &matriceaux, i, j, k);
					ok = 1;
				}
				if (!strcmp(argument, "SHARPEN")) {
					sharpen(imagine, &matriceaux, i, j, k);
					ok = 1;
				}
				if (!strcmp(argument, "BLUR")) {
					blur(imagine, &matriceaux, i, j, k);
					ok = 1;
				}
				if (!strcmp(argument, "GAUSSIAN_BLUR")) {
					gaussian_blur(imagine, &matriceaux, i, j, k);
					ok = 1;
				}
			}
		if ((*imagine).x1 < 1)
			j = 1;
		else
			j = (*imagine).x1;
	}
	if (ok == 0) {
		printf("APPLY parameter invalid\n");
		dealocarematrice((*imagine).n, (*imagine).m, &matriceaux);
		return;
	}
	if ((*imagine).x1 < 1)
		j = 1;
	else
		j = (*imagine).x1;
	if ((*imagine).y1 < 1)
		i = 1;
	else
		i = (*imagine).y1;
	for (; i < (*imagine).y2 && i < (*imagine).n - 1; i++) {
		for (; j < (*imagine).x2 && j < (*imagine).m - 1; j++)
			for (int k = 0; k < (*imagine).nrculor; k++)
				((*imagine).matriceculor)[i][j][k] = matriceaux[i][j][k];
		if ((*imagine).x1 < 1)
			j = 1;
		else
			j = (*imagine).x1;
	}
	printf("APPLY %s done\n", argument);
	dealocarematrice((*imagine).n, (*imagine).m, &matriceaux);
}

int main(void)
{
	imagini imagine;
	char *comanda, *linie, *copielin;
	int existaimagin = 0, ok = 0;
	size_t dimlin = 200;
	linie = malloc(dimlin * sizeof(char));
	do {
		dimlin = getline(&linie, &dimlin, stdin);
		//Citirea se face cu getline pentru ca nu stim care este
		//dimensiunea maxima a unei linii care poate fi citita
		//de la tastatura, iar apoi o sa copiem linia citita
		//intr-o variabila separata pentru a putea extrage argumentele
		//necesare din aceasta in utilizarea functilor pentru comenzii
		copielin = malloc(dimlin * sizeof(char) + 1);
		strcpy(copielin, linie);
		comanda = strtok(linie, "\n ");
		ok = 0;
		if (!strcmp(comanda, "LOAD")) {
			if (existaimagin == 1)
				dealocarematrice(imagine.n, imagine.m, &imagine.matriceculor);
			load(&imagine, copielin, &existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "SELECT")) {
			selectimag(&imagine, copielin, existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "HISTOGRAM")) {
			histograma(imagine, copielin, existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "EQUALIZE")) {
			equalize(&imagine, existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "ROTATE")) {
			rotate(&imagine, copielin, &existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "CROP")) {
			crop(&imagine, &existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "APPLY")) {
			apply(&imagine, copielin, existaimagin);
			ok = 1;
		}
		if (!strcmp(comanda, "SAVE")) {
			save(imagine, copielin, existaimagin);
			ok = 1;
		}
		if (existaimagin == 0 && !strcmp(comanda, "EXIT")) {
			printf("No image loaded\n");
			ok = 1;
		}
		if (!strcmp(comanda, "EXIT") && existaimagin == 1)
			ok = 1;
		if (!ok)
			printf("Invalid command\n");
		free(copielin);
	} while (strcmp(comanda, "EXIT"));
	free(linie);
	if (existaimagin == 1)
		dealocarematrice(imagine.n, imagine.m, &imagine.matriceculor);
}
