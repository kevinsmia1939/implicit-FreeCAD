#!/usr/bin/env bash
# SPDX-License-Identifier: LGPL-2.1-or-later
set -euo pipefail
implicit_project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
implicit_gui_binary="$implicit_project_dir/build-freecad-implicit/bin/FreeCAD"
if [[ ! -x "$implicit_gui_binary" ]]; then
    echo "Native FreeCAD executable missing: $implicit_gui_binary" >&2
    echo "See docs/build-dependencies-opensuse.md for build instructions." >&2
    exit 1
fi
export QT_QPA_PLATFORM=xcb
implicit_demo_script="$implicit_project_dir/examples/interactive_demo.py"
if [[ "${1:-}" == "--live-part" ]]; then
    implicit_demo_script="$implicit_project_dir/examples/live_part_demo.py"
    shift
fi
exec "$implicit_gui_binary" --safe-mode "$implicit_demo_script" "$@"
