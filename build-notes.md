# Build Instructions
## Cloning the Repo

Aurora for audacity includes a specific version of Audacity and KissFFT, both of which are included as submodules. To clone the repository from your terminal, use the command:

```sh
git clone --recurse-submodules https://github.com/Aurora-for-Audacity/Aurora-for-Audacity.git
```

## CMAKE

It is becoming increasingly apparent that Aurora probably should be organised similar to the other audacity modules, which means embracing the CMake build system.

### Building Audacity on M2

True for at least the Audacity 3.3.3-alpha
On Xcode 14 on an M2 Mac the following was required to build audacity.
If you do not have homebrew, [download it now](https://brew.sh)

- In your terminal navigate to this repository
- copy the `mod-aurora` directory to `audacity/modules`
  - do this manually
  - or sym link with `ln -s $(readlink -f mod-aurora) $(readlink -f audacity/modules/)/mod-aurora`
- add the module by editing the `audacity/modules/CMakeLists.txt` file
  - `sed -i.bak 's/mod-script-pipe/mod-script-pipe\n   mod-aurora/' audacity/modules/CMakeLists.txt`
  - or just do it by hand by adding `mod-aurora` under `mod-script-pipe`
  - `mkdir build && cd build` assuming audacity is the same directory you are in
- `brew install cmake` if you do not have cmake. This process was tested with `3.27.1`
- `brew install conan@1` Audacity 3.3.3 required conan at version 1
- `brew link conan@1`
- `rm -R ~/.conan2` and `~/.conan` (for good measure) to clear out conan cache, especially if you already have conan installed
- `cmake ../audacity -GXcode -Daudacity_use_mad="off" -Daudacity_use_id3tag=off -Daudaicity_conan_allow_prebuilt_binaries=Off --no-warn-unused-cli`

conan will create a wxwidgets build in `~/.conan` or `~/.conan2` that can be reference by other project

As a series of terminal commands after navigating here

```sh
cd /path/to/Aurora-for-Audacity

# Copy
cp -r mod-aurora audacity/modules/
# Symlinm if you want to easily update the source code.
ln -s $(readlink -f mod-aurora) $(readlink -f audacity/modules/)/mod-aurora

sed -i.bak 's/mod-script-pipe/mod-script-pipe\n   mod-aurora/' audacity/modules/CMakeLists.txt
mkdir build && cd build

# Optional
brew install cmake
brew install conan@1
brew link conan@1

# If you already have conan
rm -R ~/.conan

cmake ../audacity -GXcode -Daudacity_use_mad="off" -Daudacity_use_id3tag=off -Daudaicity_conan_allow_prebuilt_binaries=Off --no-warn-unused-cli
```

### Windows

Follow the build instructions for [audacity](https://github.com/audacity/audacity/blob/43981edbdd8a1d9da0cf79a07ba694ad79cb7b47/BUILDING.md)

Most problems seem to come from not running as administrator. Makes sure any programme is launched via `Run as Administrator...`

- copy the `mod-aurora` directory to `audacity/modules`
- add the module to the build list by editing the `audacity/modules/CMakeLists.txt` file
  - add `mod-aurora` under `mod-script-pipe`
- Run the Cmake GUI as covered in the [audacity repo instructions](https://github.com/audacity/audacity/blob/43981edbdd8a1d9da0cf79a07ba694ad79cb7b47/BUILDING.md#building-on-windows)

## Enable Aurora

- Go to `Preferences` > `Modules`
- set aurora to `Enable`


## BUILD SCRIPTS

add script to alter "~/Library/Application Support/audacity/audacity.cfg"
