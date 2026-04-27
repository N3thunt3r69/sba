# SBA: Scalable Binary Analysis Framework


## Announcements
The SBA framework is currently undergoing a significant overhaul:
* **Architecture & Robustness**
  - **Disassembler:** Develop a robust, architecture-agnostic disassembler leveraging *LLVM MC* for high-fidelity binary analysis. Support static analysis directly on raw bytes, bypassing the inherent flaws of traditional disassembly techniques.
  - **Binary Loading:** Integrate *LLVMObject* for reliable, cross-platform support of ELF, PE, and Mach-O executable formats.
  - **Lifting:** Implement a high-performance C++ lifter to replace the legacy OCaml pipeline.
* **Framework Capabilities**
  - **ControlFlowGraphAPI**: Support diverse graph types and construction strategies.
  - **AnalysisAPI**: Allow seamless integration of forward and backward dataflow analysis.
    - **Pre-Disassembly Analysis** [[1]](): Analyze raw bytes, breaking the circular dependency between binary analysis and traditional disassembly, and bypassing inherent flaws to provide high-fidelity results. The approach scales linearly, computing results for every byte offset of an $n$-byte binary in $O(n)$ time.
    - **Abstract Interpretation** [[2]]() [[3]]() [[4]]() [[5]](): A foundational technique in program analysis that forms the basis of modern static binary analysis. By evaluating programs over abstract domains, this engine allows the framework to model all possible execution paths without running the code.
* **Applications**
  - **Jump Table Analysis**
    - Improve bounds analysis
  - **Function Property Check**
    - Callee-Saved Registers Preservation
    - Invalid Pointer Dereference
  - **Non-Returning Call Analysis**
    - *Without relying on catalogs of non-returning functions*.


## Getting Started
### Dependencies
SBA requires a C++23 compiler (GCC/Clang), CMake, and OCaml.
We recommend using **Opam** to manage the OCaml environment, as it ensures compatibility across different Linux distributions.

#### 1. Install
```bash
# Fedora/RHEL
sudo dnf install clang make cmake ninja-build opam patch

# Ubuntu/Debian
sudo apt-get install clang make cmake ninja-build opam
```

#### 2. Configure OCaml Environment
Initialize Opam and create a switch for OCaml 4.14 (required for the legacy lifter):
```bash
opam init
opam switch create sba 4.14.2
eval $(opam env)
opam install camlp4 ocamlfind
```

### Build SBA
```bash
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=clang++
ninja -j $(nproc)
```

## Applications
### Jump Table Analysis
To analyze a binary object `~/obj`, use the following command:
```
./tools/jump_table x86_64.auto ~/obj
```
By default, SBA creates temporary files and outputs result in `/tmp/sba/`. These paths can be specified using `-d` and `-o` as follows:
```
./tools/jump_table -d /tmp/sba/ -o /tmp/sba/result x86_64.auto ~/obj
```


## Publications
1. Analyzing Bytes: Pre-Disassembly Static Binary Analysis (**PLDI 2026**)
2. [Scalable, Sound, and Accurate Jump Table Analysis](https://doi.org/10.1145/3650212.3680301) (**ISSTA 2024**)
3. [Accurate Disassembly of Complex Binaries Without Use of Compiler Metadata](https://doi.org/10.1145/3623278.3624766) (**ASPLOS 2023**)
4. [SAFER: Efficient and Error-Tolerant Binary Instrumentation](https://www.usenix.org/system/files/usenixsecurity23-priyadarshan.pdf) (**USENIX 2023**)
5. [Practical fine-grained binary code randomization](https://doi.org/10.1145/3427228.3427292) (**ACSAC 2020**)
