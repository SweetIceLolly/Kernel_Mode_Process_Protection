# kernel_mode_process_protection
My first kernel-mode process protection driver!

# [bin](bin) folder
This folder contains the executable binaries, including the driver loader (.exe file) and the driver (.sys file).

<b> !! IT IS RECOMMENDED TO TEST THIS PROGRAM IN A VIRTUAL MACHINE !! </b>

How to use these files:
1. Put driver_loader.exe and protection.sys in the same folder.
2. Run driver_loader.exe as Administrator
3. Press 1, Enter. The driver loader will load the driver. This step may be failed because I don't have an appropriate digital signature. You may want to [disable driver signature enforcement](https://windowsreport.com/driver-signature-enforcement-windows-10/).
4. Press 2, Enter. Input the PID of the process you want to protect. The driver loader will communicate with the driver and the driver will start the protection.
5. Try to terminate the target process with Task Manager or taskkill.
5. Press 3, Enter. The driver loader will communicate with the driver and the driver will stop the protection.
6. Press 4, Enter. The driver loader will unload the driver.

# [driver_loader](driver_loader) folder
The source code of driver loader executable (driver_loader.exe).

Most of the code is copied from [my first kernel-mode driver](https://github.com/SweetIceLolly/My_First_Driver).

This program does driver-related operations, including loading the driver, communicating with it, and unloading it.

# [protection](protection) folder
The source code of the driver executable (protection.sys).

Part of the code is copied from [my first kernel-mode driver](https://github.com/SweetIceLolly/My_First_Driver).

[driver.c](protection/driver.c): This file defines `DriverEntry`, which is the entry point of the driver. This file also defines `on_driver_unload`. When the driver is being unloaded, this procedure is called and the protection is stopped.

[device.c](protection/device.c): This file defines device-related functions. `SetupIoDevice` creates an IO device to communicate with the user-mode driver loader.

[major_functions.c](protection/major_functions.c): This file defines major functions handler for `IRP_MJ_*`. Specifically, `IRP_MJ_CREATE` , `IRP_MJ_CLOSE` and `IRP_MJ_WRITE` are handled. The program will use `handle_buffer_message` to process the message received from `IRP_MJ_WRITE`.

[major_functions.c](protection/major_functions.c): Thie file defines `handle_buffer_message` to process the message received from `IRP_MJ_WRITE`. Depending on the command received, the driver will call `enable_protection` or `disable_protection`.

[protection.c](protection/protection.c): This file does everything related to process protection. In `enable_protection` function, the program uses `ObRegisterCallbacks` to register handle creation callbacks. The callback functions are `PreOperationCallback` and `PostOperationCallback`. `PreOperationCallback` is called when a handle to a process is being created. When the driver finds that the opening process handle is the process we want to protect, the access rights are removed so that nobody can access the protected process. In `disable_protection` function, the program uses `ObUnRegisterCallbacks` to unregister callbacks so that the target process is not being protected anymore.

# Acknowledgment/Credits
I found [this helpful tutorial](https://www.evilsocket.net/2014/02/05/termination-and-injection-self-defense-on-windows/), which helped me to implement the protection. Thank you for the valuable and detailed tutorial! ❤

This is my first kernel-mode process protection driver. I hope you can find what you need in this repo :)
