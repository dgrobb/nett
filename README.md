# nett
`nett` is command-line utility used to perform style
checking on C source files. It checks for
compliance with the style guide used by the 
CSSE2310 programming course at the University of Queensland.

## Usage

The program has the following modes of operation:

1. Perform style checking on a set of source files
    and output all found violations to the terminal
    (default)

    ```
    nett file1 [file2 ...]
    ```

2. Perform style checking on a set of source files
    and generate `.styled` files containing a
    capped number of violations found.

    ```
    nett --generate-files file1 [file2 ...]
    ```
    The generated `.styled` files will be created in the same
    directory as the original source files provided.

## Installation
`nett` is written using the Clang Libtooling framework.
In order to build the program, its source code is
required to be stored within a checked-out LLVM git
repository under `llvm-project/clang-tools-extra`.

To do this, run the following:

```
git clone https://github.com/llvm/llvm-project.git && \
  cd llvm-project && \
  git checkout 82fbc5d45b0c2fc9050d1d5e335e35afb4ab2611 && \
  cd clang-tools-extra && \
  git submodule add https://github.com/dgrobb/nett.git && \
  cd nett
```

Once the `nett` source files are in place (under
`llvm-project/clang-tools-extra`), it can be built
using the provided build script

```
./build.sh
```

> Note that the build process requires 
> [`cmake`](https://cmake.org/install/) and 
> [`ninja`](https://ninja-build.org) 
> to be installed beforehand. 

## Tests

To check that the program works correctly, you can run the 
test suite using the provided test script

```
./runtests.sh
```

## Docker Compatibility
`nett` is also available via Docker Hub. To build the docker image
from scratch, run

```
docker build -t nett .
```

To run the program via the publicly available docker image
(`dgrobb/nett`), you can use the provided wrapper script:

```
./nettrun.py [--generate-files] file [file ...]
```

This script wraps the `docker run` command and takes care of 
mounting the necessary directories for the program to work correctly.

## License

`nett` is licensed under the GNU General Public License v3.0. 
