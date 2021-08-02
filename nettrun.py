#!/usr/bin/env python3
#
# nettrun.py
#
# Acts as a wrapper around the nett Docker image
# in order to simplify invocation
#
# Usage: nettrun.py [--generate-files] file [file...]
import pathlib
import argparse
import types
import subprocess

def create_argument_parser():
    """
    Creates and returns a new argument parser for the program
    """
    parser = argparse.ArgumentParser(
        prog="nettrun",
        description="A wrapper script around the nett Docker image"
        )
    parser.add_argument(
        "file", 
        nargs="+",
        help="The files to check"
        )
    parser.add_argument(
        "--generate-files",
        action='store_true',
        help="Generate .styled files instead of terminal output"
        )
    return parser

def resolve_file_list(filepaths):
    """
    Returns a SimpleNamespace containing:
        - (files) the list of unique resolved filepaths to 
            be passed to the nett program
        - (parents) the list of unique direct parent directories
            related to the provided filepaths
    
    Note that directory paths and non-existing files
    are not included in the final result.
    """
    parent_dirs = set()
    resolved_filepaths = set()

    for path in filepaths:
        resolved = pathlib.Path(path).resolve()

        if not resolved.is_file():
            continue
        resolved_filepaths.add(resolved)
        parent_dirs.add(resolved.parent)

    result = types.SimpleNamespace()
    result.files = list(resolved_filepaths)
    result.parents = list(parent_dirs)
    return result

def run_nett_docker_image(should_generate_files, mount_directories, filepaths):
    """
    Runs the nett docker image with the appropriate flags
    and filepaths.
    """
    args = ["docker", "run", "-it"]

    if mount_directories:
        for dir in mount_directories:
            args.append("-v")
            args.append(str(dir) + ":" + str(dir))

    args.append("dgrobb/nett")

    if should_generate_files:
        args.append("--generate-files")

    for file in filepaths:
        args.append(str(file))

    subprocess.run(args)

if __name__ == "__main__":
    parser = create_argument_parser()
    args = parser.parse_args()
    
    resolved = resolve_file_list(args.file)
    
    run_nett_docker_image(
        args.generate_files, 
        resolved.parents, 
        resolved.files
        )