#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define PORT 22777

int receiver(void) {
    int recv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;

    // 1. Luodaan UDP-socket
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (recv_sock == -1) {
        perror("socket() virhe");
        exit(EXIT_FAILURE);
    }

    // 2. Asetetaan osoiterakenne (local address)
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // Otetaan vastaan viestejä miltä tahansa käyttöliittymältä
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    // 3. Sidotaan socket porttiin
    if (bind(recv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() virhe");
        close(recv_sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP-vastaanottaja kuuntelee portissa %d...\n", PORT);

    // 4. Kuunnellaan ja otetaan vastaan viestejä
    while (1) {
        clnt_addr_size = sizeof(clnt_addr);

        // Vastaanotetaan datagrammi
        str_len = recvfrom(recv_sock, message, BUF_SIZE - 1, 0,
                           (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if (str_len == -1) {
            perror("recvfrom() virhe");
            continue;
        }

        // Lisätään null-terminaattori
        message[str_len] = '\0';

        // Tulostetaan viesti ja sen lähde
        printf("[%s:%d] %s\n",
               inet_ntoa(clnt_addr.sin_addr),
               ntohs(clnt_addr.sin_port),
               message);

        // **TÄMÄ ON "KIRJOITA TOISEEN PUTKEEN" -VAIHE:**
        // Tässä esimerkissä "toinen putki" on standardituloste (stdout).
        // Jos haluaisit kirjoittaa toiseen UDP-putkeen, käyttäisit
        // tässä kohdassa erillistä sendto() -kutsua *uuteen* kohdeosoitteeseen.

    }

    // Ei päädytä tänne (ikuinen silmukka)
    close(recv_sock);
    return 0;
}
