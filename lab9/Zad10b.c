#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_PATH "random_numbers.dat"
#define CHUNK_SIZE 10

void read_and_find_max(int file_descriptor, int num_numbers) {
    int* buffer = (int*)malloc(num_numbers * sizeof(int));
    if (buffer == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    read(file_descriptor, buffer, num_numbers * sizeof(int));

    int max_value = buffer[0];
    for (int i = 1; i < num_numbers; ++i) {
        if (buffer[i] > max_value) {
            max_value = buffer[i];
        }
    }

    printf("Max value in the chunk: %d\n", max_value);

    free(buffer);
}

int main() {
    int file_descriptor = open(FILE_PATH, O_RDONLY);
    if (file_descriptor == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 10; ++i) {  // Read and process 10 chunks of random numbers
        read_and_find_max(file_descriptor, CHUNK_SIZE);
        sleep(1);  // Simulate some delay before the next chunk
    }

    close(file_descriptor);

    return 0;
}

