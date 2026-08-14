#!/bin/bash
set -e

MODULE="$1"

if [ -z "$MODULE" ]; then
    echo "Usage: $0 <module.so>"
    exit 1
fi

if [ ! -f "$MODULE" ]; then
    echo "ERROR: Module not found: $MODULE"
    exit 1
fi

otool -L "$MODULE" |
    awk '/@loader_path\/\.\.\/Frameworks\// {
        print $1
    }' |
    while read -r OLD
    do
        NEW="${OLD/@loader_path/@executable_path}"

        echo "$OLD"
        echo " -> $NEW"

        install_name_tool \
            -change "$OLD" "$NEW" \
            "$MODULE"
    done