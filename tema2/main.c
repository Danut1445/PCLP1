//Copyright Tunsoiu Dan-Andrei 315CA 2022-2023
#include<stdio.h>
#include<stdlib.h>

void citiremat(int ****mat, int **dimmat, int *nrmat)
{
	int n, m;
	scanf("%d%d", &n, &m);
	(*nrmat)++;
	if ((*nrmat) == 1) {
		(*mat) = malloc((*nrmat) * sizeof(int **));
		if ((*mat) == NULL)
			return;
		(*dimmat) = malloc((*nrmat) * 2 * sizeof(int *));
		if ((*dimmat) == NULL)
			return;
	} else {
		(*mat) = realloc((*mat), (*nrmat) * sizeof(int **));
		if ((*mat) == NULL)
			return;
		(*dimmat) = realloc((*dimmat), (*nrmat) * 2 * sizeof(int *));
		if ((*dimmat) == NULL)
			return;
	}
	(*mat)[(*nrmat) - 1] = malloc(n * sizeof(int *));
	if ((*mat)[(*nrmat) - 1] == NULL)
		return;
	for (int i = 0; i < n; i++) {
		(*mat)[(*nrmat) - 1][i] = malloc(m * sizeof(int));
		if ((*mat)[(*nrmat) - 1][i] == NULL) {
			for (int j = 0; j < i; j++)
				free((*mat)[((*nrmat) - 1)][j]);
			free((*mat)[((*nrmat) - 1)]);
			return;
		}
		for (int j = 0; j < m; j++) {
			scanf("%d", &((*mat)[(*nrmat) - 1][i][j]));
			(*mat)[(*nrmat) - 1][i][j] = (*mat)[(*nrmat) - 1][i][j] % 10007;
		}
	}
	(*dimmat)[2 * ((*nrmat) - 1)] = n;
	(*dimmat)[2 * ((*nrmat) - 1) + 1] = m;
}

void eliberareaux(int ***aux, int n)
{
	for (int i = 0; i < n; i++)
		free((*aux)[i]);
	free((*aux));
}

void eliberaretot(int ****mat, int **dimmat, int nrmat)
{
	int k = 0;
	while (k < nrmat) {
		eliberareaux(&((*mat)[k]), (*dimmat)[2 * k]);
		k = k + 1;
	}
	free((*mat));
	free((*dimmat));
}

