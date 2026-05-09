#!/usr/bin/env python3
"""
Rename gdem_* / gds_* tree to qglobe_* / qglobeds_* and update text references.
Run from repo root once.
"""
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
EXTRA_NAMES = {"Makefile", "GNUmakefile", "CMakeLists.txt", "LICENSE", "COPYING"}


def skip_walk(path: Path) -> bool:
    return path.name in SKIP_DIR_NAMES or ".git" in path.parts


def transform_component(name: str) -> str:
    base, ext = os.path.splitext(name)
    stem = base

    if stem.startswith("GDS"):
        stem = "QGlobeDS" + stem[3:]

    stem = stem.replace("GDM_", "QGlobe_")
    stem = stem.replace("gdm_", "qglobe_")
    stem = stem.replace("gdem_", "qglobe_")
    stem = stem.replace("gds_", "qglobeds_")

    if stem.startswith("gdem") and not stem.startswith("gdem_"):
        stem = "qglobe" + stem[4:]

    return stem + ext


def collect_dirs(root: Path) -> list[Path]:
    out: list[Path] = []
    for dirpath, dirnames, _ in os.walk(root):
        dp = Path(dirpath)
        if skip_walk(dp):
            dirnames[:] = []
            continue
        dirnames[:] = [d for d in dirnames if not skip_walk(dp / d)]
        for d in dirnames:
            out.append(dp / d)
    return out


def collect_files(root: Path) -> list[Path]:
    out: list[Path] = []
    for dirpath, _, filenames in os.walk(root):
        dp = Path(dirpath)
        if skip_walk(dp):
            continue
        for fn in filenames:
            out.append(dp / fn)
    return out


def rename_dirs() -> None:
    dirs = collect_dirs(ROOT)
    dirs.sort(key=lambda p: len(p.parts), reverse=True)
    for d in dirs:
        new_name = transform_component(d.name)
        if new_name == d.name:
            continue
        dest = d.with_name(new_name)
        if dest.exists():
            print(f"SKIP DIR exists: {dest}")
            continue
        print(f"DIR  {d.relative_to(ROOT)} -> {new_name}")
        d.rename(dest)


def rename_files() -> None:
    files = collect_files(ROOT)
    files.sort(key=lambda p: len(p.parts), reverse=True)
    for f in files:
        if not f.is_file():
            continue
        new_name = transform_component(f.name)
        if new_name == f.name:
            continue
        dest = f.with_name(new_name)
        if dest.exists():
            print(f"SKIP FILE exists: {dest}")
            continue
        print(f"FILE {f.relative_to(ROOT)} -> {new_name}")
        f.rename(dest)


def rename_root_solutions() -> None:
    for sln in sorted(ROOT.glob("gdem_*.sln")):
        new_name = transform_component(sln.name)
        dest = sln.with_name(new_name)
        if dest.exists():
            continue
        print(f"SLN  {sln.name} -> {new_name}")
        sln.rename(dest)


def patch_text(raw: str) -> str:
    s = raw
    # Compounds without path underscores (after dirs/files renamed)
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

    # GDS-prefixed identifiers (GDSFile, GDSTcpServer, GDSFILE, …)
    s = re.sub(r"\bGDS(?=[A-Z0-9])", "QGlobeDS", s)

    return s


def is_probably_text(path: Path) -> bool:
    if path.suffix.lower() in TEXT_SUFFIXES:
        return True
    if path.name in EXTRA_NAMES:
        return True
    return False


def apply_text_replacements() -> None:
    changed = 0
    for path in collect_files(ROOT):
        if not path.is_file() or not is_probably_text(path):
            continue
        try:
            raw_bytes = path.read_bytes()
        except OSError:
            continue
        if b"\x00" in raw_bytes[:8000]:
            continue
        try:
            raw = raw_bytes.decode("utf-8")
        except UnicodeDecodeError:
            try:
                raw = raw_bytes.decode("utf-8-sig")
            except UnicodeDecodeError:
                continue

        new_raw = patch_text(raw)
        if new_raw == raw:
            continue

        path.write_bytes(new_raw.encode("utf-8"))
        changed += 1
        print(f"TXT  {path.relative_to(ROOT)}")
    print(f"Patched {changed} text files.")


def main() -> None:
    os.chdir(ROOT)
    print(f"ROOT={ROOT}")
    rename_dirs()
    rename_files()
    rename_root_solutions()
    apply_text_replacements()
    print("Done.")


if __name__ == "__main__":
    main()
    sys.exit(0)
