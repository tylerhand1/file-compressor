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
Generate the build system. This step automatically provisions and activates the local repository formatting validation hooks.
```bash
cmake -B build -S .
```

### 3. Build the Project
Compile the source code and testing suites:
```bash
cmake --build build
```

### 4. Run the Tests
Execute the compiled Catch2 test suites using CTest:
```bash
cd build && ctest --output-on-failure
```

---

## Code Style & Formatting

This project enforces a strict, customized **LLVM style** (4-space indentation, left-aligned pointers). 

To ensure consistency, local commits will be rejected if code formatting rules are broken. You can automatically reformat your staging branch files at any time by running:

```bash
cmake --build build --target format
```
