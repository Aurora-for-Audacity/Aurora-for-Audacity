# Root directories
ROOT := $(CURDIR)
AUDACITY := $(ROOT)/audacity
MODULE := $(ROOT)/mod-aurora
BUILD := $(ROOT)/build

# Windows specific additions
ifeq ($(OS),Windows_NT)
	AURORA_ROOT ?= $(USERPROFILE)/.local
else
	AURORA_ROOT ?= $(HOME)/.local
endif

# CMake 4.x compatibility for older Conan dependencies
export CMAKE_POLICY_VERSION_MINIMUM ?= 3.5


CMAKE_AUDACITY_FLAGS := \
	-G Ninja \
	-Daudacity_use_mad=OFF \
	-Daudacity_use_id3tag=OFF \
	-Daudacity_conan_allow_prebuilt_binaries=on	

CMAKE_FLAGS := \
	-G Ninja \
	-Daudacity_use_mad=OFF \
	-Daudacity_use_id3tag=OFF \
	-Daudacity_conan_allow_prebuilt_binaries=OFF \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(AURORA_ROOT)
	

CMAKE_RELEASE_FLAGS := \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	-DMACOS_ARCHITECTURE=arm64 \
	-Daudacity_perform_codesign=OFF \
	-DAUDACITY_BUILD_LEVEL=2 \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(AURORA_ROOT)

# Make Rules

.PHONY: xcode configure link patch clean distclean test

xcode: configure
	@echo
	@echo "Xcode project created:"
	@echo "  $(BUILD)/Audacity.xcodeproj"

audacity-only:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_AUDACITY_FLAGS)
	# for windows it might need to copy dll flat across the build directory rather than in targets. 
	# Copy-Item .\build\Debug\*.dll .\build\

configure: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)

# for module build, ninja -C build mod-aurora 
# ninja -C build -t targets | grep -i script if you can't find it
release-build:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_RELEASE_FLAGS)
	cmake -B $(BUILD)

patch:
	python "$(ROOT)/tools/patch_aurora.py" "$(AUDACITY)/modules/etc/CMakeLists.txt"

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -f "$(AUDACITY)/modules/etc/mod-aurora"
	cp "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/etc/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"

