#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("results.txt", "r");
    if (fp == NULL) {
        perror("Unable to open the file");
        return 1;
    }

    int fast_diff, slow_diff;
    long long sum_fast_diff = 0, sum_slow_diff = 0;
    int count = 0;
    while (fscanf(fp, "fast diff %d\nslow diff %d\n", &fast_diff, &slow_diff) == 2) {
        sum_fast_diff += fast_diff;
        sum_slow_diff += slow_diff;
        count += 1;
    }
    fclose(fp);

    if (count == 0) {
        printf("No data was read.\n");
        return 1;
    }
    printf("count %d",count);
    double avg_fast_diff = (double)sum_fast_diff / count;
    double avg_slow_diff = (double)sum_slow_diff / count;

    // Writing averages to a CSV file
    FILE *fout = fopen("averages.csv", "w");
    if (fout == NULL) {
        perror("Unable to create the file");
        return 1;
    }
    fprintf(fout, "Average Fast Diff,Average Slow Diff\n%.2f,%.2f\n", avg_fast_diff, avg_slow_diff);
    fclose(fout);

    printf("Averages written to averages.csv\n");

    return 0;
}
