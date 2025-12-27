#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "crsf.h"

#define BUF_SIZE 64
#define PORT 22777

int receiver(void) {

	// Socket variables
    int recv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    uint8_t rx_data[BUF_SIZE] = {0}; // VN: if works without, remove = {0} for performance
    int str_len;

    // crsf variables
    //crsf_frame_union_t tx_frame;
    crsf_channels_t channels;

    // 1. reate UDP-socket
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (recv_sock == -1) {
        perror("socket() virhe");
        exit(EXIT_FAILURE);
    }


    // 2. setup local address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // Receive from any address
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // 3. bind socket
    if (bind(recv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() virhe");
        close(recv_sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP-vastaanottaja kuuntelee portissa %d...\n", PORT);


    // Set RC channels (example values)
    channels.ch1 = 1500;  // Throttle
    channels.ch2 = 1500;  // Roll
    channels.ch3 = 1500;  // Pitch
    channels.ch4 = 1500;  // Yaw
    channels.ch5 = 992;   // Arm switch
    // ... set other channels


    // 4. Listen and receive
    while (1) {
        clnt_addr_size = sizeof(clnt_addr);

        // Vastaanotetaan datagrammi
        str_len = recvfrom(recv_sock, rx_data, BUF_SIZE - 1, 0,
                           (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if (str_len == -1) {
            perror("recvfrom() error");
            continue;
        }

        if (crsf_is_valid_frame(rx_data, sizeof(rx_data))) {
            crsf_frame_type_e type = crsf_get_frame_type(rx_data);
            printf("Received frame type: 0x%02X\n", type);

            // Handle different frame types
            switch (type) {
                case CRSF_FRAMETYPE_LINK_STATISTICS:
                    // Process link stats
                    break;
                case CRSF_FRAMETYPE_GPS:
                    // Process GPS data
                    break;
                case CRSF_FRAMETYPE_RC_CHANNELS_PACKED: // 0X16
					{
						crsf_rc_channels_packed_t *rx_channels_packed = (crsf_rc_channels_packed_t *)rx_data;
						uint16_t *pchannels = (uint16_t *)&channels;

						// Process flight data
						crsf_unpack_channels(rx_channels_packed->data, pchannels);
						printf("\n\nRoll: %4d, Pitch: %4d Throttle: %4d, Yaw: %4d\n", channels.ch1, channels.ch2, channels.ch3, channels.ch4);
						printf("Disarm: %4d, Flight Mode: %4d, Buzzer: %4d, Blackbox log activation: %4d\n", channels.ch5, channels.ch6, channels.ch7, channels.ch8);
						printf("VTX Control: %4d, Pan: %4d, OSD Menu Navigation: %4d, RTH activation: %4d\n", channels.ch9, channels.ch10, channels.ch11, channels.ch12);
						printf("LED Strip Control: %4d, Script Control: %4d, Trainer Mode: %4d, Custom / Reserved: %4d\n", channels.ch13, channels.ch14, channels.ch15, channels.ch16);
					}
                    break;
                default:
                	printf("Unknown type 0x%02X\n", type);
                	printf("Received frame type: 0x%02X\n", type);

            }
        } else {

			// Lisätään null-terminaattori
			rx_data[str_len] = '\0';

			// Tulostetaan viesti ja sen lähde
			printf("[%s:%d] (noncrsf): %s\n",
				   inet_ntoa(clnt_addr.sin_addr),
				   ntohs(clnt_addr.sin_port),
				   rx_data);

			// **TÄMÄ ON "KIRJOITA TOISEEN PUTKEEN" -VAIHE:**
			// Tässä esimerkissä "toinen putki" on standardituloste (stdout).
			// Jos haluaisit kirjoittaa toiseen UDP-putkeen, käyttäisit
			// tässä kohdassa erillistä sendto() -kutsua *uuteen* kohdeosoitteeseen.
        }

    }

    // Ei päädytä tänne (ikuinen silmukka)
    close(recv_sock);
    return 0;
}
