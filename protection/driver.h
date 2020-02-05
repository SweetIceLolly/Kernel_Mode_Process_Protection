#include <ntddk.h>
#include <wdf.h>

//Define major functions
NTSTATUS Io_Unsupported(PDEVICE_OBJECT DeviceObject, PIRP Irp);		//IRP_MJ_*
NTSTATUS Create_DeviceIo(PDEVICE_OBJECT DeviceObject, PIRP Irp);	//IRP_MJ_CREATE
NTSTATUS Close_DeviceIo(PDEVICE_OBJECT DeviceObject, PIRP Irp);		//IRP_MJ_CLOSE
NTSTATUS Buffered_Write(PDEVICE_OBJECT DeviceObject, PIRP Irp);		//IRP_MJ_WRITE

//Define buffer messaege handler function (for Buffered_Write)
void handle_buffer_message(char *buffer, int len);

//Define device-related functions
void init_unicode_strings();
int SetupIoDevice(PDRIVER_OBJECT DriverObject);

//Define protection functions
void disable_protection();
int enable_protection(int pid);
