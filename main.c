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

double distance(double* vec1, double* vec2, int size) {
  double sum = 0;
  
  for (int i=0; i<size; i++) {
    sum += pow(vec1[i]-vec2[i], 2);
  }

  return sqrt(sum);
}



int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 5) {
      return 1;
    }
    int K = atoi(argv[1]);
    int N = atoi(argv[2]);
    int d = atoi(argv[3]);
    int iter = argc == 5 ? atoi(argv[4]) : 200;
    // printf("%d", K);
    // printf("%d", N);
    // printf("%d", d);
    // printf("%d", iter);
  	if (iter <= 1 || iter >= 1000) {
      printf("Invalid maximum iteration!");
    }
    
    if (K <=1 || K >= N) {
      printf("Invalid number of clusters!");
    }

    FILE *fp = stdin;
    char line[MAX_LINE_LENGTH];

    double **vectors = (double**)calloc(N, sizeof(double *));
    if (vectors == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
    }
    
    double **centroids = calloc((size_t)K, sizeof(double *));
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
        //     vectors = realloc(vectors, capacity * sizeof(double *));
        //     if (vectors == NULL) {
        //         fprintf(stderr, "Memory allocation failed\n");
        //         exit(1);
        //     }
        // }
        
        vectors[line_count] = calloc(d, sizeof(double));

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

    
    // Deep copy centroids
    for (int i=0; i<K; i++) {
      centroids[i] = calloc(d, sizeof(double));
      
      for (int j=0; j<d; j++) {
        centroids[i][j] = vectors[i][j];
      }
    }



    double max_distance = eps + 1; 
    int iter_couter = 0;
    while (max_distance >= eps && iter_couter < iter) {
      max_distance = 0;
      double **centroids_sum = (double**)calloc(K, sizeof(double *));
      double *counters = (double*)calloc(K, sizeof(double));

      for (int j=0; j<K; j++) {
        centroids_sum[j] = calloc(d, sizeof(double));
      }

      for (int i=0; i<N; i++) {
        double min_distance = distance(vectors[i], centroids[0], d);
        int min_j = 0;
        for (int j=0; j<K; j++) {
          double distance_from_centroid = distance(vectors[i], centroids[j], d);
          if (min_distance > distance_from_centroid) {
            min_distance = distance_from_centroid;
            min_j = j;
          }
        }
        counters[min_j]++;
        // centroids_sum[j]+=vectors[i];
        for (int p=0; p<d; p++) {
          centroids_sum[min_j][p]+=vectors[i][p];
        } 
      }

      for (int j=0; j<K; j++) {
        if (counters[j] > 0) {
          double* new_centroid_j = calloc(d, sizeof(double));
          for (int p=0; p<d; p++) {
            new_centroid_j[p] = centroids_sum[j][p] / counters[j];
          }
          double centroids_distance = distance(centroids[j], new_centroid_j, d);
          // printf("centroids distance in itertaion %d is: %f \n", j, counters[j]);
          if (centroids_distance > max_distance) {
            max_distance = centroids_distance;
          }
          // free(centroids[j]);
          for (int p=0; p<d; p++) {
            centroids[j][p] = new_centroid_j[p];
          }
          free(new_centroid_j);
        }
        // centroids[j] = new_centroid_j;
      }

      // Free memory
      free(centroids_sum);
      free(counters);

      iter_couter++;
    }

    for (int i=0; i<K; i++) {
        for (int j=0; j<d; j++) {
          printf("%.4f",centroids[i][j]);
          if (j != d-1)
            printf(",");
          else
            printf("\n");
        }
    }

    for (int i = 0; i < line_count; i++) {
        free(vectors[i]);
    }
    free(vectors);
    for (int i=0; i<K; i++) {
      free(centroids[i]);
    }
    free(centroids);
    // free(line);
    fclose(fp);
    return 0;
}

