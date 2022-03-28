# Knoting

## How to build

### Requirements

- CMake
- NodeJS
- Binaries for the third party libraries that aren't open source

https://cmake.org/download/

https://nodejs.org/en/download/

All the third party binaries must be installed either in the correct OS path for third party libraries, or inside
the `lib/` directory.

Structure of the lib directory:
```
cmake
CMakeLists.txt
core
editor
lib
├── include
│  ├── fmod.cs
│  ├── fmod.h
│  ├── fmod.hpp
│  ├── fmod_codec.h
│  ├── fmod_common.h
│  ├── fmod_dsp.cs
│  ├── fmod_dsp.h
│  ├── fmod_dsp_effects.h
│  ├── fmod_errors.cs
│  ├── fmod_errors.h
│  ├── fmod_output.h
│  ├── mbedtls/
│  ├── NoesisLicense.h
│  ├── NoesisPCH.h
│  ├── NsCore/
│  ├── NsDrawing/
│  ├── NsGui/
│  ├── NsMath/
│  ├── NsRender/
│  ├── psa/
│  ├── sodium/
│  └── sodium.h
└── x86_64
   ├── linux
   │  ├── libfmod.so -> libfmod.so.13.5
   │  ├── libfmod.so.13 -> libfmod.so.13.5
   │  ├── libfmod.so.13.5
   │  ├── libfmodL.so -> libfmodL.so.13.5
   │  ├── libfmodL.so.13 -> libfmodL.so.13.5
   │  ├── libfmodL.so.13.5
   │  └── libNoesis.so
   └── windows
      ├── fmod.dll
      ├── fmod_vc.lib
      ├── fmodL.dll
      ├── fmodL_vc.lib
      ├── libsodium.lib
      ├── libsodium.pdb
      ├── mbedcrypto.lib
      ├── mbedTLS.lib
      ├── mbedx509.lib
      ├── Noesis.dll
      ├── Noesis.lib
      └── quickjs.lib
README.md
res
third
untie
```

You can get in contact with one of the members of the team to get this `lib.zip` directory if it hasn't already been
provided to you. Do to licence issues, we cannot provide these binaries inside the repository.

### Build

#### Clone

```sh
git clone --recursive https://github.com/knot-alot/knoting.git
cd knoting
```

#### Scripts build

```sh
# In the root of the repository
npm i -g typescript
cd res/scripts
tsc # Output directory is `res/scripts/out`
```

After the first compilation the last step of running `tsc` is automated by the build pipeline.

#### General Build

```sh
# In the root of the repository
mkdir bld
cd bld
cmake .. # Recommended 'cmake -G Ninja ..' to use the Ninja build system
# Make sure to be building to the correct architecture of the provided binaries for the third party libraries
# The provided binaries are targeting x64
cmake --build . --parallel
# If 'dist/res/scripts' is empty, please run `cmake ..` once again after making sure that `res/scripts/out` is populated
```

#### Running

```sh
# It takes one environment variable, 'KNOTING_SERVER' which denotes the IP to either host, or connect to
# The build output is located in 'dist/bin/knoting' and the CWD does not matter
 
# In the 'bld' directory of the repository

# Client
./dist/bin/knoting client
# Server
./dist/bin/knoting server
```
