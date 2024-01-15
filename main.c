#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define eps 0.001

// int main(int k, int iter, char* args[]) {
int main(int argc, char* argv[]) {
    if (argc != 3) {
      return 1;
    }
    int k = atoi(argv[1]);
    int iter = atoi(argv[2]);

  	if (iter <= 1 || iter >= 1000) {
      printf("Invalid maximum iteration!");
    }
    printf("[i]");
  	
    size_t bufferSize = 0;
    ssize_t bytesRead;
  	char **lines = NULL; 
    int counter = 0;
  	int maxBufferSize = 0;
    // Read input from stdin (standard input) using getline
    while (fgets(lines[counter], 25, stdin) != NULL) // when deploying to the nove check if the 0 should be -1
    {
      counter++;
    }
		
    if (k <=1 || k >= counter) {
      printf("Invalid number of clusters!");
    }
    
    for (int i=0;i<counter;i++) 
      printf(lines[i]);

    return 0;
}

// N = 2
// K = ?
// d = 3

// 8.1402,-5.8022,-7.2376
// 10.1626,-7.4824,-6.5774






