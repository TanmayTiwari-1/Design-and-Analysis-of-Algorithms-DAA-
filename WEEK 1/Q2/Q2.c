#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {

    srand(time(NULL));

    FILE *fp = fopen("results.csv", "w");

    fprintf(fp, "Trials,Fair,Biased\n");

    int fairHeads = 0;
    int biasedHeads = 0;

    for (int i = 1; i <= 100000; i++) {

        // Fair Coin (P(H)=0.5)
        if (rand() % 2)
            fairHeads++;

        // Biased Coin (P(H)=0.7)
        double r = (double)rand() / RAND_MAX;
        if (r < 0.7)
            biasedHeads++;

        if (i % 100 == 0) {
            fprintf(fp, "%d,%lf,%lf\n",
                    i,
                    (double)fairHeads / i,
                    (double)biasedHeads / i);
        }
    }

    fclose(fp);

    printf("results.csv generated successfully.\n");

    return 0;
}
