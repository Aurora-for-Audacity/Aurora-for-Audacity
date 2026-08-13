# Build Instructions
## Cloning the Repo

Aurora for audacity includes a specific version of Audacity and KissFFT, both of which are included as submodules. To clone the repository from your terminal, use the command:

```sh
git clone --recurse-submodules https://github.com/Aurora-for-Audacity/Aurora-for-Audacity.git
```

## CMAKE

It is becoming increasingly apparent that Aurora probably should be organised similar to the other audacity modules, which means embracing the CMake build system. Install instructions are not yet complete, so sadly there will be a little work installing libraries on different systems. An example for macOS is given below.

A `Makefile` is provided for easier building using make and interacting with cmake. Part of the module build process requires patching into the Audacity CMakeLists.txt, specifically the `audacity/modules/etc/CMakeLists.txt`. This allows inserting a line or two to include `mod-aurora` as part of the larger Audacity build.

The `mod-aurora` build process assumes a `libAurora` install in `$(HOME)/.local/`. See the `libAurora` repository for more information on building and installing the Aurora static library.

### Building Audacity on macOS
True for at least the Audacity 3.3.3-alpha
On Xcode 14 on an M2 Mac the following was required to build audacity.
If you do not have homebrew, [download it now](https://brew.sh)

- In your terminal navigate to this repository
- `brew install cmake` if you do not have cmake. This process was tested with `3.27.1`
- `brew install conan@1` Audacity 3.3.3 required conan at version 1
- `brew link conan@1`
- `rm -R ~/.conan2` and `~/.conan` (for good measure) to clear out conan cache, especially if you already have conan installed
- `make xcode`

conan will create a wxwidgets build in `~/.conan` or `~/.conan2` that can be reference by other project

As a series of terminal commands after navigating here

```sh
cd /path/to/Aurora-for-Audacity

# Install LIbraries if Missing
brew install cmake
brew install conan@1
brew link conan@1

# If you already have conan
rm -R ~/.conan

make xcode
```

### Windows

Follow the build instructions for [audacity](https://github.com/audacity/audacity/blob/43981edbdd8a1d9da0cf79a07ba694ad79cb7b47/BUILDING.md)

Most problems seem to come from not running as administrator. Makes sure any programme is launched via `Run as Administrator...`

- copy the `mod-aurora` directory to `audacity/modules`
- add the module to the build list by editing the `audacity/modules/CMakeLists.txt` file
  - add `mod-aurora` under `mod-script-pipe`
- Run the Cmake GUI as covered in the [audacity repo instructions](https://github.com/audacity/audacity/blob/43981edbdd8a1d9da0cf79a07ba694ad79cb7b47/BUILDING.md#building-on-windows)

Requirements

```powershell
winget install --id Python.Python.3.11
winget show --id GnuWin32.Make --source winget
winget install --id Kitware.CMake --source winget
winget install Ninja-build.Ninja --source winget
winget install Microsoft.VisualStudio.2022.BuildTools --source winget

# Use Developer Power Shell for Visual Studio
make build-windows
```
## Enable Aurora

- Go to `Preferences` > `Modules`
- set aurora to `Enable`

I recommend editing the `audacity/libraries/lib-module-manager/ModuleSettings.cpp` file to include `"mod-aurora"` under `autoEnabledModules`

```cpp
static const std::unordered_set<wxString> &autoEnabledModules()
{
   // Add names to this list, of modules that are expected to ship with Audacity
   // or widely used third-party modules to enable automatically.
   static std::unordered_set<wxString> modules{
       "mod-aurora",
   };
   return modules;
}
```

This will speed up the debugging process.
