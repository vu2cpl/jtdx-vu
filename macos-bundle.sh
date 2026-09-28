#!/bin/bash
# Build a self-contained JTDX-VU.app that runs without Homebrew.
# Output: build/bundle/JTDX-VU.app
#
# Prereq: a configured + built tree in build/ (cmake --build build).
# The CMake install step is run here into build/dist; its fixup_bundle
# stage errors on current Homebrew (@rpath libsharpyuv), but dist/jtdx.app
# is complete by then. dist/ is an intermediate and is removed at the end.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC="$ROOT/build/dist/jtdx.app"
OUT_DIR="$ROOT/build/bundle"
APP="$OUT_DIR/JTDX-VU.app"
BREW="$(brew --prefix)"
MACDEPLOYQT="$BREW/opt/qt@5/bin/macdeployqt"

rm -rf "${ROOT:?}/build/dist"
cmake --build "$ROOT/build" --target install > "$ROOT/build/install.log" 2>&1 || true
[ -x "$SRC/Contents/MacOS/jtdx" ] || { echo "install step did not produce $SRC (see build/install.log)" >&2; exit 1; }

mkdir -p "$OUT_DIR"
rm -rf "${APP:?}"
cp -R "$SRC" "$APP"
M="$APP/Contents/MacOS"
FW="$APP/Contents/Frameworks"

# rigctl*-jtdx are symlinks into the Homebrew hamlib Cellar: make them real files
for t in rigctl rigctld rigctlcom; do
  rm -f "${M:?}/$t-jtdx"
  cp "$BREW/opt/hamlib/bin/$t" "$M/$t-jtdx"
  chmod u+w "$M/$t-jtdx"
done

"$MACDEPLOYQT" "$APP" -always-overwrite \
  $(for e in jtdxjt9 udp_daemon_jtdx wsprd_jtdx rigctl-jtdx rigctld-jtdx rigctlcom-jtdx; do
      echo "-executable=$M/$e"; done) > "$OUT_DIR/deploy.log" 2>&1

# macdeployqt misses libraries only reachable through @rpath
cp -L "$BREW/opt/gcc/lib/gcc/current/libgcc_s.1.1.dylib" "$FW/"
cp -L "$BREW/opt/webp/lib/libsharpyuv.0.dylib" "$FW/"
chmod u+w "$FW"/*.dylib
for f in "$FW"/libwebp*.dylib; do
  install_name_tool -change @rpath/libsharpyuv.0.dylib @loader_path/libsharpyuv.0.dylib "$f" 2>/dev/null
done

# any remaining absolute Homebrew refs → bundled copy
while IFS= read -r f; do
  file "$f" | grep -q Mach-O || continue
  otool -L "$f" | tail -n +2 | awk '{print $1}' | { grep "^$BREW" || true; } | while read -r l; do
    b="$(basename "$l")"
    [ -f "$FW/$b" ] || cp -L "$l" "$FW/"
    install_name_tool -change "$l" "@executable_path/../Frameworks/$b" "$f" 2>/dev/null
  done
done < <(find "$APP/Contents/MacOS" -type f)

# check: no Homebrew dependency lines left
left="$(find "$APP" -type f | while IFS= read -r f; do
          file -b "$f" | grep -q Mach-O || continue
          otool -L "$f" | tail -n +3 | { grep "$BREW" || true; } | sed "s#^#$f: #"
        done)"
[ -z "$left" ] || { echo "Homebrew references remain:" >&2; echo "$left" >&2; exit 1; }

P="$APP/Contents/Info.plist"
/usr/libexec/PlistBuddy -c "Set :CFBundleName JTDX-VU" "$P"
/usr/libexec/PlistBuddy -c "Delete :CFBundleDisplayName" "$P" 2>/dev/null || true
/usr/libexec/PlistBuddy -c "Add :CFBundleDisplayName string JTDX-VU" "$P"

codesign --force --deep -s - "$APP"
codesign --verify --deep --strict "$APP"
rm -rf "${ROOT:?}/build/dist"
echo "OK: $APP ($(du -sh "$APP" | cut -f1))"
