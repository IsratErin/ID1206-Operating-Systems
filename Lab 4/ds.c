#include <stdio.h>
#include <stdlib.h>

#define CYLINDERS 5000
#define REQUESTS 100

// Function prototypes
int my_abs(int a, int b);

// Disk scheduling algorithms
void fcfs(int initial, int requests[]);
void sstf(int initial, int requests[]);
void scan(int initial, int requests[]);
void cscan(int initial, int requests[]);
void look(int initial, int requests[]);
void clook(int initial, int requests[]);



int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        printf("Usage: %s <initial_head_position>\n", argv[0]);
        return 1;
    }
    int initial = atoi(argv[1]);
    
    // Generate random cylinder requests
   int requests[REQUESTS];
    for (int i = 0; i < REQUESTS; i++) {
        requests[i] = rand()% CYLINDERS;
    }
 
    fcfs(initial, requests);
    sstf(initial, requests);
    scan(initial, requests);
    cscan(initial, requests);
    look(initial, requests);
    clook(initial, requests);
    

    return 0;
}

 void sort(int array[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Swap elements if out of order
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// FCFS (First-Come-First-Serve) Disk Scheduling Algorithm
void fcfs(int initial, int requests[]) {
    int totalHeadMovement = 0;

    // Iterate through the requests in the order they are received
    for (int i = 0; i < REQUESTS; i++) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    
    printf("FCFS: Total head movement = %d\n", totalHeadMovement);
}

// SSTF (Shortest Seek Time First) Disk Scheduling Algorithm
void sstf(int initial, int requests[]) {
    int totalHeadMovement = 0;
    int visited[REQUESTS] = {0}; 

    for (int i = 0; i < REQUESTS; i++) {
        int minDistance = CYLINDERS + 1;
        int minIndex = -1;

        // Find the request with the shortest seek time
        for (int j = 0; j < REQUESTS; j++) {
            if (!visited[j]) {
                int distance = my_abs(initial, requests[j]);
                if (distance < minDistance) {
                    minDistance = distance;
                    minIndex = j;
                }
            }
        }
    
        visited[minIndex] = 1;
        totalHeadMovement += minDistance;
        initial = requests[minIndex];
    }
  
    printf("SSTF: Total head movement = %d\n", totalHeadMovement);
}

void scan(int initial, int requests[]) {
    int totalHeadMovement = 0;

    sort(requests,REQUESTS);
     // Find the index where initial position 
    int index = 0;
    while (index < REQUESTS && requests[index] <= initial) {
        index++;
    }

    // SCAN to the right
    for (int i = index; i < REQUESTS; i++) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    // SCAN to the left
    for (int i = index - 1; i >= 0; i--) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    printf("SCAN: Total head movement = %d\n", totalHeadMovement);
}

void cscan(int initial, int requests[]) {
    int totalHeadMovement = 0;

    sort(requests,REQUESTS);

   
    int index = 0;
    while (index < REQUESTS && requests[index] <= initial) {
        index++;
    }

    // C-SCAN to the right
    for (int i = index; i < REQUESTS; i++) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    // Move to the beginning and continue C-SCAN to the right   
    totalHeadMovement += my_abs(initial, CYLINDERS-1);
    initial = 0;
    totalHeadMovement += my_abs(CYLINDERS-1,initial);    
    for (int i = 0; i < index; i++) {
      totalHeadMovement += my_abs(initial, requests[i]);
       initial = requests[i];
    }
    printf("C-SCAN: Total head movement = %d\n", totalHeadMovement);
}

void look(int initial, int requests[]) {
    int totalHeadMovement = 0;

    sort(requests,REQUESTS);
    int index = 0;
    while (index < REQUESTS && requests[index] <= initial) {
        index++;
    }

    // LOOK to the right
    for (int i = index; i < REQUESTS; i++) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    // LOOK to the left
    for (int i = index - 1; i >= 0; i--) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }

    printf("LOOK: Total head movement = %d\n", totalHeadMovement);
}

void clook(int initial, int requests[]) {
    int totalHeadMovement = 0;

   sort(requests,REQUESTS);
    int index = 0;
    while (index < REQUESTS && requests[index] <= initial) {
        index++;
    }

    // C-LOOK to the right
    for (int i = index; i < REQUESTS; i++) {
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }
    
    // Move to the beginning and continue C-LOOK to the right
    for (int i = 0; i < index; i++) {      
        totalHeadMovement += my_abs(initial, requests[i]);
        initial = requests[i];
    }
    printf("C-LOOK: Total head movement = %d\n", totalHeadMovement);
}
int my_abs(int a, int b) {
    return a > b ? a - b : b - a;
}