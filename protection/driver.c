#include "driver.h"

//Function prototypes --------------------------------------------
void on_driver_unload(PDRIVER_OBJECT DriverObject);
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath);
//----------------------------------------------------------------

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);

	DriverObject->DriverUnload = on_driver_unload;

	unsigned int i;
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
		DriverObject->MajorFunction[i] = Io_Unsupported;
	}
	DriverObject->MajorFunction[IRP_MJ_CREATE] = Create_DeviceIo;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = Close_DeviceIo;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = Buffered_Write;

	init_unicode_strings();
	if (SetupIoDevice(DriverObject) == 1) {
		DbgPrint("SetupIoDevice() succeed!");
	}
	else {
		DbgPrint("SetupIoDevice() failed!");
	}

	DbgPrint("Driver loaded!");

	return STATUS_SUCCESS;
}

void on_driver_unload(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	disable_protection();

	DbgPrint("Driver unloaded.");
}
