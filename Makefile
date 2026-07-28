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

.PHONY: xcode configure link patch clean distclean

xcode: configure
	@echo
	@echo "Xcode project created:"
	@echo "  $(BUILD)/Audacity.xcodeproj"

configure: patch
	cmake -S $(AUDACITY) -B $(BUILD) $(CMAKE_FLAGS)

link:
	@if [ ! -d "$(AUDACITY)" ]; then \
		echo ""; \
		echo "Audacity submodule not found."; \
		echo "Run:"; \
		echo "  git submodule update --init --recursive"; \
		exit 1; \
	fi

	@if [ ! -L "$(AUDACITY)/modules/mod-aurora" ] && \
	   [ ! -d "$(AUDACITY)/modules/mod-aurora" ]; then \
		echo "Linking mod-aurora..."; \
		ln -s "$(MODULE)" "$(AUDACITY)/modules/mod-aurora"; \
	fi

patch: link 
# 	@if ! grep -q "mod-aurora" "$(AUDACITY)/modules/CMakeLists.txt"; then \
# 		echo "Adding mod-aurora to Audacity..."; \
# 		cp "$(AUDACITY)/modules/CMakeLists.txt" \
# 		   "$(AUDACITY)/modules/CMakeLists.txt.bak"; \
# 		awk '\
# 			/mod-script-pipe/ { \
# 				print; \
# 				print "   mod-aurora"; \
# 				next; \
# 			} \
# 			{ print }' \
# 			"$(AUDACITY)/modules/CMakeLists.txt.bak" \
# 			> "$(AUDACITY)/modules/CMakeLists.txt"; \
# 	fi

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -f "$(AUDACITY)/modules/mod-aurora"
	cp "$(AUDACITY)/modules/CMakeLists.txt.bak" \
	   "$(AUDACITY)/modules/CMakeLists.txt" 2>/dev/null || true
	rm -f "$(AUDACITY)/modules/CMakeLists.txt.bak"