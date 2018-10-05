# isnp-emu-lib

[![Project Stats](https://www.openhub.net/p/isnp-emu-lib/widgets/project_thin_badge?format=gif)](https://www.openhub.net/p/isnp-emu-lib)

An [ISNP](https://www.researchgate.net/publication/303380880_ISNPGNEIS_Facility_in_Gatchina_for_Neutron_Testing_with_Atmospheric-Like_Spectrum) facility modeling library based on [Geant4](http://geant4.web.cern.ch/) toolkit.

## Requirements

* GCC 4.9.2 or higher
* CMake 3.1 or higher
* Geant4 10.4 or higher
* Google Test 1.7 or higher

## Content

* `lib` - library itself.
* `basic` - a very basic executable that invokes library.
* `examples` - macro files with ISNP-related examples.

## Prerequisites

* Geant4 installed and configured

## Building

1. Go to your working directory, for example `~/workspace` (all directory names are arbitrary).

2. Create `isnp-emu-lib` subdirectory within the working directory.

3. Download content of `isnp-emu-lib` repository into `~/workspace/isnp-emu-lib`.

4. Create `isnp-emu-lib-build` subdirectory within the working directory.

5. Enter `~/workspace/isnp-emu-lib-build` directory.

6. Run `cmake`:

```bash
cmake ~/workspace/isnp-emu-lib
```

6.1. If you need to install the library into your profile-specific directory rather than system directory, then set `CMAKE_INSTALL_PREFIX` system property for `cmake` as follows:

```bash
cmake -DCMAKE_INSTALL_PREFIX=~ ~/workspace/isnp-emu-lib
```

7. Build and install library and executables:

```bash
make && make install
```

8. Run example

```bash
cd ~/workspace/isnp-emu-lib
isnp-basic examples/basic.mac
```

