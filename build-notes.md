# Build Instructions

## CMAKE

CMake tries to generate a project then build it but is failing at the build stage. You should still be able to go into the generated Xcode project and build from there. Found in `audacity/cmake-build-debug`

Maybe also

`/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk`

```sh
xcode-select -switch /Applications/Xcode.app    
```

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
  - or just do it by hand by add ing `mod-aurora` under `mod-script-pipe`
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

## Enable Aurora

- Go to `Tools` > `Add / Remove Plug-ins...`
- sort by `Path`
 - Generate
 - Tools
 - Analyze

