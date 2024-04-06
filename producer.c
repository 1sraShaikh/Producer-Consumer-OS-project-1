#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define BUFFER_SIZE 2

// Shared memory structure
struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    sem_t mutex, full, empty;
} *shared_memory;

void *producer(void *arg) {
    // Produce items and put them onto the table
    while (1) {
        // Produce item
        int item = rand() % 100;

        // Wait if the buffer is full
        sem_wait(&shared_memory->empty);
        sem_wait(&shared_memory->mutex);

        // Put item onto the table
        shared_memory->buffer[shared_memory->in] = item;
        shared_memory->in = (shared_memory->in + 1) % BUFFER_SIZE;

        printf("Produced: %d\n", item);

        sem_post(&shared_memory->mutex);
        sem_post(&shared_memory->full);

        // Sleep for some time
        sleep(rand() % 3);
    }
}

int main() {
    // Allocate shared memory
    int shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(*shared_memory));
    shared_memory = mmap(0, sizeof(*shared_memory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize semaphores
    sem_init(&shared_memory->mutex, 1, 1);
    sem_init(&shared_memory->full, 1, 0);
    sem_init(&shared_memory->empty, 1, BUFFER_SIZE);

    // Create producer thread
    pthread_t producer_thread;
    pthread_create(&producer_thread, NULL, producer, NULL);

    // Join thread
    pthread_join(producer_thread, NULL);

    return 0;
}

