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

void *consumer(void *arg) {
    // Consume items from the table
    while (1) {
        // Wait if the buffer is empty
        sem_wait(&shared_memory->full);
        sem_wait(&shared_memory->mutex);

        // Take item from the table
        int item = shared_memory->buffer[shared_memory->out];
        shared_memory->out = (shared_memory->out + 1) % BUFFER_SIZE;

        printf("Consumed: %d\n", item);

        sem_post(&shared_memory->mutex);
        sem_post(&shared_memory->empty);

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

    // Create consumer thread
    pthread_t consumer_thread;
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Join thread
    pthread_join(consumer_thread, NULL);

    return 0;
}

