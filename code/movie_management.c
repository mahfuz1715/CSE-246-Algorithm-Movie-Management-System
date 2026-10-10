#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MOVIES 10000
#define MAX_GENRES 10
#define MAX_NAME_LENGTH 20
#define MAX_DURATION 200
#define MAX_FRANCHISE_ORDER 10

// Structure to store movie details
typedef struct {
    char name[MAX_NAME_LENGTH];
    int duration;
    char genres[MAX_GENRES][MAX_NAME_LENGTH];
    int genre_count;
    char franchise[MAX_NAME_LENGTH];
    int franchise_order;
} Movie;

// Function to compute LCS length
int lcs(char X[MAX_GENRES][MAX_NAME_LENGTH], int m,
         char Y[MAX_GENRES][MAX_NAME_LENGTH], int n) {

    int **dp = malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; i++) {
        dp[i] = malloc((n + 1) * sizeof(int));
    }

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (strcmp(X[i - 1], Y[j - 1]) == 0)
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = (dp[i - 1][j] > dp[i][j - 1])
                           ? dp[i - 1][j] : dp[i][j - 1];
        }
    }

    int result = dp[m][n];

    for (int i = 0; i <= m; i++) {
        free(dp[i]);
    }

    free(dp);
    return result;
}

// Function to perform 0/1 Knapsack
int knapsack(Movie movies[], int n, int time_limit, int selected[]) {
    int **dp = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        dp[i] = malloc((time_limit + 1) * sizeof(int));
    }

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= time_limit; j++) {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (movies[i - 1].duration <= j)
                dp[i][j] = (dp[i - 1][j] >
                            dp[i - 1][j - movies[i - 1].duration] + 1)
                           ? dp[i - 1][j]
                           : dp[i - 1][j - movies[i - 1].duration] + 1;
            else
                dp[i][j] = dp[i - 1][j];
        }
    }

    int remaining_time = time_limit;
    for (int i = n; i > 0; i--) {
        if (dp[i][remaining_time] != dp[i - 1][remaining_time]) {
            selected[i - 1] = 1; // Mark movie as selected
            remaining_time -= movies[i - 1].duration;
        }
    }

    int result = dp[n][time_limit];

    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }

    free(dp);
    return result;
}

// Function to perform Topological Sort
void topological_sort(Movie movies[], int n, int selected[], FILE *output_file) {
    // Create a graph representation
    int **graph = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = calloc(n, sizeof(int));
    }

    int *in_degree = calloc(n, sizeof(int));

    // Build the graph based on franchise order
    for (int i = 0; i < n; i++) {
        if (!selected[i]) continue; // Skip unselected movies

        for (int j = 0; j < n; j++) {
            if (!selected[j]) continue; // Skip unselected movies

            if (strcmp(movies[i].franchise, movies[j].franchise) == 0 &&
                movies[i].franchise_order < movies[j].franchise_order) {
                graph[i][j] = 1; // Add edge from i to j
                in_degree[j]++;
            }
        }
    }

    // Perform topological sort using Kahn's algorithm
    int *queue = malloc(n * sizeof(int));
    int front = 0, rear = 0;

    // Enqueue movies with in-degree 0
    for (int i = 0; i < n; i++) {
        if (selected[i] && in_degree[i] == 0) {
            queue[rear++] = i;
        }
    }

    // Write the sorted order to output file
    fprintf(output_file, "\nTopological Order of Selected Movies:\n");

    while (front < rear) {
        int u = queue[front++];
        fprintf(output_file, "%s\n", movies[u].name);

        // Reduce in-degree of adjacent movies
        for (int v = 0; v < n; v++) {
            if (graph[u][v] && selected[v]) {
                in_degree[v]--;
                if (in_degree[v] == 0) {
                    queue[rear++] = v;
                }
            }
        }
    }

    // Free allocated memory
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }

    free(graph);
    free(in_degree);
    free(queue);
}

int main() {
    Movie *movies = malloc(MAX_MOVIES * sizeof(Movie)); // Allocate on heap
    int **similarity = malloc(MAX_MOVIES * sizeof(int *)); // Allocate on heap

    for (int i = 0; i < MAX_MOVIES; i++) {
        similarity[i] = malloc(MAX_MOVIES * sizeof(int));
    }

    int *selected = calloc(MAX_MOVIES, sizeof(int)); // Allocate on heap
    int n = 0, time_limit = 0;
    clock_t start, end;
    double lcs_time, knapsack_time, topo_time;

    // Read input from file
    FILE *input_file = fopen("input.txt", "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    // Read time limit
    fscanf(input_file, "%d", &time_limit);

    // Read movie details
    while (fscanf(input_file, "%s %d %d",
                 movies[n].name,
                 &movies[n].duration,
                 &movies[n].genre_count) != EOF) {

        for (int i = 0; i < movies[n].genre_count; i++) {
            fscanf(input_file, "%s", movies[n].genres[i]);
        }

        fscanf(input_file, "%s %d",
               movies[n].franchise,
               &movies[n].franchise_order);

        n++;
    }

    fclose(input_file);

    // Measure LCS execution time
    start = clock();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            similarity[i][j] = lcs(
                movies[i].genres, movies[i].genre_count,
                movies[j].genres, movies[j].genre_count
            );
        }
    }

    end = clock();
    lcs_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Measure Knapsack execution time
    start = clock();
    int max_movies = knapsack(movies, n, time_limit, selected);
    end = clock();
    knapsack_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Write output to file
    FILE *output_file = fopen("output.txt", "w");
    if (!output_file) {
        perror("Error opening output file");
        return 1;
    }

    fprintf(output_file, "Selected Movies:\n");
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            fprintf(output_file, "%s\n", movies[i].name);
        }
    }

    // Measure Topological Sort execution time
    start = clock();
    topological_sort(movies, n, selected, output_file);
    end = clock();
    topo_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    fclose(output_file);

    // Write execution times to file
    FILE *time_file = fopen("time_results.txt", "w");
    if (!time_file) {
        perror("Error opening time results file");
        return 1;
    }

    fprintf(time_file, "LCS Time: %f seconds\n", lcs_time);
    fprintf(time_file, "Knapsack Time: %f seconds\n", knapsack_time);
    fprintf(time_file, "Topological Sort Time: %f seconds\n", topo_time);

    fclose(time_file);

    // Free allocated memory
    free(movies);

    for (int i = 0; i < MAX_MOVIES; i++) {
        free(similarity[i]);
    }

    free(similarity);
    free(selected);

    printf("Program executed successfully. Check output.txt and time_results.txt for results.\n");

    return 0;
}
