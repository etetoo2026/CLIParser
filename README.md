# CLIParser

> Single-header C++ argument parser library — flags, positional args, subcommands, and auto-generated help

## What it does
Drop `cliparser.hpp` into your project and get a full argument parsing library in one include. Supports boolean flags, typed options (`--count 5`), positional arguments, subcommands, default values, required args, and automatic `--help` output. Header-only, C++17, zero dependencies.

## Quick Start
```cpp
#include "cliparser.hpp"

int main(int argc, char** argv) {
    cli::Parser p("mytool", "Does useful things");
    p.add_flag("verbose", 'v', "Enable verbose output");
    p.add_option<int>("count", 'n', "Number of items", /*default*/ 10);
    p.add_option<std::string>("output", 'o', "Output file");
    p.add_positional("input", "Input file path", /*required*/ true);
    p.parse(argc, argv);

    bool verbose = p.get<bool>("verbose");
    int count    = p.get<int>("count");
    std::string input = p.get<std::string>("input");
}
```

```
$ mytool --help
Usage: mytool [OPTIONS] <input>

Does useful things

Options:
  -v, --verbose        Enable verbose output
  -n, --count <int>    Number of items (default: 10)
  -o, --output <str>   Output file
  -h, --help           Show this help message
```

## Features
- Typed options: `int`, `float`, `string`, `bool`
- Short (`-v`) and long (`--verbose`) flags
- Subcommands: `mytool build --release`
- Required vs optional args with defaults
- Automatic `--help` generation
- Error messages with "did you mean?" suggestions
- C++17, header-only, single file

## Installation
```bash
# Copy one file:
cp cliparser.hpp /your/project/include/
```
