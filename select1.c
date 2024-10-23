/*
Asking ChatGPT: write a C code that uses the select syscall
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <fcntl.h>

int main() {
	// File descriptor set
	fd_set readfds;

	// Time structure for timeout
	struct timeval timeout;

	// Initialize timeout: 10 seconds and 0 microseconds
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	// Clear the set and add file descriptor 0 (stdin) to the set
	FD_ZERO(&readfds);
	FD_SET(STDIN_FILENO, &readfds); // STDIN_FILENO is 0

	// Wait for input on stdin (file descriptor 0)
	printf("Waiting for input (10 seconds timeout)...\n");

	int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
	printf("result: %d\n", result);

	if (result == -1) {
		perror("select");
		exit(EXIT_FAILURE);
	} else if (result == 0) {
		printf("Timeout occurred! No data entered.\n");
	} else {
		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			char buffer[128];
			ssize_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

			if (bytes_read > 0) {
				buffer[bytes_read] = '\0'; // Null-terminate the string
				printf("You entered: %s", buffer);
			} else {
				perror("read");
			}
		}
	}

	return 0;
}

