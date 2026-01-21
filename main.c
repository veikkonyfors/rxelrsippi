/*
 * main.c
 *
 *  Created on: Dec 3, 2025
 *      Author: pappa
 */

#include <stdio.h>
#include "receiver.h"
#include <unistd.h>
#include <stdlib.h>

/**
 * @file main.c
 *
 * Created on: Jan 6, 2026
 * Author: pappa
 *
 * @brief command line program to start receiver for crsf packaged data from given UDP port for writing it to given UART port on FC
 * @param -p UDP port from where to read (default 22777).
 * @param -t UART port to write (default /dev/pts/3).
 * @return 0.
 * @note To be run on RPI connected to drone's FC UART.
 * @note Never ending loop to be terminated with SIGINT or SIGKILL.
 * @note Receiver accepts UDP connection from any IP address.
 */
int main(int argc, char *argv[]) {

	int opt;
	int port = 22777;
	char *tty = "/tmp/ttyV0";

	while ((opt = getopt(argc, argv, "p:t:")) != -1) {
			switch (opt) {
				case 'p':
					port = atoi(optarg);
					break;
				case 't':
					tty = optarg;
					break;
				default:
					fprintf(stderr, "Usage: %s [-p UDP port] [-t UART]\n", argv[0]);
					exit(EXIT_FAILURE);
			}
		}

    printf("Call receiver on %d, %s\n", port, tty);
    receiver(port, tty);

    return 0;
}
