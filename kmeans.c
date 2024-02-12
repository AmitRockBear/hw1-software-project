#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define eps 0.0001
#define MAX_LINE_LENGTH 1024

double calculate_distance(double* vec1, double* vec2, int size) {
  int i;
  double sum;
  
  sum = 0;
  for (i=0; i<size; i++) {
    sum += pow(vec1[i]-vec2[i], 2);
  }

  return sqrt(sum);
}

double** input_file_to_matrix(FILE* fp, int rows, int columns) {
  double **vectors;
  int line_count, tokensCounter;
  char line[MAX_LINE_LENGTH], *token;
  size_t len;

  vectors = (double**)calloc(rows, sizeof(double *));
  if (vectors == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  line_count = 0;
  len = 0;
  while (getline(&line, &len, fp) != -1 && line_count < rows) {
      line[strcspn(line, "\n")] = '\0';

      vectors[line_count] = calloc(columns, sizeof(double));
      if (vectors[line_count] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }

      token = strtok(line, ",");
      tokensCounter = 0;      
      while (token != NULL) {
          vectors[line_count][tokensCounter] = strtod(token, NULL);
          
          token = strtok(NULL, ",");
          tokensCounter++;
      }
      line_count++;
  }

  return vectors;
}

double** deep_copy_matrix(double** copied_matrix, int rows, int columns) {
  int i, j;
  double **new_matrix;

  new_matrix = calloc((size_t)rows, sizeof(double *));
  if (new_matrix == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  for (i=0; i<rows; i++) {
      new_matrix[i] = calloc(columns, sizeof(double));
      if (new_matrix[i] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }

      for (j=0; j<columns; j++) {
        new_matrix[i][j] = copied_matrix[i][j];
      }
  }

  return new_matrix;
}

int find_closest_centroid_to_vector_index(double* vector, int vector_size, double** centroids, int centroids_num) {
  int j, min_j;
  double min_distance, distance_from_centroid;

  min_j = 0;
  min_distance = calculate_distance(vector, centroids[0], vector_size);

  for (j=0; j<centroids_num; j++) {
    distance_from_centroid = calculate_distance(vector, centroids[j], vector_size);
    if (min_distance > distance_from_centroid) {
      min_distance = distance_from_centroid;
      min_j = j;
    }
  }

  return min_j;
}

double* create_new_centroid(double* centroid_sum, int centroid_counter, int centroid_size) {
  int p;
  double *new_centroid;

  new_centroid = calloc(centroid_size, sizeof(double));
  if (new_centroid == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  for (p=0; p<centroid_size; p++) {
    new_centroid[p] = centroid_sum[p] / centroid_counter;
  }

  return new_centroid;
}

void calculate_centroids_convergence(double** centroids, double** vectors, int centroids_num, int centroid_size, int vectors_num, int max_iterations) {
  int iter_couter, closest_centroid_index, i, j, p;
  double max_distance, centroids_distance, **centroids_sum, *counters, *new_centroid_j;
  max_distance = eps + 1;
  iter_couter = 0;

  while (max_distance >= eps && iter_couter < max_iterations) {
    max_distance = 0;
    centroids_sum = (double**)calloc(centroids_num, sizeof(double *));
    if (centroids_sum == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }
    counters = (double*)calloc(centroids_num, sizeof(double));
    if (counters == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }

    for (j=0; j<centroids_num; j++) {
      centroids_sum[j] = calloc(centroid_size, sizeof(double));

      if (centroids_sum[j] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }
    }

    for (i=0; i<vectors_num; i++) {
      closest_centroid_index = find_closest_centroid_to_vector_index(vectors[i], centroid_size, centroids, centroids_num);
      counters[closest_centroid_index]++;

      for (p=0; p<centroid_size; p++) {
        centroids_sum[closest_centroid_index][p]+=vectors[i][p];
      } 
    }

    for (j=0; j<centroids_num; j++) {
      if (counters[j] > 0) {
        new_centroid_j = create_new_centroid(centroids_sum[j], counters[j], centroid_size);
        centroids_distance = calculate_distance(centroids[j], new_centroid_j, centroid_size);
        if (centroids_distance > max_distance) {
          max_distance = centroids_distance;
        }
        free(centroids[j]);
        centroids[j]= new_centroid_j;
      }
    }

    free(centroids_sum);
    free(counters);

    iter_couter++;
  }
}

void print_output(double** centroids, int centroids_num, int centroid_size) {
  int i, j;

  for (i=0; i<centroids_num; i++) {
    for (j=0; j<centroid_size; j++) {
      printf("%.4f",centroids[i][j]);
      if (j != centroid_size-1)
        printf(",");
      else
        printf("\n");
    }
  }
}

int isInteger(const char *str) {
    if (*str == '\0')
        return 0;

    while (*str != '\0') {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int K, N, d, iter, i;
    double **vectors, **centroids;
    FILE *fp;

    if (argc != 4 && argc != 5) {
      printf("An Error Has Occurred");
      exit(1);
    }

    if (argc == 5)

    K = atoi(argv[1]);
    N = atoi(argv[2]);
    d = atoi(argv[3]);
    iter = argc == 5 ? atoi(argv[4]) : 200;

  	if (iter <= 1 || iter >= 1000) {
      printf("Invalid maximum iteration!");
      exit(1);
    }

    if (N <= 1) {
      printf("Invalid number of points!");
      exit(1);
    }

    if (K <=1 || K >= N) {
      printf("Invalid number of clusters!");
      exit(1);
    }

    if (d < 1) {
      printf("Invalid dimension of point!");
      exit(1);
    }

    fp = stdin;
    if (fp == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }

    vectors = input_file_to_matrix(fp, N, d);
    centroids = deep_copy_matrix(vectors, K, d);

    calculate_centroids_convergence(centroids, vectors, K, d, N, iter);

    print_output(centroids, K, d);

    for (i=0; i<N; i++) {
        free(vectors[i]);
    }
    free(vectors);
    for (i=0; i<K; i++) {
      free(centroids[i]);
    }
    free(centroids);
    fclose(fp);

    return 0;
}

