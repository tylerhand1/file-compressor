# File Compressor

A C++17 project for compressing and decompressing files by implementing Huffman coding.

## Motivation & Purpose

I chose to build this out of curiosity for file compression after utilizing `compressorJs` in a separate project. So I decided to try it in C++ for a challenge. This project handles text and binary files, achieving up to a 44% reduction in file size for standard English text.

I built the engine using modern object-oriented software engineering principles, featuring automated memory safety, dependency inversion, and built-in telemetry of the compression.

## Key Architectural Decisions & Engineering Highlights

* **Modern Memory Ownership Models**: Enforced compile-time memory safety by managing the binary tree hierarchy entirely through `std::unique_ptr`.
* **Decoupled Dependency Injection (DI)**: Designed the compression engine around strict DI principles by injecting the abstract output `BitWriter` interface via the constructor. This decouples the core compression logic from the underlying storage medium, allowing seamless extensibility for alternative output destinations.
* **State-Safe Stream Paradigms**: Isolated runtime input dependencies by passing the `BitReader` interface directly to the decompression method.
* **Robust Boundary Safeguards**: Implemented bounds checking inside the data streaming modules to prevent runtime memory violations during tree traversal.
* **Production-Grade Infrastructure**: Formulated a scalable verification pipeline utilizing CMake compilation targets, cross-platform presets, AddressSanitizer (ASan) memory auditing, and Catch2 unit-testing.


## Tech Stack & Tools
* **Build System:** Modern CMake (Version 3.14+) utilizing **CMake Presets**
* **Testing Framework:** Catch2 & CTest
* **Memory Diagnostics:** AddressSanitizer (ASan) built into the compiler pipelines
* **Code Quality:** Native Git Hook via CMake (Customized LLVM-style Clang-Format)
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
ctest --preset run-asan-tests-mac --test-dir out/build/asan
```

#### Run on Linux (Linux CI / GitHub Actions)
On Linux environments, full tracking is executed back-to-back, catching traditional memory leaks alongside standard corruption:
```bash
ctest --preset run-asan-tests --test-dir out/build/asan
```

---

## Code Style & Formatting

This project enforces a strict, customized **LLVM style** (4-space indentation, left-aligned pointers). 

To ensure consistency, local commits will be rejected if code formatting rules are broken. You can automatically reformat your staging branch files at any time by running:

```bash
cmake --build out/build/default-debug --target format
```
