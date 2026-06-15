# File Compressor

A C++17 project for compressing and decompressing files.

## Motivation & Purpose

This project is a high-performance exploration of file compression using C++ and applying best practices in OOP like Dependency Injection and testing with CTest and Catch2.

Rather than relying on high-level libraries to implement file compression, I wanted to see how huffman coding in action:
* **Algorithmic Complexity:** Implementing a native Huffman Coding engine using priority queues, tree-traversal serialization, and localized frequency maps.
* **Low-Level Bit Streaming:** Developing custom bit-level stream wrappers (`file_bit_writer` and `file_bit_reader`) to allow packing fractional byte arrays seamlessly onto a raw disk subsystem.
* **Production-Grade Infrastructure:** Building a fully modern DevOps environment utilizing strict compiler memory sanitizers (ASan), local automated format validation via Git hooks, and deterministic cross-platform building via CMake Presets.

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
