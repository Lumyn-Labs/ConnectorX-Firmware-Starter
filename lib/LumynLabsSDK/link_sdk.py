"""
LumynLabsSDK Build Script

This script configures the linker to properly link the pre-compiled SDK library.
It runs as part of PlatformIO's build process via the extraScript in library.json.
"""

import os
Import("env")

script_dir = Dir('.').srcnode().abspath
lib_path = os.path.join(script_dir, "lib")
lib_file = os.path.join(lib_path, "libLumynLabsSDK.a")

lib_path = lib_path.replace("\\", "/")
lib_file = lib_file.replace("\\", "/")

if os.path.exists(lib_file):
    print(f"[LumynLabsSDK] Library: {lib_file}")
    env.Append(LIBPATH=[lib_path])
    env.Append(LINKFLAGS=[
        "-Wl,--whole-archive",
        lib_file,
        "-Wl,--no-whole-archive"
    ])
    print("[LumynLabsSDK] Configured linker with --whole-archive for pre-compiled SDK")
else:
    print(f"[LumynLabsSDK] Warning: Library not found: {lib_file}")
