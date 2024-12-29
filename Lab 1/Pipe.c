 #include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pipe_fd[2]; // File descriptors for the pipe
    pipe(pipe_fd); // Create a pipe

    switch (fork())
    {
    case -1: // error
        perror("Fork did not work");
        break;

    case 0: // child process
        // Close unused read end
        close(pipe_fd[0]);

        // Redirect output to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        // Execute "ls /" in the child process
        execlp("ls", "ls", "/", NULL);
        break;

    default: // parent process
        // Close unused write end
        close(pipe_fd[1]);
        // Redirect input to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        // Execute "wc -l" in the parent process
        execlp("wc", "wc", "-l", NULL);
        break;
    }

    return 0;
}