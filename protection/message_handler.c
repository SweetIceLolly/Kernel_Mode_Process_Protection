#include "driver.h"

void handle_buffer_message(char *buffer, int len) {
	if (len == 1) {
		if (buffer[0] == 'd') {				//'d' for disable protection
			disable_protection();
		}
	}
	else if (len == 5) {
		if (buffer[0] == 'e') {				//'e' for enable protection
			int pid;

			memcpy(&pid, &buffer[1], 4);
			DbgPrint("Got it! Target pid: %i", pid);
			if (enable_protection(pid)) {
				DbgPrint("enable_protection() succeed!");
			}
			else {
				DbgPrint("enable_protection() failed!");
			}
		}
	}
}