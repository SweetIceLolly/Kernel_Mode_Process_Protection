#include "driver.h"

//Global variables -----------------------------------------------
const wchar_t	*DeviceName = L"\\Device\\MyProtectionDriver";
const wchar_t	*SymbolicLink = L"\\DosDevices\\MyProtectionDriver";

UNICODE_STRING	ustrDeviceName;
UNICODE_STRING	ustrSymbolicLink;

PDEVICE_OBJECT	CreatedDeviceObject = NULL;
//----------------------------------------------------------------

void init_unicode_strings() {
	RtlInitUnicodeString(&ustrDeviceName, DeviceName);
	RtlInitUnicodeString(&ustrSymbolicLink, SymbolicLink);
}

/*
Purpose:	Create I/O device
Args:		DriverObject: Driver Object
Return:		0: Failed; 1: Succeed
*/
int SetupIoDevice(PDRIVER_OBJECT DriverObject) {
	DbgPrint("Creating I/O device! name: %ws", ustrDeviceName.Buffer);

	int ret = IoCreateDevice(DriverObject, 0, &ustrDeviceName, FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN, FALSE, &CreatedDeviceObject);
	if (ret != STATUS_SUCCESS) {
		DbgPrint("IoCreateDevice() failed!");
		return 0;
	}

	CreatedDeviceObject->Flags |= DO_BUFFERED_IO;
	CreatedDeviceObject->Flags &= (~DO_DEVICE_INITIALIZING);
	DbgPrint("Device created!");

	ret = IoCreateSymbolicLink(&ustrSymbolicLink, &ustrDeviceName);
	if (ret != STATUS_SUCCESS) {
		DbgPrint("IoCreateSymbolicLink() failed!");
		return 0;
	}
	DbgPrint("Symbolic link created!: %ws -> %ws", ustrSymbolicLink.Buffer, ustrDeviceName.Buffer);
	return 1;
}
