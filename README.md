# unbox
Dumps s&box game packages straight from memory.

## Background

s&box compiles game packages (e.g. `package.author.gamename`) at runtime using
Roslyn on .NET 10 — the resulting assemblies exist only in process memory and
never touch disk. unbox scans the memory of a running s&box instance, identifies these
in-memory PE images, and writes them as `.unboxed` files you can inspect with
dnSpyEx or ILSpy.

## Demo
<img width="1155" height="375" alt="dump" src="https://github.com/user-attachments/assets/d77b6dee-c0e4-49d0-bac9-5d7e38c24069" />
<img width="500" height="426" alt="dn" src="https://github.com/user-attachments/assets/5903bdc0-ef3c-4122-b34a-f9504db3acb5" />

## Usage
```
unbox -o <output_dir>
```

| Flag | Description |
|------|-------------|
| `-o` | Directory where dumped assemblies will be written |
| `-v` | Activate the verbose mode |


**Example**
```
unbox -o ./dumps
```

## Build

Requires CMake 3.20+, LLVM/clang-cl, Ninja, and an x64 MSVC environment.
```bash
cmake --preset default
cmake --build build
```

## Notes

- s&box must be running with a package loaded before dumping
- Run as administrator or ensure sufficient permissions to read sbox.exe memory
- Dumped files are valid PE loadable by dnSpyEx

## Disclaimer

For personal research only. The author is not responsible for any misuse. Respect other developers' work.
