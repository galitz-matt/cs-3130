#include "lab.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* forward function for "attack 0" case.
   change this code to implement your attack
 */
void forward_attack_0(struct message *message) {
	if (message->from == 'A' && message->to == 'B') {
		char new_data[1024];
		snprintf(new_data, sizeof(new_data), "PAY $10001438 TO M");
		strncpy((char * restrict)message->data, new_data, sizeof(message->data));
		message->data_size = strlen((const char*)message->data);
	}
	send_message(message);
}

/* forward function for "attack 1" case.
   change this code to implement your attack
 */
void forward_attack_1(struct message *message) {
	if (message->from == 'A' && message->to == 'B' && message->is_encrypted) {
		const char *new_payment_instr = "PAY $5000 TO M";
		struct message *new_msg = new_message('A', 'B', new_payment_instr, true, false);
		memcpy(message, new_msg, sizeof(struct message));
	}
	send_message(message);
}

/* forward function for "attack 2" case.
   change this code to implement your attack
 */
bool first_payment_sent = false;
void forward_attack_2(struct message *message) {
	if (message->from == 'A' && message->to == 'B' && message->is_encrypted && message->is_signed) {
		if (!first_payment_sent) {
			send_message(message);
			first_payment_sent = true;
		}
		send_message(message);
	} else {
		send_message(message);
	}
}

/* forward function for "attack 3" case.
   change this code to implement your attack
 */
static int payment_count = 0;
void forward_attack_3(struct message *message) {
	if (message->from == 'A' && message->to == 'B' && message->is_encrypted && message->is_signed) {
		payment_count++;
		send_message(message);
		if (payment_count == 2) {
			send_message(message);
		}
	} else {
		send_message(message);
	}
}

/* forward function for "attack 4" case.
   change this code to implement your attack */
void forward_attack_4(struct message *message) {
    send_message(message);
}

/* forward function for "attack 5" case.
   I did not intend this one to be possible. */
void forward_attack_5(struct message *message) {
    send_message(message);
}