void dimensiune(int *dimmat, int nrmat)
{
	int n;
	scanf("%d", &n);
	if (n >= nrmat || n < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", dimmat[2 * n], dimmat[2 * n + 1]);
}

void afisare(int ***mat, int *dimmat, int nrmat)
{
	int n;
	scanf("%d", &n);
	if (n >= nrmat || n < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	for (int i = 0; i < dimmat[2 * n]; i++) {
		for (int j = 0; j < dimmat[2 * n + 1]; j++)
			printf("%d ", mat[n][i][j]);
		printf("\n");
	}
}

void redimensionare(int ****mat, int **dimmat, int nrmat)
{
	int a, n, m, *lin, *col, **aux;
	scanf("%d", &a);
	scanf("%d", &n);
	lin = malloc(n * sizeof(int));
	if (!lin)
		return;
	for (int i = 0; i < n; i++)
		scanf("%d", &lin[i]);
	scanf("%d", &m);
	col = malloc(m * sizeof(int));
	if (!col)
		return;
	for (int i = 0; i < m; i++)
		scanf("%d", &col[i]);
	if (a < nrmat && a >= 0) {//alocarea auxiliarei pentru redimensionare
		aux = malloc((*dimmat)[2 * a] * sizeof(int *));
		if (!aux)
			return;
		for (int i = 0; i < (*dimmat)[2 * a]; i++) {
			aux[i] = malloc((*dimmat)[2 * a + 1] * sizeof(int));
			for (int j = 0; j < (*dimmat)[2 * a + 1]; j++)
				aux[i][j] = (*mat)[a][i][j];
			free((*mat)[a][i]);
		}
		(*mat)[a] = realloc((*mat)[a], n * sizeof(int *));
		for (int i = 0; i < n; i++) {
			(*mat)[a][i] = malloc(m * sizeof(int));
			for (int j = 0; j < m; j++)
				(*mat)[a][i][j] = aux[lin[i]][col[j]];
		}
		for (int i = 0; i < (*dimmat)[2 * a]; i++)
			free(aux[i]);
		free(aux);
		(*dimmat)[2 * a] = n;
		(*dimmat)[2 * a + 1] = m;
	} else {
		printf("No matrix with the given index\n");
	}
	free(lin);
	free(col);
}

int modulo(int a)
{
	return (a % 10007 + 10007) % 10007;
}

void inmultirematr(int **a, int **b, int ***c, int x, int y, int z)
{
	for (int i = 0; i < x; i++)
		for (int j = 0; j < z; j++) {
			(*c)[i][j] = 0;
			for (int k = 0; k < y; k++)
				(*c)[i][j] = modulo((*c)[i][j] + modulo(a[i][k] * b[k][j]));
			(*c)[i][j] = modulo((*c)[i][j]);
		}
}

void inmultireclasic(int ****mat, int **dimmat, int *nrmat)
{
	int n, m, *aux2, **aux1, x, y, z;
	scanf("%d%d", &n, &m);
	if (n >= *nrmat || m >= *nrmat || n < 0 || m < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*dimmat)[2 * n + 1] != (*dimmat)[2 * m]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	(*nrmat)++;
	(*mat) = realloc((*mat), (*nrmat) * sizeof(int **));
	if ((*mat) == NULL)
		return;
	(*dimmat) = realloc((*dimmat), (*nrmat) * 2 * sizeof(int));
	if ((*dimmat) == NULL)
		return;
	(*dimmat)[((*nrmat) - 1) * 2] = (*dimmat)[2 * n];
	(*dimmat)[2 * ((*nrmat) - 1) + 1] = (*dimmat)[2 * m + 1];
	aux1 = malloc((*dimmat)[2 * ((*nrmat) - 1)] * sizeof(int *));
	(*mat)[(*nrmat) - 1] = aux1;
	if ((*mat)[(*nrmat) - 1] == NULL)
		return;
	for (int i = 0; i < (*dimmat)[2 * ((*nrmat) - 1)]; i++) {
		aux2 = malloc((*dimmat)[2 * ((*nrmat) - 1) + 1] * sizeof(int));
		(*mat)[(*nrmat) - 1][i] = aux2;
		if ((*mat)[(*nrmat) - 1][i] == NULL) {
			for (int j = 0; j < i; j++)
				free((*mat)[(*nrmat) - 1][j]);
			free((*mat)[(*nrmat) - 1]);
			(*nrmat)--;
			(*mat) = realloc((*mat), (*nrmat) * sizeof(int **));
			return;
		}
	}
	x = (*dimmat)[2 * n];
	y = (*dimmat)[2 * m];
	z = (*dimmat)[2 * m + 1];
	inmultirematr((*mat)[n], (*mat)[m], &((*mat)[(*nrmat) - 1]), x, y, z);
}

void swap(int **a, int x, int y)
{
	int aux = (*a)[x];
	(*a)[x] = (*a)[y];
	(*a)[y] = aux;
}

void sortare(int ****mat, int **dimmat, int nrmat)
{
	int *s, ok, **aux;
	s = malloc(nrmat * sizeof(int));
	if (!s)
		return;
	for (int k = 0; k < nrmat; k++) {
		s[k] = 0;
		for (int i = 0; i < (*dimmat)[2 * k]; i++)
			for (int j = 0; j < (*dimmat)[2 * k + 1]; j++)
				s[k] = modulo(s[k] + (*mat)[k][i][j]);
	}
	do {
		ok = 0;
		for (int k = 0; k < nrmat - 1; k++) {
			if (s[k] > s[k + 1]) {
				ok = 1;
				aux = (*mat)[k];
				(*mat)[k] = (*mat)[k + 1];
				(*mat)[k + 1] = aux;
				swap(&s, k, k + 1);
				swap(dimmat, 2 * k, 2 * (k + 1));
				swap(dimmat, 2 * k + 1, 2 * (k + 1) + 1);
			}
			if (s[k] == s[k + 1] && (*dimmat)[2 * k] > (*dimmat)[2 * (k + 1)]) {
				ok = 1;
				aux = (*mat)[k];
				(*mat)[k] = (*mat)[k + 1];
				(*mat)[k + 1] = aux;
				swap(&s, k, k + 1);
				swap(dimmat, 2 * k, 2 * (k + 1));
				swap(dimmat, 2 * k + 1, 2 * (k + 1) + 1);
			}
		}
	} while (ok == 1);
	free(s);
}

void transpunere(int ****mat, int **dimmat, int nrmat)
{
	int n;
	scanf("%d", &n);
	if (n >= nrmat || n < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	int **aux;//alocarea auxiliarei pentru transpunere
	aux = malloc((*dimmat)[2 * n] * sizeof(int *));
	if (!aux)
		return;
	for (int i = 0; i < (*dimmat)[2 * n]; i++) {
		aux[i] = malloc((*dimmat)[2 * n + 1] * sizeof(int));
		if (!aux[i]) {
			for (int j = 0; j < i; j++)
				free(aux[j]);
			free(aux);
			return;
		}
		for (int j = 0; j < (*dimmat)[2 * n + 1]; j++)
			aux[i][j] = (*mat)[n][i][j];
		free((*mat)[n][i]);
	}
	(*mat)[n] = realloc((*mat)[n], (*dimmat)[2 * n + 1] * sizeof(int *));
	if ((*mat)[n] == NULL) {
		printf("EROARE!\n");
		return;
	}
	for (int i = 0; i < (*dimmat)[2 * n + 1]; i++) {
		(*mat)[n][i] = malloc((*dimmat)[2 * n] * sizeof(int *));
		if ((*mat)[n][i] == NULL) {
			for (int j = 0; j < i; j++)
				free((*mat)[n][j]);
			free((*mat)[n]);
			return;
		}
		for (int j = 0; j < (*dimmat)[2 * n]; j++)
			(*mat)[n][i][j] = aux[j][i];
	}
	for (int i = 0; i < (*dimmat)[2 * n]; i++)
		free(aux[i]);
	free(aux);
	swap(dimmat, 2 * n, 2 * n + 1);
}

void alocareaux(int ***aux, int n, int m)
{
	(*aux) = malloc(n * sizeof(int *));
	if ((*aux) == NULL)
		return;
	for (int i = 0; i < n; i++) {
		(*aux)[i] = malloc(m * sizeof(int));
		if ((*aux)[i] == NULL) {
			for (int j = 0; j < i; j++)
				free((*aux)[i]);
			free((*aux));
			return;
		}
	}
}

void ridicarelaput(int ****mat, int *dimmat, int nrmat)
{
	int n, p, **aux1, **aux2, *baza2, nr = 0, x;
	scanf("%d%d", &n, &p);
	if (n >= nrmat || n < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if (p < 0) {
		printf("Power should be positive\n");
		return;
	}
	if (p == 1)
		return;
	if (dimmat[2 * n] != dimmat[2 * n + 1]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	//alcoarea matricilor auxiliare ot ridicare la putere
	alocareaux(&aux1, dimmat[2 * n], dimmat[2 * n]);
	alocareaux(&aux2, dimmat[2 * n], dimmat[2 * n]);
	if (!aux1 || !aux2)
		return;
	for (int i = 0; i < dimmat[2 * n]; i++)
		for (int j = 0; j < dimmat[2 * n]; j++) {
			if (i != j)
				aux2[i][j] = 0;
			else
				aux2[i][j] = 1;
			aux1[i][j] = (*mat)[n][i][j];
		}
	while (p) {//Transformarea in baza2 a lui p
		nr++;
		if (nr == 1)
			baza2 = malloc(nr * sizeof(int));
		else
			baza2 = realloc(baza2, nr * sizeof(int));
		baza2[nr - 1] = p % 2;
		p = p / 2;
	}
	for (int i = 0; i < dimmat[2 * n]; i++)
		for (int j = 0; j < dimmat[2 * n]; j++)
			(*mat)[n][i][j] = aux2[i][j];
	for (int k = nr - 1; k >= 0; k--) {
		x = dimmat[2 * n];
		inmultirematr(aux2, aux2, &((*mat)[n]), x, x, x);
		for (int i = 0; i < dimmat[2 * n]; i++)
			for (int j = 0; j < dimmat[2 * n]; j++)
				aux2[i][j] = (*mat)[n][i][j];
		if (baza2[k] == 1)
			inmultirematr(aux2, aux1, &((*mat)[n]), x, x, x);
		for (int i = 0; i < dimmat[2 * n]; i++)
			for (int j = 0; j < dimmat[2 * n]; j++)
				aux2[i][j] = (*mat)[n][i][j];
	}
	eliberareaux(&aux1, dimmat[2 * n]);
	eliberareaux(&aux2, dimmat[2 * n]);
	if (nr != 0)
		free(baza2);
}

void eliberareomat(int ****mat, int **dimmat, int *nrmat)
{
	int n;
	scanf("%d", &n);
	if (n >= (*nrmat) || n < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	eliberareaux(&((*mat)[n]), (*dimmat)[2 * n]);
	for (int i = n; i < ((*nrmat) - 1); i++) {
		(*mat)[i] = (*mat)[i + 1];
		(*dimmat)[2 * i] = (*dimmat)[2 * (i + 1)];
		(*dimmat)[2 * i + 1] = (*dimmat)[2 * (i + 1) + 1];
	}
	(*nrmat)--;
	(*mat) = realloc((*mat), (*nrmat) * sizeof(int **));
	(*dimmat) = realloc((*dimmat), (*nrmat) * 2 * sizeof(int));
}

//Prima dintre cele 7 functii de calculare a matricilor auxiliare pentru Strasse
void M1(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i][j] + a[i + x / 2][j + x / 2]);
			(*aux2)[i][j] = modulo(b[i][j] + b[i + x / 2][j + x / 2]);
		}
}

void M2(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i + x / 2][j] + a[i + x / 2][j + x / 2]);
			(*aux2)[i][j] = modulo(b[i][j]);
		}
}

