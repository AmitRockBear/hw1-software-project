#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define eps 0.0001
#define MAX_LINE_LENGTH 1024

int countCommas(const char *str) {
    int count = 0;

    while (*str) {
        if (*str == ',') {
            count++;
        }
        str++;
    }

    return count;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
      return 1;
    }
    int k = atoi(argv[1]);
    int iter = atoi(argv[2]);

  	if (iter <= 1 || iter >= 1000) {
      printf("Invalid maximum iteration!");
    }

    char *arrayOfStrings[1];
    int counter = 0;

    FILE *fp = stdin;
    char line[MAX_LINE_LENGTH];
    float **vectors = NULL;
    int capacity = 0;
    int line_count = 0;

    if (fp == NULL) {
      perror("Error opening input");
      exit(1);
    }


    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';

         if (line_count == capacity) {
            capacity += 50;
            vectors = realloc(vectors, capacity * sizeof(float *));
            if (vectors == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }
        
        size_t d = countCommas(line) + 1;
        vectors[line_count] = calloc(d, sizeof(float));
        if (vectors[line_count] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        // Tokenize the input string using strtok
        char *token = strtok(line, ",");

        int tokensCounter = 0;        
        while (token != NULL) {
            vectors[line_count][tokensCounter] = strtof(token, NULL);
            
            token = strtok(NULL, ",");
            tokensCounter++;
        }

        line_count++;
    }

    if (k <=1 || k >= counter) {
      printf("Invalid number of clusters!");
    }

    for (int i = 0; i < line_count; i++) {
        printf("Line %d: %f\n", i + 1, vectors[i][0]);
    }

    for (int i = 0; i < line_count; i++) {
        free(vectors[i]);
    }
    free(vectors);

    fclose(fp);
    return 0;








}

