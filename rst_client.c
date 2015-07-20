#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 5001
#define ADDR "212.201.121.85"

int
main(int argc, char *argv[])
{
	int fd;
	char c;
#if 1
	struct sctp_reset_streams srs;
#else
	sctp_assoc_t assoc_id;
#endif
	struct sctp_udpencaps udpencaps;
	struct sockaddr_in addr;

	if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
		perror("socket");
	}
	memset(&udpencaps, 0, sizeof(struct sctp_udpencaps));
	udpencaps.sue_address.ss_family=AF_INET;
	udpencaps.sue_port = htons(9899);
	if (setsockopt(fd, IPPROTO_SCTP, SCTP_REMOTE_UDP_ENCAPS_PORT, (void *)&udpencaps, sizeof(struct sctp_udpencaps)) < 0) {
		perror("setsockopt");
	}
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_len = sizeof(struct sockaddr_in);
	addr.sin_addr.s_addr = inet_addr(ADDR);
	addr.sin_port = htons(PORT);
	if (connect(fd, (const struct sockaddr *) &addr, sizeof(struct sockaddr_in)) != 0) {
		perror("connect");
	}
	c = 'A';
	if (send(fd, (const void *)&c, 1, 0) < 0) {
		perror("send 1");
	}
	c = 'B';
	if (send(fd, (const void *)&c, 1, 0) < 0) {
		perror("send 2");
	}
#if 1
	memset(&srs, 0, sizeof(struct sctp_reset_streams));
	srs.srs_flags = SCTP_STREAM_RESET_OUTGOING;
	srs.srs_number_streams = 0;
	if (setsockopt(fd, IPPROTO_SCTP, SCTP_RESET_STREAMS, &srs, (socklen_t)sizeof(struct sctp_reset_streams)) < 0) {
		perror("setsockopt SCTP_RESET_STREAMS");
	}
#else
	if (setsockopt(fd, IPPROTO_SCTP, SCTP_RESET_ASSOC, &assoc_id, (socklen_t)sizeof(sctp_assoc_t)) < 0) {
		perror("setsockopt SCTP_RESET_ASSOC");
	}
#endif
	sleep(10);
	c = 'C';
        if (send(fd, (const void *)&c, 1, 0) < 0) {
                perror("send 3");
        }
	if (close(fd) < 0) {
		perror("close");
	}
	return (0);
}
