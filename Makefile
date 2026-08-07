# Root directories
ROOT := $(CURDIR)
AUDACITY := $(ROOT)/audacity
MODULE := $(ROOT)/mod-aurora
BUILD := $(ROOT)/build

CMAKE_FLAGS := \
	-G Xcode \
	-Daudacity_use_mad=OFF \
	-Daudacity_use_id3tag=OFF \
	-Daudacity_conan_allow_prebuilt_binaries=OFF
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(HOME)/.local

CMAKE_RELEASE_FLAGS := \
	-G Ninja \
	-DCMAKE_BUILD_TYPE=Release \
	-DMACOS_ARCHITECTURE=arm64 \
	-Daudacity_perform_codesign=OFF \
	-DAUDACITY_BUILD_LEVEL=2 \
	-DAURORA_MODULE_PATH=$(MODULE) \
	-DCMAKE_PREFIX_PATH=$(HOME)/.local

.PHONY: xcode configure link patch clean distclean test

xcode: configure
	@echo
	@echo "Xcode project created:"
	@echo "  $(BUILD)/Audacity.xcodeproj"

audacity-only:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)

configure: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)

release-build:
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_RELEASE_FLAGS)
	cmake -B $(BUILD) --parallel

patch:
	@if ! grep -q "AURORA_MODULE_PATH" "$(AUDACITY)/modules/etc/CMakeLists.txt"; then \
		echo "Adding external mod-aurora support..."; \
		cp "$(AUDACITY)/modules/etc/CMakeLists.txt" \
		   "$(AUDACITY)/modules/etc/CMakeLists.txt.bak"; \
		printf '\n\nif(EXISTS "$${AURORA_MODULE_PATH}/CMakeLists.txt")\n' \
			>> "$(AUDACITY)/modules/etc/CMakeLists.txt"; \
		printf '   message(STATUS "Adding Aurora module: $${AURORA_MODULE_PATH}")\n' \
			>> "$(AUDACITY)/modules/etc/CMakeLists.txt"; \
		printf '   add_subdirectory("$${AURORA_MODULE_PATH}" mod-aurora)\n' \
			>> "$(AUDACITY)/modules/etc/CMakeLists.txt"; \
		printf 'endif()\n' \
			>> "$(AUDACITY)/modules/etc/CMakeLists.txt"; \
	fi

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -f "$(AUDACITY)/modules/etc/mod-aurora"
	cp "$(AUDACITY)/modules/etc/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/etc/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"

