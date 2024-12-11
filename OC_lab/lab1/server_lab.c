#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

static char CLIENT_PROGRAM_NAME[] = "filter_str";

int main(int argc, char **argv) {
    if (argc == 1) {
        char msg[1024];
        uint32_t len = snprintf(msg, sizeof(msg) - 1, "usage: %s filename\n", argv[0]);
        write(STDERR_FILENO, msg, len);
        exit(EXIT_SUCCESS);
    }

    // NOTE: Get full path to the directory, where program resides
    char progpath[1024];
    {
        // NOTE: Read full program path, including its name
        ssize_t len = readlink("/proc/self/exe", progpath,
                               sizeof(progpath) - 1);
        if (len == -1) {
            const char msg[] = "error: failed to read full program path\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        }

        // NOTE: Trim the path to first slash from the end
        while (progpath[len] != '/')
            --len;

        progpath[len] = '\0';
    }

    // NOTE: Open pipe
    int channel[2];
    int channel2[2];
    if (pipe(channel) == -1) {
        const char msg[] = "error: failed to create pipe\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }
    if (pipe(channel2) == -1) {
        const char msg[] = "error: failed to create pipe\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    // NOTE: Spawn a new process
    const pid_t child1 = fork();
    const pid_t child2 = fork();
    switch (child1) {
        case -1: { // NOTE: Kernel fails to create another process
            const char msg[] = "error: failed to spawn new process\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        } break;

        case 0: { // NOTE: We're a child1, child1 doesn't know its pid after fork
            pid_t pid = getpid(); // NOTE: Get child1 PID

            // NOTE: Connect parent stdin to child1 stdin
            dup2(STDIN_FILENO, channel[STDIN_FILENO]);
            close(channel[STDOUT_FILENO]);

            {
                char msg[64];
                const int32_t length = snprintf(msg, sizeof(msg),
                                                "%d: I'm a child1\n", pid);
                write(STDOUT_FILENO, msg, length);
            }

            {
                char path[1024];
                snprintf(path, sizeof(path) - 1, "%s/%s", progpath, CLIENT_PROGRAM_NAME);

                // NOTE: args[0] must be a program name, next the actual arguments
                // NOTE: `NULL` at the end is mandatory, because `exec*`
                //       expects a NULL-terminated list of C-strings
                char *const args[] = {CLIENT_PROGRAM_NAME, argv[1], NULL};

                int32_t status = execv(path, args);

                if (status == -1) {
                    const char msg[] = "error: failed to exec into new exectuable image\n";
                    write(STDERR_FILENO, msg, sizeof(msg));
                    exit(EXIT_FAILURE);
                }
            }
        } break;

        default: { // NOTE: We're a parent, parent knows PID of child1 after fork
            pid_t pid = getpid(); // NOTE: Get parent PID

            {
                char msg[64];
                const int32_t length = snprintf(msg, sizeof(msg),
                                                "%d: I'm a parent, my child1 has PID %d\n", pid, child1);
                write(STDOUT_FILENO, msg, length);
            }

            // NOTE: `wait` blocks the parent until child1 exits
            int child_status;
            wait(&child_status);

            if (child_status != EXIT_SUCCESS) {
                const char msg[] = "error: child1 exited with error\n";
                write(STDERR_FILENO, msg, sizeof(msg));
                exit(child_status);
            }
        } break;
    }
    switch (child2) {
        case -1: { // NOTE: Kernel fails to create another process
            const char msg[] = "error: failed to spawn new process\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        } break;

        case 0: { // NOTE: We're a child2, child2 doesn't know its pid after fork
            pid_t pid = getpid(); // NOTE: Get child2 PID

            // NOTE: Connect parent stdin to child1 stdin
            dup2(STDIN_FILENO, channel[STDIN_FILENO]);
            close(channel[STDOUT_FILENO]);

            {
                char msg[64];
                const int32_t length = snprintf(msg, sizeof(msg),
                                                "%d: I'm a child1\n", pid);
                write(STDOUT_FILENO, msg, length);
            }

            {
                char path[1024];
                snprintf(path, sizeof(path) - 1, "%s/%s", progpath, CLIENT_PROGRAM_NAME);

                // NOTE: args[0] must be a program name, next the actual arguments
                // NOTE: `NULL` at the end is mandatory, because `exec*`
                //       expects a NULL-terminated list of C-strings
                char *const args[] = {CLIENT_PROGRAM_NAME, argv[1], NULL};

                int32_t status = execv(path, args);

                if (status == -1) {
                    const char msg[] = "error: failed to exec into new exectuable image\n";
                    write(STDERR_FILENO, msg, sizeof(msg));
                    exit(EXIT_FAILURE);
                }
            }
        } break;

        default: { // NOTE: We're a parent, parent knows PID of child1 after fork
            pid_t pid = getpid(); // NOTE: Get parent PID

            {
                char msg[64];
                const int32_t length = snprintf(msg, sizeof(msg),
                                                "%d: I'm a parent, my child1 has PID %d\n", pid, child1);
                write(STDOUT_FILENO, msg, length);
            }

            // NOTE: `wait` blocks the parent until child1 exits
            int child_status;
            wait(&child_status);

            if (child_status != EXIT_SUCCESS) {
                const char msg[] = "error: child1 exited with error\n";
                write(STDERR_FILENO, msg, sizeof(msg));
                exit(child_status);
            }
        } break;
    }
}


