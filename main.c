#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define eps 0.0001
#define MAX_LINE_LENGTH 1024

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
    char **text = NULL;
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
            text = realloc(text, capacity * sizeof(char *));
            if (text == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }
        
        text[line_count] = malloc(strlen(line) + 1);
        if (text[line_count] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        strcpy(text[line_count], line);

        line_count++;
    }

    if (k <=1 || k >= counter) {
      printf("Invalid number of clusters!");
    }

    for (int i = 0; i < line_count; i++) {
        printf("Line %d: %s\n", i + 1, text[i]);
    }

    for (int i = 0; i < line_count; i++) {
        free(text[i]);
    }
    free(text);

    fclose(fp);
    return 0;
}

