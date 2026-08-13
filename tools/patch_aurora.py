from pathlib import Path
import sys

cmake_file = Path(sys.argv[1])
backup = cmake_file.with_suffix(".bak")

marker = "AURORA_MODULE_PATH"

text = cmake_file.read_text()

if marker not in text:
    print("Adding external mod-aurora support...")

    backup.write_text(text)

    text += r'''

if(EXISTS "${AURORA_MODULE_PATH}/CMakeLists.txt")
    message(STATUS "Adding Aurora module: ${AURORA_MODULE_PATH}")
    add_subdirectory("${AURORA_MODULE_PATH}" mod-aurora)
endif()
'''

    cmake_file.write_text(text)
else:
    print("Aurora module support already present")