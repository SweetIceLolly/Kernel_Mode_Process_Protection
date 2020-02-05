#include "driver_loader.h"

/*
Purpose:	Create the driver service
Args:		ServiceName: The name of the service
.			ExecutablePath: File path of the driver file
Return:		0:Failed; 1: Succeed
*/
int Create_Service(const char *ServiceName, const char *ExecutablePath) {
	printf("Creating service: %s\n", ServiceName);

	SC_HANDLE sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);		//Open a handle to Service Manager
	if (sh == INVALID_HANDLE_VALUE) {
		printf("OpenSCManager() failed!\n");
		return 0;
	}

	SC_HANDLE hService = CreateService(sh, ServiceName, ServiceName,		//Create the driver service
		SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		ExecutablePath, NULL, NULL, NULL, NULL, NULL);
	CloseServiceHandle(sh);													//Use this instead of CloseHandle()

	if (hService == NULL) {
		printf("CreateService() failed!\n");
		return 0;
	}

	CloseServiceHandle(hService);
	return 1;
}

/*
Purpose:	Start the specified service
Args:		ServiceName: Name of the service
Return:		0: Failed; 1: Succeed
*/
int Start_Service(const char *ServiceName) {
	SC_HANDLE sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);		//Open a handle to Service Manager
	if (sh == INVALID_HANDLE_VALUE) {
		printf("OpenSCManager() failed!\n");
		return 0;
	}

	SC_HANDLE hService = OpenService(sh, ServiceName, SERVICE_ALL_ACCESS);	//Get a handle to the driver service
	CloseServiceHandle(sh);
	if (hService == NULL) {
		printf("OpenService() failed!\n");
		return 0;
	}

	if (StartService(hService, 0, NULL) == 0) {
		int err_code = (int)GetLastError();
		printf("StartService() failed: %i\n", err_code);
		return 0;
	}

	CloseServiceHandle(hService);
	return 1;
}

/*
Purpose:	Stop the specified service
Args:		ServiceName: Name of the service
Return:		0: Failed; 1: Succeed
*/
int Stop_Service(const char *ServiceName) {
	SC_HANDLE sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);		//Open a handle to Service Manager
	if (sh == INVALID_HANDLE_VALUE) {
		printf("OpenSCManager() failed!\n");
		return 0;
	}

	SC_HANDLE hService = OpenService(sh, ServiceName, SERVICE_ALL_ACCESS);	//Get a handle to the driver service
	CloseServiceHandle(sh);
	if (hService == NULL) {
		printf("OpenService() failed!\n");
		return 0;
	}

	SERVICE_STATUS ss;
	if (ControlService(hService, SERVICE_CONTROL_STOP, &ss) == 0) {
		CloseServiceHandle(hService);
		return 0;
	}

	CloseServiceHandle(hService);
	return 1;
}

/*
Purpose:	Delete the specified service
Args:		ServiceName: Name of the service
Return:		0: Failed; 1: Succeed
*/
int Delete_Service(const char *ServiceName) {
	SC_HANDLE sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);		//Open a handle to Service Manager
	if (sh == INVALID_HANDLE_VALUE) {
		printf("OpenSCManager() failed!\n");
		return 0;
	}

	SC_HANDLE hService = OpenService(sh, ServiceName, SERVICE_ALL_ACCESS);	//Get a handle to the driver service
	CloseServiceHandle(sh);
	if (hService == NULL) {
		printf("OpenService() failed!\n");
		return 0;
	}

	if (DeleteService(hService) == 0) {
		CloseServiceHandle(hService);
		return 0;
	}

	CloseServiceHandle(hService);
	return 1;
}

/*
Purpose:	Get an IO handle
Args:		DeviceName: Device name
Return:		The IO handle. -1 if failed
Note:		Remember to close the handle with Close_IO_Handle()
*/
HANDLE Get_IO_Handle(char *DeviceName) {
	printf("Opening: %s\n", DeviceName);
	return (HANDLE)CreateFile(DeviceName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
}

/*
Purpose:	Close an IO handle
Args:		hIO: The IO handle
*/
void Close_IO_Handle(HANDLE hIO) {
	if (hIO != INVALID_HANDLE_VALUE) {
		CloseHandle(hIO);
	}
}

/*
Purpose:	Write data to an IO handle
Args:		hDevice: The IO handle
.			Buffer: Data to write
.			WriteSize: The length of data to write
Return:		0: failed; 1: succeed
*/
int Write_IO_Handle(HANDLE hDevice, const char *Buffer, const int WriteSize) {
	if (WriteFile(hDevice, Buffer, WriteSize, NULL, NULL) != 0) {
		return 1;
	}
	else {
		return 0;
	}
}
