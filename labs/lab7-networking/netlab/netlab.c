#include <stdio.h>
#include "netsim.h"

int timeoutID = -1;
int message_received = 0;

unsigned char compute_checksum(char *data, size_t len) {
	unsigned char checksum = 0;
	for (size_t i = 1; i < len; i++) {
		checksum ^= data[i];
	}
	return checksum;
}

void resend_request(void* _data) {
	if (message_received) {
		return;
	}
	char *data = (char*)_data;
	data[0] = compute_checksum(data, 5);
	printf("Resending request: %c\n", data[4]);
	send(5, data);
}

void recvd(size_t len, void* _data) {
	char *data = _data;
	unsigned char checksum = compute_checksum(data, len);
	if (checksum != data[0]) {
		return;
	}
	message_received = 1;
	clearTimeout(timeoutID);
	unsigned char seq_num = data[1];
	
	fwrite(data+3,1,len-3,stdout);
	fflush(stdout);
	
	char ack_data[5] = {'\0', 'A', 'C', 'K', seq_num};
	ack_data[0] = compute_checksum(ack_data, 5);
	send(5, ack_data);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s n\n    where n is a number between 0 and 3\n", argv[0]);
		return 1;
	}
	char data[5];
	data[1] = 'G'; data[2] = 'E'; data[3] = 'T'; data[4] = argv[1][0];
	data[0] = compute_checksum(data, 5);
	send(5, data);
	timeoutID = setTimeout(resend_request, 1500, data);
	waitForAllTimeoutsAndMessagesThenExit();
}
