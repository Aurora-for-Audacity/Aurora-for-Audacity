## Build Instructions

### Audacity

Starting from the root of this repo, the steps to build audacity@2.4.1 were

The `Aurora-for-Audacity` repo will need to cloned with `--recurse-submodules`

```sh
git clone --recurse-submodules https://github.com/Aurora-for-Audacity/Aurora-for-Audacity.git
cd Aurora-for-Audacity/wxWidgets
sudo ../audacity/mac/scripts/build_wxwidgets
```

- Two paths required addition to the XCode project
    - `../wxWidgets/include` 
    - `../wxWidgets/bld_release_x86_64/lib/wx/include/osx_cocoa-unicode-3.1-release`
    OR
    - `$(WX_PREFIX)/bld_$(WX_FLAVOUR)_x86_64/lib/wx/include/osx_cocoa-unicode-$(WX_VER)-$(WX_FLAVOUR)` 
    - `$(WX_PREFIX)/include`
- WX_PREFIX from `/usr/local/x86_64/lib` to `../wxWidgets/bld_release_x86_64`
- move `wx-config` to the right place `$(WX_PREFIX)/bin/`
- references of `wxWidgetCocoaImpl(peer, view, false, false)` to `wxWidgetCocoaImpl(peer, view)`
- `wxwidgets` library is in the wrong format compared to standard `brew install`. Need to copy over build:
    -  without breaking symlinks
    -  still maintaining structure expected by paths in `.xcconfig`
- change `AUDACITY_VERSION_STRING`

### Audacity Project Config File

WX_PREFIX should be wherever WX_PREFIX has been put relative to the Audacity xcode project.

```cpp
WX_PREFIX = ../../wxWidgets
WX_VER = 3.1
WX_FULL_VER = 3.1.3.0.0
WX_FLAVOR = debug

WX_CONFIG = $(WX_PREFIX)/bld_$(WX_FLAVOR)_x86_64/wx-config
WX_CFLAGS = -I$(WX_PREFIX)/bld_$(WX_FLAVOR)_x86_64/lib/wx/include/osx_cocoa-unicode-$(WX_VER)-$(WX_FLAVOR) -I$(WX_PREFIX)/include/ -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=1 -DWXUSINGDLL -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__

WX_CXXFLAGS = $(WX_CFLAGS)

WX_LDFLAGS = -L$(WX_PREFIX)/bld_$(WX_FLAVOR)_x86_64/lib -lwx_osx_cocoau_$(WX_FLAVOR)_xrc-$(WX_FULL_VER) -lwx_osx_cocoau_$(WX_FLAVOR)_html-$(WX_FULL_VER) -lwx_osx_cocoau_$(WX_FLAVOR)_qa-$(WX_FULL_VER) -lwx_osx_cocoau_$(WX_FLAVOR)_adv-$(WX_FULL_VER) -lwx_osx_cocoau_$(WX_FLAVOR)_core-$(WX_FULL_VER) -lwx_baseu_$(WX_FLAVOR)_xml-$(WX_FULL_VER) -lwx_baseu_$(WX_FLAVOR)_net-$(WX_FULL_VER) -lwx_baseu_$(WX_FLAVOR)-$(WX_FULL_VER)

HEADER_SEARCH_PATHS =$(WX_PREFIX)/include $(WX_PREFIX)/bld_$(WX_FLAVOUR)_x86_64/lib/wx/include/osx_cocoa-unicode-$(WX_VER)-$(WX_FLAVOUR)
```

### WX Structure

Standard install of wxwidets package would need to comform to the following structure

```
./wxwidgets
├── README.md
├── bin
│   └── wx-config
├── include
│   └── wx-3.1
│       └── wx
│           ├── aboutdlg.h
│           ├── ...
└── lib
    ├── libwx_baseu-3.1.3.0.0.dylib
    ├── libwx_baseu-3.1.3.dylib -> libwx_baseu_debug-3.1.3.0.0.dylib
    ├── libwx_baseu-3.1.dylib -> libwx_baseu_debug-3.1.3.dylib
    ├── ...
    └── wx
        ├── config
        │   └── osx_cocoa-unicode-3.1
        └── include
            └── osx_cocoa-unicode-3.1
                └── wx
                    └── setup.h
```

Audacity adds in a `FLAVOUR` macro to allow for debug and release, but it also adds in i386 and x86_64 architecture varieties which will likely not be relevant to current users given the proble is supporting future architectures, not backwards compatibility.

## Enable Aurora

- Go to `Tools` > `Add / Remove Plug-ins...`
- sort by `Path`
 - Generate
 - Tools
 - Analyze

 ## wxWidgets for `arm64`

compiling with:

```sh
    ../configure --with-osx_cocoa  \
   --with-macosx-version-min=13.0 \
   --with-macosx-sdk="/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk" \
   --disable-shared \
   --prefix=/usr/local \
   --enable-universal_binary=x86_64,arm64 \
   --with-libtiff=builtin
```
seems to have success. though this relies on `--enable-universal_binary=x86_64,arm64`. Unsure how reliable this is.

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
cp -r mod-aurora audacity/modules/
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

#### Aurora as a Module

It is becoming increasingly apparent that Aurora probably should be organised similar to the other audacity modules, which means embracing the CMake build system. 

At the moment modules are organised by the `audacity_module` macro defined within Audcaity. Since so much of Aurora requires re-writing of header paths alone, the easiest way I can think is to essentially duplicate this method but allow for some entry points to stipulate header search paths.

