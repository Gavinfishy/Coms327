#include <stdio.h>

void times_table(int a[10][10])
{

	int i,j;

	for (j=0; j < 10; j++) {
		for (i = 0; i < 10; i++) {
			a[j][i] = (i + 1) * (j + 1);
		}
	}
}

int main(int argc, char *argv[])
{
	int a[10][10];
	int i,j;

	times_table(a);

	for(j = 0; j < 10; j++) {
		for (i = 0; i <10; i++) {
			printf("%3d", a[j][i]);
		}
		printf("\n");
	}
	return 0;
}