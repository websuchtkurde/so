#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define FILE_PATH "output_file.txt"
#define CHUNK_SIZE 4096

void write_data(int file_descriptor, const char* data, size_t size) {
    write(file_descriptor, data, size);
}

void write_with_fsync(int file_descriptor, const char* data, size_t size) {
    write_data(file_descriptor, data, size);
    fsync(file_descriptor);
}

void write_with_fdatasync(int file_descriptor, const char* data, size_t size) {
    write_data(file_descriptor, data, size);
    fdatasync(file_descriptor);
}

void measure_time(int file_descriptor, const char* data, size_t size, const char* description) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Choose the appropriate write function based on the description
    if (strcmp(description, "fsync") == 0) {
        write_with_fsync(file_descriptor, data, size);
    } else if (strcmp(description, "fdatasync") == 0) {
        write_with_fdatasync(file_descriptor, data, size);
    } else {
        write_data(file_descriptor, data, size);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    long long elapsed_time_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    printf("%s: Time elapsed: %lld ns\n", description, elapsed_time_ns);
}

int main() {
    int file_descriptor = open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (file_descriptor == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char data[CHUNK_SIZE];
    memset(data, 'A', CHUNK_SIZE); // Fill the data with 'A' characters

    for (int i = 0; i < 3; ++i) {
        measure_time(file_descriptor, data, CHUNK_SIZE, "Without Sync");
        measure_time(file_descriptor, data, CHUNK_SIZE, "fsync");
        measure_time(file_descriptor, data, CHUNK_SIZE, "fdatasync");
        lseek(file_descriptor, 0, SEEK_SET); // Reset file position for the next iteration
    }

    close(file_descriptor);

    return 0;
}

