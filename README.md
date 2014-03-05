# clw - OpenCL Wrapper

Loosely based on [QtOpenCL](https://qt.gitorious.org/qt-labs/opencl) library.

## Building

To generate project files premake4 is required (version 4.3 at least). Type 

`premake4 [options] [generator]`

where `options` can be:
* `--clincdir=PATH` where `PATH` points to a directory with a CL/cl.h file (e.g. `--clincdir=/opt/AMDAPP/SDK/include`). If OpenCL header files are located in one of compiler default include directory this option can be omitted.
* `--cllibdir=PATH` where `PATH` points to a directory with a OpenCL.lib/libOpenCL.so. If OpenCL library files are located in one of linker default libraries directory this option can be omitted.
* `--cllib64dir=PATH` where `PATH` points to a directory with a OpenCL.lib/libOpenCL.so in x64 flavour (e.g. `--cllib64dir=/opt/AMDAPP/SDK/lib`). If OpenCL library files are located in one of linker default libraries directory this option can be omitted.

If only one architecture build is required (most of cases) you only need to provide path to that OpenCL libraries.

`generator` can be one of following:

- `vs20xx` for Visual Studio
- `gmake` for GNU makefiles using g++

If not given, default one for host OS is picked (MSVC for Windows, GNU makefiles for Linux).
Visual Studio project files should be self-explanatory and shouldn't cause any troubles.

To build using just-generated GNU makefiles files type:
`make config=CONFIG`

`CONFIG` is a mixture of `release`,`debug`,`releaselib`,`debuglib` and `32`,`64`.

## License

clw is licensed under MIT license, see [COPYING](COPYING) file for details.

