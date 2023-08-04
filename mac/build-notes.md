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
- WX_PREFIX from `/usr/local/x86_64/lib` to `../wxWidgets/bld_release_x86_64`
- move `wx-config` to the right place `$(WX_PREFIX)/bin/`
- references of `wxWidgetCocoaImpl(peer, view, false, false)` to `wxWidgetCocoaImpl(peer, view)`