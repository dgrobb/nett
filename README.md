# nett
Nett is command-line utility used to perform style
checking on C source files in order to check for
compliance with the CSSE2310 style guide.

## Usage

The utility has the following modes of operation:

1. Perform style checking on a set of source files
    and output all found violations to the terminal
    (default)

    ```
    $ ./nett source1 [source2, ..., sourceN]
    ```

2. Perform style checking on a set of source files
    and generate `.styled` files containing a
    capped number of violations found.

    ```
    $ ./nett --generate-files source1 [source2, ..., sourceN]
    ```

## Style Violations

Nett generates style violations under the following
categories:

* NAMING
    * Relates to incorrect naming of types,
        variables, functions, constants and file names.
* COMMENTS
    * Relates to functions and variables
        not having any associated comments.
* BRACES
    * Relates to incorrect brace positioning.
* WHITESPACE
    * Relates to spacing issues between 
        variables and operators, and pointer spacing
        consistency and style.
* INDENTATION
    * Relates to incorrect indentation of
        statements in relation to their parents.
* LINE_LENGTH
    * Relates to the lengths of lines within a
        source file.
* OVERALL
    * A generic category of style violation which
        relates to things which are bad practice
        or which make code unclear/unreadable.
* WARNING
    * Relates to the use of banned language features.

## Installation
Nett is written using the Clang Libtooling framework.
In order to build the program, its source code is
required to be stored within a checked-out LLVM git
repository under `llvm-project/clang-tools-extra`.

The repository can be cloned using

```
$ git clone --branch llvmorg-10.0.0 https://github.com/llvm/llvm-project.git
```

and the Nett source files can be added via a git 
submodule using
```
$ cd llvm-project/clang-tools-extra
$ git submodule add https://github.com/dgrobb/nett.git
```
Once the Nett source files are in place (under
`llvm-project/clang-tools-extra`), it can be built
as follows:

```
$ cd llvm-project
$ echo 'add_subdirectory(nett)' >> clang-tools-extra/CMakeLists.txt
$ mkdir build
$ cd build
$ cmake -G Ninja ../llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DCMAKE_BUILD_TYPE:STRING=Release
$ ninja nett

``` 

Note that the build process requires the `ninja` build
tool. This can be found at <https://ninja-build.org/>.

Once built, the `nett` executable is available under
`llvm-project/build/bin`. If Nett needs to be rebuilt,
the following can be run:

```
$ cd llvm-project/build
$ ninja nett
```

## Tests

To check that the program runs correctly, you can run the 
test suite as follows:

```
$ python3 nett/test/runner.py
```

## License

Nett is licensed under the GNU General Public
License v3.0. 
