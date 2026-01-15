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

int main(int argc, char *argv[]) {

	int opt;
	char *ip = "127.0.0.1";
	int port = 22777;
	char *tty = "/dev/pts/3";

	while ((opt = getopt(argc, argv, "i:p:t:")) != -1) {
			switch (opt) {
				case 'i':
					ip = optarg;
					break;
				case 'p':
					port = atoi(optarg);
					break;
				case 't':
					tty = optarg;
					break;
				default:
					fprintf(stderr, "Usage: %s [-i ipaddr] [-p UDP port] [-t UART]\n", argv[0]);
					exit(EXIT_FAILURE);
			}
		}

    printf("Call receiver \n");
    receiver(ip, port, tty);

    return 0;
}
