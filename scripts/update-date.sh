

echo "${HOME}/Library/Application Support/audacity/audacity.cfg"
grep "${HOME}/Library/Application Support/audacity/audacity.cfg" -e "mod-aurora=\d$"
grep "${HOME}/Library/Application Support/audacity/audacity.cfg" -e "mod-aurora=\d\d"
sed -i.bak 's/mod-aurora=4/mod-aurora=1/' "${HOME}/Library/Application Support/audacity/audacity.cfg"
mod-aurora=1


grep "${HOME}/Library/Application Support/audacity/audacity.cfg" -e "mod-aurora=\d\d" | sed -E 's/\d/1/'

grep "${HOME}/Library/Application Support/audacity/audacity.cfg" -e "mod-aurora=\d\d" | sed  "s/[0-9]/1/"

EXP="mod-aurora=[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9]T[0-9][0-9]:[0-9][0-9]:[0-9][0-9]"
TEST=$(stat -t '%FT%T' -x /Users/mhamilt/Documents/GitHub/Aurora-for-Audacity/build/Debug/Audacity.app/Contents/modules/mod-aurora.so | grep  Birth | grep -o -E "\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}")
sed -i.bak 's/mod-aurora=4/mod-aurora=1/' "${HOME}/Library/Application Support/audacity/audacity.cfg"
sed -i.bak "s/${EXP}/mod-aurora=${TEST}/" "${HOME}/Library/Application Support/audacity/audacity.cfg"

sed 's/mod-aurora=2023-10-12T09:53:57/mod-aurora=2023-10-12T09:53:57/
sed -i.bak 's/mod-aurora=2023-10-12T09:53:57/mod-aurora=2023-10-12T09:53:57/' "${HOME}/Library/Application Support/audacity/audacity.cfg"

TEST=$(stat -t '%FT%T' -x /Users/mhamilt/Documents/GitHub/Aurora-for-Audacity/build/Debug/Audacity.app/Contents/modules/mod-aurora.so | grep  Birth | grep -o -E "\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}")
stat -x /Users/mhamilt/Documents/GitHub/Aurora-for-Audacity/build/Debug/Audacity.app/Contents/modules/mod-aurora.so | grep Change | sed 's/\digit/1/'


echo "Thu Oct 12 09:53:57 2023" | strftime '%F'

echo "2023-10-12T09:53:57"


