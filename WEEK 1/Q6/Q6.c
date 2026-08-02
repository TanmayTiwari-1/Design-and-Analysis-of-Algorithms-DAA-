#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisons;


int hasDuplicate(int arr[], int n)
{
    comparisons = 0;

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            comparisons++;

            if (arr[i] == arr[j])
                return 1;
        }
    }

    return 0;
}

int main()
{
    srand(time(NULL));

    FILE *fp = fopen("comparisons.csv", "w");

    if (fp == NULL)
    {
        printf("Error creating CSV file!\n");
        return 1;
    }

    fprintf(fp, "Array Size,Comparisons,Duplicate Found\n");

    for (int n = 100; n <= 5000; n += 100)
    {
        int arr[n];

     
        for (int i = 0; i < n; i++)
            arr[i] = rand() % 1000000;

        int duplicate = hasDuplicate(arr, n);

        fprintf(fp, "%d,%lld,%s\n",
                n,
                comparisons,
                duplicate ? "Yes" : "No");

        printf("n = %4d   Comparisons = %-10lld   Duplicate = %s\n",
               n,
               comparisons,
               duplicate ? "Yes" : "No");
    }

    fclose(fp);

    printf("\ncomparisons.csv generated successfully.\n");

    return 0;
}
