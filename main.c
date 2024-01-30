#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define eps 0.0001
#define MAX_LINE_LENGTH 1024

double calculate_distance(double* vec1, double* vec2, int size) {
  double sum = 0;
  
  for (int i=0; i<size; i++) {
    sum += pow(vec1[i]-vec2[i], 2);
  }

  return sqrt(sum);
}
double** input_file_to_matrix(FILE* fp, int rows, int columns) {
  double** vectors = (double**)calloc(rows, sizeof(double *));
  if (vectors == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  int line_count = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), fp) != NULL && line_count < rows) {
      line[strcspn(line, "\n")] = '\0';

      vectors[line_count] = calloc(columns, sizeof(double));
      if (vectors[line_count] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }

      char *token = strtok(line, ",");
      int tokensCounter = 0;        
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
  double** new_matrix = calloc((size_t)rows, sizeof(double *));
  if (new_matrix == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  for (int i=0; i<rows; i++) {
      new_matrix[i] = calloc(columns, sizeof(double));
      if (new_matrix[i] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }

      for (int j=0; j<columns; j++) {
        new_matrix[i][j] = copied_matrix[i][j];
      }
  }

  return new_matrix;
}

int find_closest_centroid_to_vector_index(double* vector, int vector_size, double** centroids, int centroids_num) {
  double min_distance = calculate_distance(vector, centroids[0], vector_size);
  int min_j = 0;

  for (int j=0; j<centroids_num; j++) {
    double distance_from_centroid = calculate_distance(vector, centroids[j], vector_size);
    if (min_distance > distance_from_centroid) {
      min_distance = distance_from_centroid;
      min_j = j;
    }
  }

  return min_j;
}

double* create_new_centroid(double* centroid_sum, int centroid_counter, int centroid_size) {
  double* new_centroid = calloc(centroid_size, sizeof(double));
  if (new_centroid == NULL) {
      printf("An Error Has Occurred");
      exit(1);
  }

  for (int p=0; p<centroid_size; p++) {
    new_centroid[p] = centroid_sum[p] / centroid_counter;
  }

  return new_centroid;
}

void calculate_centroids_convergence(double** centroids, double** vectors, int centroids_num, int centroid_size, int vectors_num, int max_iterations) {
  double max_distance = eps + 1; 
  int iter_couter = 0;
  while (max_distance >= eps && iter_couter < max_iterations) {
    max_distance = 0;
    double **centroids_sum = (double**)calloc(centroids_num, sizeof(double *));
    if (centroids_sum == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }
    double *counters = (double*)calloc(centroids_num, sizeof(double));
    if (counters == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }

    for (int j=0; j<centroids_num; j++) {
      centroids_sum[j] = calloc(centroid_size, sizeof(double));

      if (centroids_sum[j] == NULL) {
        printf("An Error Has Occurred");
        exit(1);
      }
    }

    for (int i=0; i<vectors_num; i++) {
      int closest_centroid_index = find_closest_centroid_to_vector_index(vectors[i], centroid_size, centroids, centroids_num);
      counters[closest_centroid_index]++;

      for (int p=0; p<centroid_size; p++) {
        centroids_sum[closest_centroid_index][p]+=vectors[i][p];
      } 
    }

    for (int j=0; j<centroids_num; j++) {
      if (counters[j] > 0) {
        double* new_centroid_j = create_new_centroid(centroids_sum[j], counters[j], centroid_size);
        double centroids_distance = calculate_distance(centroids[j], new_centroid_j, centroid_size);
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
  for (int i=0; i<centroids_num; i++) {
    for (int j=0; j<centroid_size; j++) {
      printf("%.4f",centroids[i][j]);
      if (j != centroid_size-1)
        printf(",");
      else
        printf("\n");
    }
  }
}

int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 5) {
      printf("An Error Has Occurred");
      exit(1);
    }
    int K = atoi(argv[1]);
    int N = atoi(argv[2]);
    int d = atoi(argv[3]);
    int iter = argc == 5 ? atoi(argv[4]) : 200;

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

    FILE *fp = stdin;
    if (fp == NULL) {
      printf("An Error Has Occurred");
      exit(1);
    }

    double** vectors = input_file_to_matrix(fp, N, d);
    double** centroids = deep_copy_matrix(vectors, K, d);

    calculate_centroids_convergence(centroids, vectors, K, d, N, iter);

    print_output(centroids, K, d);

    for (int i = 0; i < N; i++) {
        free(vectors[i]);
    }
    free(vectors);
    for (int i=0; i<K; i++) {
      free(centroids[i]);
    }
    free(centroids);
    fclose(fp);

    return 0;
}

