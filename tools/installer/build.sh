#!/bin/bash

set -e

MODULE_NAME="mod-aurora"
VERSION="3.7.8"
IDENTIFIER="com.aurora-plugins.audacity.mod-aurora"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

COMPONENT_PKG="$SCRIPT_DIR/${MODULE_NAME}-component.pkg"
FINAL_PKG="$SCRIPT_DIR/${MODULE_NAME}.pkg"

rm -f "$COMPONENT_PKG" "$FINAL_PKG"

pkgbuild \
    --root "$SCRIPT_DIR/payload" \
    --scripts "$SCRIPT_DIR/scripts" \
    --identifier "$IDENTIFIER" \
    --version "$VERSION" \
    --install-location / \
    "$COMPONENT_PKG"

echo
echo "Created $COMPONENT_PKG"

productbuild \
    --distribution "$SCRIPT_DIR/Distribution" \
    --package-path "$SCRIPT_DIR" \
    --resources "$SCRIPT_DIR/resources" \
    "$SCRIPT_DIR/${MODULE_NAME}.pkg"

echo
echo "Created $FINAL_PKG"