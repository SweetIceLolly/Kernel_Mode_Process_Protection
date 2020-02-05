#include "driver_loader.h"

#define SERVICE_NAME	"MyProtectionDriver"
#define DEVICE_NAME		"\\\\.\\MyProtectionDriver"

#ifndef __GNUC__			//Use scanf_s for Visual Studio, but not for gcc
#define scanf scanf_s
#endif

HANDLE	hIoHandle = INVALID_HANDLE_VALUE;

/*
Purpose:	Load the driver
Args:		driver_file: Path to the driver file
Return:		0:Failed; 1: Succeed
*/
int load_driver(const char *driver_file) {
	//Create service
	if (Create_Service(SERVICE_NAME, driver_file) == 1) {
		printf("Create_Service() succeed!\n");
	}
	else {
		printf("Create_Service() failed!\n");
		return 0;
	}

	//Start service
	if (Start_Service(SERVICE_NAME) == 1) {
		printf("Start_Service() succeed!\n");
	}
	else {
		printf("Start_Service() failed!\n");
		return 0;
	}

	//Get IO handle
	hIoHandle = Get_IO_Handle(DEVICE_NAME);
	if (hIoHandle != INVALID_HANDLE_VALUE) {
		printf("Get_IO_Handle() succeed!\n");
	}
	else {
		printf("Get_IO_Handle() failed: %i\n", GetLastError());
		return 0;
	}

	return 1;
}

/*
Purpose:	Send message to the device to start the protection
Args:		pid: The process ID to protect
Return:		0:Failed; 1: Succeed
*/
int protect_process(int pid) {
	char command[6] = { 0 };

	command[0] = 'e';				//'e' for enable protection
	*((int*)&command[1]) = pid;

	if (Write_IO_Handle(hIoHandle, command, 5) == 1) {
		printf("Write_IO_Handle() succeed!\n");
	}
	else {
		printf("Write_IO_Handle() failed!\n");
		return 0;
	}

	return 1;
}

int stop_protection() {
	char command[1] = { 'd' };			//'d' for disable protection

	if (Write_IO_Handle(hIoHandle, command, 1) == 1) {
		printf("Write_IO_Handle() succeed!\n");
	}
	else {
		printf("Write_IO_Handle() failed!\n");
		return 0;
	}

	return 1;
}

void unload_driver() {
	Close_IO_Handle(hIoHandle);
	printf("Close_IO_Handle() called!\n");

	if (Stop_Service(SERVICE_NAME) == 1) {
		printf("Stop_Service() succeed!\n");
	}
	else {
		printf("Stop_Service() failed!\n");
	}

	if (Delete_Service(SERVICE_NAME) == 1) {
		printf("Delete_Service() succeed!\n");
	}
	else {
		printf("Delete_Service() failed!\n");
	}
}

int main(void) {
	int		selection;
	int		pid;
	char	driver_file[MAX_PATH];

	printf(
		"1: Load Driver\n"
		"2: Protect Process\n"
		"3: Stop Protection\n"
		"4: Unload Driver\n"
		"5: Bye!\n"
		"\n"
		);

	GetCurrentDirectory(MAX_PATH, driver_file);
#ifndef __GNUC__			//Use strcat_s for Visual Studio, but not for gcc
	strcat_s(driver_file, MAX_PATH, "\\protection.sys");
#else
	strcat(driver_file, "\\protection.sys");
#endif

	for (;;) {
		scanf("%i", &selection);

		switch (selection) {
		case 1:							//Load driver
			load_driver(driver_file);
			break;

		case 2:							//Protect process
			printf("PID: ");
			scanf("%i", &pid);
			protect_process(pid);
			break;

		case 3:							//Stop protection
			stop_protection();
			break;
			
		case 4:							//Unload driver
			unload_driver();
			break;

		case 5:							//Exit
			stop_protection();
			unload_driver();
			return 0;
			break;

		default:
			printf("What?\n");
		}
	}

	return 0;
}
