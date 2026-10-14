# SPDX-License-Identifier: LGPL-2.1-or-later
"""Run a command on an isolated Xvfb display without requiring xvfb-run."""
import os
import shutil
import subprocess
import sys


def main():
    if len(sys.argv) < 2:
        raise SystemExit("usage: python3 tools/with_xvfb.py COMMAND [ARG ...]")
    executable = shutil.which("Xvfb")
    if executable is None:
        raise SystemExit("Xvfb is not installed; use an existing desktop DISPLAY instead")
    server = subprocess.Popen([executable, "-displayfd", "1", "-screen", "0", "1280x1024x24",
                               "-nolisten", "tcp"], stdout=subprocess.PIPE, text=True)
    try:
        display = server.stdout.readline().strip()
        if not display.isdigit():
            raise RuntimeError("Xvfb did not provide a display number")
        environment = os.environ.copy()
        environment.update(DISPLAY=":" + display, QT_QPA_PLATFORM="xcb", LIBGL_ALWAYS_SOFTWARE="1")
        return subprocess.run(sys.argv[1:], env=environment).returncode
    finally:
        server.terminate()
        try:
            server.wait(timeout=10)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait()


if __name__ == "__main__":
    raise SystemExit(main())
