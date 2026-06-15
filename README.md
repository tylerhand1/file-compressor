# File Compressor

A C++17 project for compressing and decompressing files.

## Tech Stack & Tools
* **Build System:** CMake (Version 3.14+)
* **Testing Framework:** Catch2 & CTest
* **Code Quality:** Native Git Hook via CMake (Customized LLVM-style Clang-Format)
* **CI Pipeline:** GitHub Actions (Validates styling compliance, compiles with `ccache`, and executes unit tests on push)

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


---

## Memory Diagnostics (AddressSanitizer)

To catch critical bugs like memory leaks, out-of-bounds array accesses, and pointer corruptions early, this project includes a dedicated **AddressSanitizer (ASan)** configuration that can be run natively on your local machine:

```bash
cmake --preset asan
cmake --build --preset asan

ctest --preset run-asan-tests --test-dir out/build/asan
```

---

## Code Style & Formatting

This project enforces a strict, customized **LLVM style** (4-space indentation, left-aligned pointers). 

To ensure consistency, local commits will be rejected if code formatting rules are broken. You can automatically reformat your staging branch files at any time by running:

```bash
cmake --build out/build/default-debug --target format
```