void M3(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i][j]);
			(*aux2)[i][j] = modulo(b[i][j + x / 2] - b[i + x / 2][j + x / 2]);
		}
}

void M4(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i + x / 2][j + x / 2]);
			(*aux2)[i][j] = modulo(b[i + x / 2][j] - b[i][j]);
		}
}

void M5(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i][j] + a[i][j + x / 2]);
			(*aux2)[i][j] = modulo(b[i + x / 2][j + x / 2]);
		}
}

void M6(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i + x / 2][j] - a[i][j]);
			(*aux2)[i][j] = modulo(b[i][j] + b[i][j + x / 2]);
		}
}

void M7(int **a, int **b, int ***aux1, int ***aux2, int x)
{
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			(*aux1)[i][j] = modulo(a[i][j + x / 2] - a[i + x / 2][j + x / 2]);
			(*aux2)[i][j] = modulo(b[i + x / 2][j] + b[i + x / 2][j + x / 2]);
		}
}

void strassen(int **a, int **b, int ***c, int x)
{
	int ***maux, **aux1, **aux2, q, p, z;
	maux = malloc(7 * sizeof(int **));
	if (!maux)
		return;
	for (int i = 0; i < 7; i++) {
		alocareaux(&maux[i], x / 2, x / 2);
		if (!maux[i]) {
			for (int j = 0; j < i; j++)
				eliberareaux(&maux[i], x / 2);
			free(maux);
			return;
		}
	}
	alocareaux(&(aux1), x / 2, x / 2);
	alocareaux(&(aux2), x / 2, x / 2);
	if (!aux1 || !aux2)
		return;
	M1(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[0], x / 2);
	else
		maux[0][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M2(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[1], x / 2);
	else
		maux[1][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M3(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[2], x / 2);
	else
		maux[2][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M4(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[3], x / 2);
	else
		maux[3][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M5(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[4], x / 2);
	else
		maux[4][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M6(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[5], x / 2);
	else
		maux[5][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	M7(a, b, &aux1, &aux2, x);
	if (x > 2)
		strassen(aux1, aux2, &maux[6], x / 2);
	else
		maux[6][0][0] = modulo(aux1[0][0] * aux2[0][0]);
	for (int i = 0; i < x / 2; i++)
		for (int j = 0; j < x / 2; j++) {
			z = maux[0][i][j];
			q = modulo(z + maux[3][i][j] - maux[4][i][j] + maux[6][i][j]);
			(*c)[i][j] = q;
			(*c)[i][j + x / 2] = modulo(maux[2][i][j] + maux[4][i][j]);
			(*c)[i + x / 2][j] = modulo(maux[1][i][j] + maux[3][i][j]);
			p = modulo(z - maux[1][i][j] + maux[2][i][j] + maux[5][i][j]);
			(*c)[i + x / 2][j + x / 2] = p;
		}
	for (int i = 0; i < 7; i++)
		eliberareaux(&maux[i], x / 2);
	free(maux);
	eliberareaux(&aux1, x / 2);
	eliberareaux(&aux2, x / 2);
}

void pregatirestrassen(int ****mat, int **dimmat, int *nrmat)
{
	int n, m;
	scanf("%d%d", &n, &m);
	if (n >= (*nrmat) || n < 0 || m >= (*nrmat) || m < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*dimmat)[2 * n + 1] != (*dimmat)[2 * m]) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	(*nrmat)++;
	(*mat) = realloc((*mat), (*nrmat) * sizeof(int **));
	if ((*mat) == NULL) {
		printf("EROARE!\n");
		return;
	}
	(*dimmat) = realloc((*dimmat), 2 * (*nrmat) * sizeof(int));
	if ((*dimmat) == NULL) {
		printf("EROARE\n");
		return;
	}
	alocareaux(&((*mat)[(*nrmat) - 1]), (*dimmat)[2 * n], (*dimmat)[2 * m + 1]);
	(*dimmat)[2 * ((*nrmat) - 1)] = (*dimmat)[2 * n];
	(*dimmat)[2 * ((*nrmat) - 1) + 1] = (*dimmat)[2 * m + 1];
	strassen((*mat)[n], (*mat)[m], &((*mat)[((*nrmat) - 1)]), (*dimmat)[2 * n]);
}

int main(void)
{
	int ***mat, *dimmat, nrmat, ok;
	char c;
	nrmat = 0;
	while (scanf("%c", &c)) {
		getchar();
		ok = 0;
		if (c == 'L') {
			ok = 1;
			citiremat(&mat, &dimmat, &nrmat);
			getchar();
		}
		if (c == 'D') {
			ok = 1;
			dimensiune(dimmat, nrmat);
			getchar();
		}
		if (c == 'P') {
			ok = 1;
			afisare(mat, dimmat, nrmat);
			getchar();
		}
		if (c == 'C') {
			ok = 1;
			redimensionare(&mat, &dimmat, nrmat);
			getchar();
		}
		if (c == 'M') {
			ok = 1;
			inmultireclasic(&mat, &dimmat, &nrmat);
			getchar();
		}
		if (c == 'O') {
			ok = 1;
			sortare(&mat, &dimmat, nrmat);
		}
		if (c == 'T') {
			ok = 1;
			transpunere(&mat, &dimmat, nrmat);
			getchar();
		}
		if (c == 'R') {
			ok = 1;
			ridicarelaput(&mat, dimmat, nrmat);
			getchar();
		}
		if (c == 'F') {
			ok = 1;
			eliberareomat(&mat, &dimmat, &nrmat);
			getchar();
		}
		if (c == 'S') {
			ok = 1;
			pregatirestrassen(&mat, &dimmat, &nrmat);
			getchar();
		}
		/*if(c == 'X')
		{
			for(int k=0;k<nrmat;k++)
			{
				for(int i=0;i<dimmat[2*k];i++)
				{
					for(int j=0;j<dimmat[2*k+1];j++)
						printf("%d ", mat[k][i][j]);
					printf("\n");
				}
				printf("\n");
			}
		}*/
		if (c == 'Q') {
			eliberaretot(&mat, &dimmat, nrmat);
			break;
		}
		if (ok == 0)
			printf("Unrecognized command\n");
	}
}
