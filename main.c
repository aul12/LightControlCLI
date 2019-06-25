#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "RadioControlProtocolC/rc_lib.h"

int main(int argc, const char* argv[]) {
    if (argc <= 4) {
        fprintf(stderr, "usage: LedStrip cmd r g b [ip]\n");
        exit(1);
    }
	const char* server_name = argc == 5 ? "192.168.2.200" : argv[5];
	const int server_port = 1337;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// creates binary representation of server name
	// and stores it as sin_addr
	// http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(server_port);

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	// TCP is connection oriented, a reliable connection
	// **must** be established before any data is exchanged
	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("could not connect to server\n");
		return 1;
	}

	// data that will be sent to the server
    rc_lib_package_t pkg;
    rc_lib_init_tx(&pkg, 1024, 4);
    pkg.channel_data[0] = atoi(argv[1]);
    pkg.channel_data[1] = atoi(argv[2]);
    pkg.channel_data[2] = atoi(argv[3]);
    pkg.channel_data[3] = atoi(argv[4]);
    int len = rc_lib_encode(&pkg);

    ssize_t sent = 0;
	do {
        ssize_t tmp = send(sock, pkg.buffer, len, 0);
        if (tmp < 0) {
            fprintf(stderr, "Error sending: %s\n", strerror(errno));
            exit(1);
        } else {
            sent += tmp;
        }
    } while (sent < len);

	// close the socket
	close(sock);
	return 0;
}
