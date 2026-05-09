#!/usr/bin/env python3
"""Second pass: patch_text with UTF-8 replace (legacy-encoded sources)."""
from __future__ import annotations

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SKIP_DIR_NAMES = {".git", ".cursor", "v1.0", "bin", "Debug", "Release", "lib"}
TEXT_SUFFIXES = {
    ".sln",
    ".vcxproj",
    ".vcxproj.filters",
    ".vcproj",
    ".props",
    ".xml",
    ".cpp",
    ".cc",
    ".cxx",
    ".c",
    ".h",
    ".hpp",
    ".inl",
    ".ui",
    ".qrc",
    ".pri",
    ".pro",
    ".cmake",
    ".txt",
    ".md",
    ".rc",
    ".ts",
    ".bat",
    ".pch",
    ".idl",
    ".def",
    ".lua",
    ".json",
    ".yaml",
    ".yml",
    ".svg",
    ".plist",
    ".conf",
    ".ini",
    ".css",
    ".js",
    ".html",
    ".htm",
    ".php",
    ".sql",
    ".sh",
}
EXTRA = {"Makefile", "GNUmakefile", "CMakeLists.txt", "LICENSE", "COPYING"}


def skip_walk(p: Path) -> bool:
    return p.name in SKIP_DIR_NAMES or ".git" in p.parts


def walk_files(root: Path):
    for dirpath, dirnames, filenames in os.walk(root):
        dp = Path(dirpath)
        if skip_walk(dp):
            dirnames[:] = []
            continue
        dirnames[:] = [d for d in dirnames if not skip_walk(dp / d)]
        for fn in filenames:
            yield dp / fn


def patch_text(s: str) -> str:
    compounds = [
        ("gdemclient", "qglobeclient"),
        ("gdemcustomstyle", "qglobecustomstyle"),
        ("gdemdbextractdlg", "qglobedbextractdlg"),
        ("gdem_tile", "qglobe_tile"),
        ("GDEM_", "QGLOBE_"),
    ]
    for old, new in compounds:
        s = s.replace(old, new)
    s = s.replace("gdem_", "qglobe_")
    s = s.replace("gds_", "qglobeds_")
    s = s.replace("GDM_", "QGlobe_")
    s = s.replace("gdm_", "qglobe_")
    s = re.sub(r"\bGDS(?=[A-Z0-9])", "QGlobeDS", s)
    return s


def is_text_file(path: Path) -> bool:
    if path.suffix.lower() in TEXT_SUFFIXES:
        return True
    return path.name in EXTRA


def main() -> None:
    changed = 0
    for path in walk_files(ROOT):
        if not path.is_file() or not is_text_file(path):
            continue
        try:
            raw_bytes = path.read_bytes()
        except OSError:
            continue
        if b"\x00" in raw_bytes[:8000]:
            continue
        raw = raw_bytes.decode("utf-8", errors="replace")
        new_raw = patch_text(raw)
        if new_raw == raw:
            continue
        path.write_bytes(new_raw.encode("utf-8"))
        changed += 1
        print(path.relative_to(ROOT))
    print(f"Patched {changed} files.")
    sys.exit(0)


if __name__ == "__main__":
    main()
