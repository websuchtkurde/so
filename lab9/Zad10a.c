#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define FILE_PATH "random_numbers.dat"
#define CHUNK_SIZE 10

void generate_and_write_numbers(int file_descriptor, int num_numbers) {
    for (int i = 0; i < num_numbers; ++i) {
        int random_number = rand();
        write(file_descriptor, &random_number, sizeof(int));
    }
}

int main() {
    int file_descriptor = open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_descriptor == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < 10; ++i) {  // Generate and write 10 chunks of random numbers
        generate_and_write_numbers(file_descriptor, CHUNK_SIZE);
        sleep(1);  // Simulate some delay before the next chunk
    }

    close(file_descriptor);

    return 0;
}

