# File Compressor

A C++17 project for compressing and decompressing files by implementing Huffman coding.

## Motivation & Purpose

I chose to build this out of curiosity for file compression after utilizing `Compressor.js` in a separate project. So I decided to try it in C++ for a challenge. This project handles text and binary files, achieving up to a 44% reduction in file size for standard English text.

I built the engine using modern object-oriented software engineering principles, featuring automated memory safety, dependency inversion, and terminal output tracking to log compression ratios.

## Key Architectural Decisions & Engineering Highlights

* **Smart Pointers**: Enforced memory safety by managing the binary tree entirely through `std::unique_ptr` and ensuring automatic cleanup.
* **Dependency Injection (DI)**: Designed the compression engine around strict DI principles by injecting the abstract output `BitWriter` interface to the functions that require I/O. This decouples the core compression logic from the underlying storage medium, allowing seamless extensibility for alternative output destinations.
* **Encapsulated Bit-wise Logic**: Isolated bit-wise logic into `BitWriter` and `BitReader` classes to keep the core `HuffmanCompressor` class clean.
* **Safe Bitstreaming Operations**: Configured index validation within the I/O data streams to ensure no run-time memory violations during binary tree parsing.
* **Automated CI Pipeline**: Configured CMake presets, Catch2 unit tests, and AddressSanitizer (ASan) memory profiling to catch memory leaks and overflows automatically in CI.


## Tech Stack & Tools
* **Build System:** Modern CMake (Version 3.14+) utilizing **CMake Presets**
* **Testing Framework:** Catch2 & CTest
* **Memory Diagnostics:** AddressSanitizer (ASan)
* **Code Quality & Linting:** Utilizing `pre-commit` framework and `clang-format` checks
* **CI Pipeline:** GitHub Actions (Validates formatting compliance, compiles with `ccache`, and executes parallel unit tests and ASan leak/memory analysis on push)

---

## Getting Started

### 1. Prerequisites
Ensure the following tools are installed globally on your host system:
* A C++17 compatible compiler (GCC, Clang, or MSVC)
* **CMake** (Version 3.14 or higher)
* **Clang-Format** (Version 14 or higher)

### 2. Configure the Project
Generate the build system using the unified debug preset. This step automatically provisions and activates the local repository formatting validation hooks.
```bash
cmake --preset default-debug
```

### 3. Build the Project
Compile the source code and testing suites using the native build target tracking:
```bash
cmake --build --preset default-debug
```

### 4. Run the Tests
Execute your standard unit test assertions using the native automation configuration:
```bash
ctest --preset run-tests --test-dir out/build/default-debug
```

### 5. Production Release (Optimized Build)
To compile the final production build:
```bash
cmake --preset production
cmake --build --preset production
```
The optimized executable will be generated inside the `out/build/production/src/file-compressor` folder.

---

## CLI Usage & Streaming Rules

The binary acts as a standard command-line pipe utility, automatically guarding against terminal hanging if no stream is detected.

```bash
# Compress a file
./out/build/default-debug/src/file-compressor -c < input.txt > output.huff

# Decompress a file
./out/build/default-debug/src/file-compressor -d < output.huff > restored.txt
```

---

## Memory Diagnostics (AddressSanitizer)

To catch critical bugs like memory leaks, out-of-bounds array accesses, and pointer corruptions early, this project includes dedicated **AddressSanitizer (ASan)** configurations.

#### Run Natively on macOS (MacBook Air)
On macOS, run with leak detection deactivated to comply with Apple's development tool constraints, while retaining full tracking for buffer overflows and memory corruption:
```bash
cmake --preset asan
cmake --build --preset asan
ctest --preset run-asan-tests-mac
```

#### Run on Linux (Linux CI / GitHub Actions)
On Linux environments, full tracking is executed back-to-back, catching traditional memory leaks alongside standard corruption:
```bash
ctest --preset run-asan-tests
```

---

## Code Style & Formatting

This project enforces a strict, customized **LLVM style** (4-space indentation, left-aligned pointers).

To ensure consistency, local commits will be rejected if code formatting rules are broken. You can automatically reformat your staging branch files at any time by running:

```bash
pre-commit run --all-files
```
