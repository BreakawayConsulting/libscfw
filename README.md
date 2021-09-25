# i.MX SCFW API library for seL4

This is a port of the i.MX SCFW API library.
The library allows access to the SCU processor that is embedded
in various i.MX SoCs via the messaging unit.

It is primarily designed for use within seL4 based projects,
however other than the build system there is nothing seL4 specific.

Testing of this library has only been performed on an i.MX8QX SoC.
It may also work on other SoC in the i.MX family.

Most source and header files are take from the imx-scfw-porting-kit
version 1.6.0 provided by NXP.
The files included in this library are taken from the
`scfw_export_mx8qx_b0` archive within the porting kit.

The files are unchanged, except for unavoidable modifications in
the header include block of each file.
This block is updated to match the directory layout of this library.

The *new files* in this port are:

* `include/scfw/init.h` which provides the declaration of the
  `sc_ipc_init` function.
* `src/rpc.c` which implements to `sc_ipc_init` and `sc_call_rpc`
  functions.
* `CMakeLists.txt` which allows integration with the standard
   seL4 ecosystem build tools.

The `sc_call_rpc` function is called by the publicly used API functions
to send and receive messages via the messaging unit. This
implementation  is a pure polling implementation; it does not use
interrupts. As some API function can take a number of milliseconds for
the SCU to execute this may not be appropriate for a given use case,
however works well if used during initial setup.

The `sc_ipc_init` function must be called by the user to initialize
each IPC channel with the address of the messaging unit.

## License

This library is made available under the BSD-3-Clause library.
As noted most files are directly from the porting kit and
are therefore copyright is held by NXP. The new files were
developed by Breakaway Consulting and copyright is held by
Breakaway Consulting Pty. Ltd.
