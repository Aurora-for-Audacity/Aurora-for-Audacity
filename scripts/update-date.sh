# Xcode Post build script to update Audacity config to avoid having to set the module to "Enabled every time"
EXP="mod-aurora=[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]T[0-9][0-9]:[0-9][0-9]:[0-9][0-9]"
TEST=$(stat -t '%FT%T' -x ${CODESIGNING_FOLDER_PATH} | grep  Birth | grep -o -E "\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}")
sed -i.bak 's/mod-aurora=4/mod-aurora=1/' "${HOME}/Library/Application Support/audacity/audacity.cfg"
sed -i.bak "s/${EXP}/mod-aurora=${TEST}/" "${HOME}/Library/Application Support/audacity/audacity.cfg"

