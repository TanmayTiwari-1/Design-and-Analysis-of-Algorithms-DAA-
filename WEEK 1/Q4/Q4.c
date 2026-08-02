#include <stdio.h>

long long moves = 0;

void towerOfHanoi(int n, char source, char auxiliary, char destination)
{
    if (n == 1)
    {
        moves++;
        return;
    }

    towerOfHanoi(n - 1, source, destination, auxiliary);

    moves++;

    towerOfHanoi(n - 1, auxiliary, source, destination);
}

int main()
{
    FILE *fp = fopen("moves.csv", "w");
    if (fp == NULL)
    {
        perror("Unable to open moves.csv");
        return 1;
    }

    fprintf(fp, "Disks,Moves\n");

    for (int n = 1; n <= 20; n++)
    {
        moves = 0;

        towerOfHanoi(n, 'A', 'B', 'C');

        fprintf(fp, "%d,%lld\n", n, moves);

        printf("%2d Disks -> %lld Moves\n", n, moves);
    }

    fclose(fp);

    printf("\nmoves.csv generated successfully.\n");

    return 0;
}
