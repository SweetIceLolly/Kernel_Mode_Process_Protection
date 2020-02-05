#include "driver.h"

/*
Referenced from https://www.evilsocket.net/2014/02/05/termination-and-injection-self-defense-on-windows/
Thank you!
*/

//Function prototypes --------------------------------------------
void PostOperationCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION OperationInformation);
OB_PREOP_CALLBACK_STATUS PreOperationCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
//----------------------------------------------------------------

//Global variables -----------------------------------------------
void *CallbackRegistrationHandle = NULL;
int ProtectedPid = 0;
//----------------------------------------------------------------

void disable_protection() {
	if (CallbackRegistrationHandle != NULL) {
		ObUnRegisterCallbacks(CallbackRegistrationHandle);
		CallbackRegistrationHandle = NULL;
	}
	DbgPrint("disable_protection() called!");
}

/*
Purpose:	Register a handle creation callback function to enable process protection
Args:		pid: The process ID to protect
Return:		0: Failed; 1: Succeed
*/
int enable_protection(int pid) {
	OB_OPERATION_REGISTRATION OperationRegistrations[1] = { { 0 } };
	OB_CALLBACK_REGISTRATION CallbackRegistration = { 0 };
	UNICODE_STRING ustrAltitude = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	ProtectedPid = pid;

	OperationRegistrations[0].ObjectType = PsProcessType;					//Set type to process
	OperationRegistrations[0].Operations = OB_OPERATION_HANDLE_CREATE;		//Intercept all handle creation
	OperationRegistrations[0].PreOperation = PreOperationCallback;
	OperationRegistrations[0].PostOperation = PostOperationCallback;

	RtlInitUnicodeString(&ustrAltitude, L"1000");
	CallbackRegistration.Version = OB_FLT_REGISTRATION_VERSION;
	CallbackRegistration.OperationRegistrationCount = 1;
	CallbackRegistration.Altitude = ustrAltitude;
	CallbackRegistration.RegistrationContext = (PVOID)&ProtectedPid;
	CallbackRegistration.OperationRegistration = OperationRegistrations;

	status = ObRegisterCallbacks(&CallbackRegistration, &CallbackRegistrationHandle);
	if (NT_SUCCESS(status)) {
		DbgPrint("ObRegisterCallbacks() succeed!");
		return 1;
	}
	else {
		DbgPrint("ObRegisterCallbacks() failed! status: %i", status);
		return 0;
	}
}

/*
Purpose:	This callback function is called when a process handle is created
Args:		RegistrationContext: Unused. Should be the PID of the protected process
.			OperationInformation: Unused. Information of the operation
*/
void PostOperationCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION OperationInformation) {
	//Do nothing here
	UNREFERENCED_PARAMETER(RegistrationContext);
	UNREFERENCED_PARAMETER(OperationInformation);
}

/*
Purpose:	Register a handle creation callback function to enable process protection
Args:		pid: The process ID to protect
Return:		Must be OB_PREOP_SUCCESS
*/
OB_PREOP_CALLBACK_STATUS PreOperationCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation) {
	PEPROCESS	TargetProcess = OperationInformation->Object;
	PEPROCESS	CurrentProcess = PsGetCurrentProcess();
	HANDLE		TargetPid = PsGetProcessId(TargetProcess);

	//Allow operations from the process itself
	if (CurrentProcess == TargetProcess) {
		return OB_PREOP_SUCCESS;
	}

	//Allow operations from the kernel
	if (OperationInformation->KernelHandle == 1) {
		return OB_PREOP_SUCCESS;
	}

	//Ignore other processes
	if (TargetPid != (HANDLE)(*(int*)RegistrationContext)) {
		return OB_PREOP_SUCCESS;
	}
	else {
		//Someone is trying to obtain a handle to the protected process
		//Remove "dangerous" access rights!!
		OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
		DbgPrint("Hahahaha! Operation blocked! Don't even try to kill me!");
	}
	
	return OB_PREOP_SUCCESS;
}
