#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

float distance(float* vec1, float* vec2) {
  float sum = 0;
  
  for (int i=0; i<sizeof(vec1); i++) {
    sum += pow(vec1[i]-vec2[i], 2);
  }

  return sqrt(sum);
}

int main(int argc, char* argv[]) {
  printf("Hello there");
    if (argc != 4 && argc != 5) {
      return 1;
    }
    int K = atoi(argv[1]);
    int N = atoi(argv[2]);
    int d = atoi(argv[3]);
    int iter = argc == 5 ? atoi(argv[4]) : 200;

  	if (iter <= 1 || iter >= 1000) {
      printf("Invalid maximum iteration!");
    }

    char *arrayOfStrings[1];
    int counter = 0;

    FILE *fp = stdin;
    char line[MAX_LINE_LENGTH];

    float **vectors = (float**)calloc(N, sizeof(float *));
    if (vectors == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
    }
    
    float **centroids = calloc((size_t)K, sizeof(float *));
    if (centroids == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
    }

    // int capacity = 0;
    int line_count = 0;

    if (fp == NULL) {
      perror("Error opening input");
      exit(1);
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        // if (line_count == capacity) {
        //     capacity += 50;
        //     vectors = realloc(vectors, capacity * sizeof(float *));
        //     if (vectors == NULL) {
        //         fprintf(stderr, "Memory allocation failed\n");
        //         exit(1);
        //     }
        // }
        
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

    if (K <=1 || K >= counter) {
      printf("Invalid number of clusters!");
    }

    for (int i = 0; i < line_count; i++) {
        printf("Line %d: %f\n", i + 1, vectors[i][0]);
    }
    
    // Deep copy centroids
    for (int i=0; i<K; i++) {
      size_t d = sizeof(vectors[i]);
      centroids[i] = calloc(d, sizeof(float));
      
      for (int j=0; j<d; j++) {
        centroids[i][j] = vectors[i][j];
      }
    }

    // for (int i = 0; i < k; i++) {
    //     printf("Line %d: %f\n", i + 1, centroids[i][0]);
    // }

    for(int i=0; i<sizeof(vectors); i++) {
      float min_distance = distance(vectors[0], centroids[0]);
      for (int j=1; j<K; j++) {
        float distance_from_centroid = distance(vectors[i], centroids[j]);
        if (min_distance > distance_from_centroid) {
          min_distance = distance_from_centroid;
        }
      
      
      }
    }














    for (int i = 0; i < line_count; i++) {
        free(vectors[i]);
    }
    free(vectors);

    fclose(fp);
    return 0;




}

