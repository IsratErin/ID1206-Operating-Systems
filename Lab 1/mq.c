#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>
int main(int argc, char *argv[]){

    char *input_file = argv[1];
    //char message[MAX_MSG_SIZE];
    int MAX_SIZE = 100;
    int MAX_NUM_MSG = 10;
    char *my_mq = "/mymq";
    char buf[MAX_SIZE];
    mqd_t mqd;

    struct mq_attr attr;
    // Form the queue attributes
    attr.mq_maxmsg = MAX_NUM_MSG;
    attr.mq_msgsize = MAX_SIZE;

    mqd = mq_open(my_mq, O_RDWR | O_CREAT, 0666, &attr);

    // Create a child process
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error \n");
        exit(1);
    }

     if (pid == 0) {
        // Child process (count words)
        mq_receive(mqd, buf, MAX_SIZE, NULL);
        char *token = strtok(buf, " \t\n");
        int word_count = 0;

        while (token != NULL) {
            word_count++;
            token = strtok(NULL, " \t\n");
        }

        printf("Number of words in the file: %d\n",word_count);

        mq_close(mqd);
    } else {
        // Parent process (read file and send content to child)
        FILE *file = fopen(input_file, "r");
        if (file == NULL) {
            perror("fopen");
            return 1;
        }

        while (fgets(buf, MAX_SIZE, file) != NULL) {
            mq_send(mqd, buf, strlen(buf), 0);
        }

        fclose(file);
        mq_close(mqd);
        
    }
    int status;
    waitpid(pid, &status, 0);
    
    // Unlink the message queue
    mq_unlink(my_mq);

    return 0;
}