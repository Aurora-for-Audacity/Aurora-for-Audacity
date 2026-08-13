# Root directories
ROOT := $(CURDIR)
AUDACITY := $(ROOT)/audacity
MODULE := $(ROOT)/mod-aurora
BUILD ?= $(ROOT)/build

# Windows specific additions
ifeq ($(OS),Windows_NT)
	AURORA_ROOT ?= $(USERPROFILE)/.local
	GEN ?= "Visual Studio 17 2022"
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
	-Daudacity_use_mad=OFF \
	-DAUDACITY_BUILD_LEVEL=2 \
	-Daudacity_use_id3tag=OFF \
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

.PHONY: audacity aurora-debug build aurora-release patch clean distclean 

audacity:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)
	
aurora-debug: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_AURORA_FLAGS)

build: aurora-debug
	cmake --build $(BUILD)

aurora-release: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_RELEASE_FLAGS)
	cmake --build $(BUILD)

patch:
	python "$(ROOT)/tools/patch_aurora.py" "$(AUDACITY)/modules/etc/CMakeLists.txt"

clean:
ifeq ($(OS),Windows_NT)
	Remove-Item -Recurse -Force .\build
else 
	rm -rf $(BUILD)
endif
	
distclean: clean
ifeq ($(OS),Windows_NT)
	Remove-Item -Force "$(AUDACITY)/modules/etc/mod-aurora" -ErrorAction SilentlyContinue
	Copy-Item "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" "$(AUDACITY)/modules/etc/CMakeLists.txt" -ErrorAction SilentlyContinue
	Remove-Item -Force "$(AUDACITY)/modules/CMakeLists.txt.bak" -ErrorAction SilentlyContinue
else
	rm -f "$(AUDACITY)/modules/etc/mod-aurora"
	cp "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/etc/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"
endif