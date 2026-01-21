#ifndef RECEIVER_H_
#define RECEIVER_H_


/**
 * @file receiver.h
 *
 * Created on: Jan 6, 2026
 * Author: pappa
 *
 * @brief Reads crsf packaged data from given UDP port and writes it to given UART port on FC.
 * @param port UDP port from where to read (default 22777).
 * @param uart UART port to write (default /dev/pts/3).
 * @return 0.
 * @note To be run on RPI connected to drone's FC UART.
 * @note Never ending loop to be terminated with SIGINT or SIGKILL.
 * @note Receiver accepts UDP connection from any IP address.
 */
int receiver(int port, char *uart);

#endif /* RECEIVER_H_ */
