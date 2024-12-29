#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> 

#define NUM_THREADS 3

int buffer = 0;  // Shared buffer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for synchronization
/* */
// Thread function
void *thread_function(void *arg) {
    int *tid = (int *)arg;

    int *changes = malloc(sizeof(int));  
     while (1)
    {
    
        // Lock the mutex to ensure exclusive access to the shared buffer 
        pthread_mutex_lock(&mutex);
         if (buffer < 15)
        {
         
        // Print thread ID, process ID, and current buffer value
        printf("TID: %lu, PID: %ld, Buffer: %d\n", (unsigned long)pthread_self(), (long)getpid(), buffer);
        buffer++;
        (*changes)++;
        
        //Unlock the mutex to release the exclusive access 
        pthread_mutex_unlock(&mutex);
        sleep(0.01);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            pthread_exit(changes);  
        }
         
        }
     free(changes);
}




int main() {
    pthread_t threads[NUM_THREADS];
    int total_buffer_access= 0;
    void *changess[3];
    pthread_mutex_init(&mutex,NULL);
    // Create threads
    
    for (int i = 0; i < NUM_THREADS; i++) {

        if (pthread_create(&threads[i], NULL, thread_function, (void *)&threads[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        
    }
    
    // Wait for threads to finish and collect the number of changes each made
    for (int i = 0; i < NUM_THREADS; i++) { 
         if (pthread_join(threads[i], &changess[i]) != 0) {
             perror("pthread_join");
             exit(EXIT_FAILURE);
         }
     }
     
    for (int i = 2; i>=0 ; i--) {
        // Print the number of changes made by each thread
        printf("TID %lu worked on the buffer %d times\n", (unsigned long)threads[i], *((int *)changess[i]));

        total_buffer_access+= *((int *)changess[i]);
           
         
    }
    
    // Print the total number of buffer accesses by all threads
    printf("Total buffer accesses: %d\n", total_buffer_access);
    pthread_mutex_destroy(&mutex);
    
    return 0;
}