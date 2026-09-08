#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MOVIES 10000
#define MAX_GENRES 10
#define MAX_NAME_LENGTH 20
#define MAX_DURATION 200
#define MAX_FRANCHISE_ORDER 10

// Arrays for random data generation
const char *GENRES[] = {"Action", "Comedy", "Drama", "Horror", "Sci-Fi", "Thriller",
                        "Romance", "Adventure", "Fantasy", "Mystery"};

const char *FRANCHISES[] = {"FranchiseA", "FranchiseB", "FranchiseC", "FranchiseD",
                            "FranchiseE"};

// Function to generate a random movie
void generate_movie(FILE *file, int index) {
    char name[MAX_NAME_LENGTH];
    int duration = 60 + rand() % (MAX_DURATION - 60 + 1); // Random duration between 60 and 200 minutes
    int genre_count = 1 + rand() % MAX_GENRES; // Random number of genres (1 to 10)
    int franchise_index = rand() % (sizeof(FRANCHISES) / sizeof(FRANCHISES[0])); // Random franchise
    int franchise_order = 1 + rand() % MAX_FRANCHISE_ORDER; // Random franchise order (1 to 10)

    // Generate movie name
    sprintf(name, "Movie%d", index + 1);

    // Write movie details to file
    fprintf(file, "%s %d %d ", name, duration, genre_count);

    // Write genres in the same order for every movie
    for (int i = 0; i < genre_count; i++) {
        fprintf(file, "%s ", GENRES[i % (sizeof(GENRES) / sizeof(GENRES[0]))]);
    }

    // Write franchise and franchise order
    fprintf(file, "%s %d\n", FRANCHISES[franchise_index], franchise_order);
}

int main() {
    int num_movies;
    FILE *file;

    // random number generator
    srand(time(NULL));

    // number of movies scan
    printf("Enter the number of movies to generate (100 to 10000): ");
    if (scanf("%d", &num_movies) != 1 || num_movies < 100 || num_movies > MAX_MOVIES) {
        printf("Invalid input. Please enter a number between 100 and %d.\n", MAX_MOVIES);
        return 1;
    }

    // file opening for writing
    file = fopen("input.txt", "w");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Write time limit
    fprintf(file, "500\n");

    // Generate and write random movies
    for (int i = 0; i < num_movies; i++) {
        generate_movie(file, i);
    }

    // Close the file
    fclose(file);
    printf("Generated %d movies in input.txt.\n", num_movies);

    return 0;
}
