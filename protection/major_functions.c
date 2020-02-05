#include "driver.h"

NTSTATUS Io_Unsupported(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("Io_Unsupported() called!");
	return STATUS_SUCCESS;
}

NTSTATUS Create_DeviceIo(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("Create_DeviceIo() called!");
	return STATUS_SUCCESS;
}

NTSTATUS Close_DeviceIo(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);

	DbgPrint("Close_DeviceIo() called!");
	return STATUS_SUCCESS;
}

NTSTATUS Buffered_Write(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION pIoStack = NULL;
	char *Buffer = NULL;

	DbgPrint("Buffered_Write() called!");

	pIoStack = IoGetCurrentIrpStackLocation(Irp);
	if (pIoStack) {											//Check for NULL pointer
		Buffer = (char*)(Irp->AssociatedIrp.SystemBuffer);

		if (Buffer) {											//Check for NULL pointer
			handle_buffer_message(Buffer, pIoStack->Parameters.Write.Length);
			DbgPrint("Message received: size: %u, msg: %s", pIoStack->Parameters.Write.Length, Buffer);
		}
		else {
			DbgPrint("Buffer is a NULL pointer!");
		}
	}
	else {
		DbgPrint("Invalid IRP stack pointer!");
	}

	return STATUS_SUCCESS;
}
