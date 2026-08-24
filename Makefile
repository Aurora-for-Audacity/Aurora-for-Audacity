# Root directories
# ROOT := $(CURDIR)
ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
AUDACITY := $(ROOT)/audacity
MODULE := $(ROOT)/mod-aurora
BUILD ?= $(ROOT)/build


# Windows specific additions
#
# AURORA_ROOT is where the libAurora Static library has been built
# The deafults here mirror that in the libAurora release install rule

ifeq ($(OS),Windows_NT)
	AURORA_ROOT ?= $(USERPROFILE)/.local
	GEN ?= Visual Studio 17 2022
	PLATFORM := Windows
else
	PLATFORM := $(shell uname -s)
	AURORA_ROOT ?= $(HOME)/.local
	GEN ?= Ninja
endif

# CMake 4.x compatibility for older Conan dependencies
export CMAKE_POLICY_VERSION_MINIMUM ?= 3.5

CMAKE_AUDACITY_FLAGS := \
	-G "$(GEN)" \
	-DAUDACITY_BUILD_LEVEL=2 \
	-Daudacity_conan_allow_prebuilt_binaries=on \

ifeq ($(PLATFORM),Darwin)
	CMAKE_AUDACITY_FLAGS += \
		-DMACOS_ARCHITECTURE=arm64 \
		-Daudacity_perform_codesign=on
endif

CMAKE_AURORA_FLAGS := \
	$(CMAKE_AUDACITY_FLAGS) \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(AURORA_ROOT)

CMAKE_RELEASE_FLAGS := \
	$(CMAKE_AURORA_FLAGS) \
	-DCMAKE_BUILD_TYPE=Release

# Make Rules

.PHONY: audacity aurora-debug build aurora-release patch clean distclean relink macos-package

audacity:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_AUDACITY_FLAGS)
	
aurora-debug: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_AURORA_FLAGS)

build: aurora-debug
	cmake --build $(BUILD)

aurora-release: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_RELEASE_FLAGS)
ifeq ($(OS),Windows_NT)
	cmake --build $(BUILD) --config Release
else
	cmake --build $(BUILD)
endif

relink:
ifeq ($(PLATFORM),Darwin)
	cp $(BUILD)/Release/Audacity.app/Contents/modules/mod-aurora.so $(ROOT)/tools/installer/payload/tmp/aurora-audacity-module/
	bash $(ROOT)/tools/update-linkage.sh $(ROOT)/tools/installer/payload/tmp/aurora-audacity-module/mod-aurora.so
endif

macos-package: relink
	bash "$(ROOT)/tools/installer/build.sh"

patch:
	python "$(ROOT)/tools/patch_aurora.py" "$(AUDACITY)/modules/etc/CMakeLists.txt"

clean:
	rm -rf $(BUILD)
	
distclean: clean
	rm -f "$(AUDACITY)/modules/etc/mod-aurora"
	cp "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/etc/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"