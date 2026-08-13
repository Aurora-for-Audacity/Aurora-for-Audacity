# Root directories
ROOT := $(CURDIR)
AUDACITY := $(ROOT)/audacity
MODULE := $(ROOT)/mod-aurora
BUILD ?= $(ROOT)/build

# Windows specific additions
ifeq ($(OS),Windows_NT)
	AURORA_ROOT ?= $(USERPROFILE)/.local
	GEN ?= "Visual Studio 17 2022"
else
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
	-Daudacity_conan_allow_prebuilt_binaries=on	

CMAKE_FLAGS := \
	-G "$(GEN)" \
	-Daudacity_use_mad=OFF \
	-DAUDACITY_BUILD_LEVEL=2 \
	-Daudacity_use_id3tag=OFF \
	-Daudacity_conan_allow_prebuilt_binaries=on \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(AURORA_ROOT)
	

CMAKE_RELEASE_FLAGS := \
	-G "$(GEN)" \
	-DCMAKE_BUILD_TYPE=Release \
	-DAUDACITY_BUILD_LEVEL=2 \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(AURORA_ROOT)

# Windows specific additions
ifeq ($(OS),Windows_NT)
	
else
	CMAKE_RELEASE_FLAGS += -DMACOS_ARCHITECTURE=arm64 -Daudacity_perform_codesign=on
endif

# Make Rules

.PHONY: xcode configure link patch clean distclean test

xcode: configure
	@echo
	@echo "Xcode project created:"
	@echo "  $(BUILD)/Audacity.xcodeproj"

audacity-only:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_AUDACITY_FLAGS)
	
configure:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)

build: configure
	cmake --build $(BUILD)

release-build:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_RELEASE_FLAGS)
	cmake --build $(BUILD)

patch:
	python "$(ROOT)/tools/patch_aurora.py" "$(AUDACITY)/modules/etc/CMakeLists.txt"

clean:
	rm -rf $(BUILD)
	
distclean: clean
	rm -f "$(AUDACITY)/modules/etc/mod-aurora"
	cp "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/etc/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"
    # Remove-Item -Recurse -Force .\build
