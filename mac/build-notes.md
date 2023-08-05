## Build Instructions

### Audacity

Starting from the root of this repo, the steps to build audacity@2.4.1 were

```sh
cd ./audacity
git clone --recurse-submodules https://github.com/audacity/wxWidgets.git
sudo ../mac/scripts/build_wxwidgets
```

- Two paths required addition to the XCode project. 
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