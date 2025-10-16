# Compiler from Scratch in C

Professional compiler implementation with **LL & LR parsing**, **GUI interface**, and **x86-64 assembly generation**.

## 🚀 Quick Start

```bash
# Build and run
build_all.bat
# Select: 1 for GUI (Recommended) or 2 for Console

# Then try: id + id * id
```

## ✨ Features

### Two Versions:
1. **GUI Compiler** (`compiler_gui.exe`) ⭐
   - Windows graphical interface
   - Tabbed output: Tokens | LL Parser | LR Parser | Assembly
   - Direct input/output with one-click compile
   
2. **Console Compiler** (`compiler.exe`)
   - Interactive menu system
   - Step-by-step phase control

### Core Components:
- ✅ **Lexical Analyzer** - Tokenization
- ✅ **LL Parser** - Top-down predictive parsing
- ✅ **LR Parser** - Bottom-up shift-reduce parsing
- ✅ **Code Generator** - x86-64 assembly output
- ✅ **3 Sample Grammars** - Ready to use

## 📁 Project Structure

```
Core:
├── compiler.h       # Main header
├── lexer.c         # Tokenizer
├── ll_parser.c     # LL parser
├── lr_parser.c     # LR parser
├── utils.c         # Utilities
├── codegen.c/h     # Assembly generator

Applications:
├── gui_compiler.c  # GUI version
├── main.c          # Console version
└── build_all.bat   # Unified build
```

## 🔧 Building

### Easy Build (Recommended):
```bash
build_all.bat
# Select: 1=GUI, 2=Console, 3=Both
```

### Manual Build:
```bash
# GUI Version
gcc -O2 -mwindows -o compiler_gui.exe gui_compiler.c lexer.c ll_parser.c lr_parser.c utils.c codegen.c -lcomctl32 -lgdi32

# Console Version  
gcc -O2 -o compiler.exe main.c lexer.c ll_parser.c lr_parser.c utils.c
```

## 🎯 Usage

### GUI (Best for demos):
1. Run `compiler_gui.exe`
2. Select grammar (Grammar 3 recommended)
3. Enter: `id + id * id`
4. Click COMPILE
5. View tabs: Tokens → LL → LR → Assembly

### Console (Best for learning):
1. Run `compiler.exe`
2. Press 1 → Select grammar 3
3. Press 9 → Enter `id + id * id`
4. See all phases execute

## 📚 Sample Grammars

### Grammar 1: Arithmetic (Left Recursive)
```
E -> E + T | T
T -> T * F | F  
F -> ( E ) | id
```

### Grammar 2: Simple LL(1)
```
S -> A B | A -> a | ε | B -> b
```

### Grammar 3: Expression (LL Compatible) ⭐
```
E  -> T E' | E' -> + T E' | ε
T  -> F T' | T' -> * F T' | ε
F  -> ( E ) | id
```

**Test**: `id`, `id + id`, `id * id`, `id + id * id`, `(id + id) * id`

## 📊 Output Examples

### GUI Assembly Tab:
```asm
; Three-Address Code
t0 = id
t1 = id * id
t2 = t0 + t1

; x86-64 Assembly
section .data
    t0: dq 0
section .text
    mov rax, [id]
    imul rax, [id]
    add rax, [id]
```

### Console LL Parser:
```
STACK          INPUT          ACTION
E $            id + id $      Use E -> T E'
T E' $         id + id $      Use T -> F T'
...
✓ ACCEPT
```

## 🎓 What You'll Learn

- Tokenization & pattern matching
- Context-free grammars
- FIRST & FOLLOW sets
- Parsing table construction
- Stack-based parsing (LL & LR)
- Three-address code
- x86-64 assembly generation

## 🛠️ Customization

Add your own grammar in `utils.c`:
```c
case 4: {
    strcpy(g->start_symbol, "S");
    strcpy(g->productions[0].lhs, "S");
    // ... define your grammar
}
```

## 📈 Technical Specs

- **Language**: C11 standard
- **LOC**: ~1800 lines
- **Optimization**: -O2 enabled
- **Memory Safe**: Buffer overflow protection
- **Platform**: Windows (GUI), Cross-platform (Console)

## 🆘 Troubleshooting

**Build fails**: Install MinGW, add to PATH  
**GUI won't start**: Windows only, check GCC installation  
**Parse errors**: Verify input matches selected grammar  

---

**Status**: ✅ Complete | 🎯 Assignment Ready | 📚 Educational

For detailed documentation, see `README_COMPLETE.md`
