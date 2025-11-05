# Advanced Mathematical Text Editor

A sophisticated C-based text editor with integrated mathematical expression evaluation and equation solving capabilities.

## 🌟 Features

### 📝 Text Editor
- **Dynamic Text Management**: Doubly linked list-based line storage
- **Line Operations**: Insert, delete, edit, and navigate text lines
- **File I/O**: Save and load text documents
- **Interactive Display**: Line numbering with current position marker

### 🧮 Mathematical Engine
- **Expression Evaluation**: Complex mathematical expressions with operator precedence
- **Variable Management**: Store and reuse variables using AVL Binary Search Tree
- **Function Library**: 10 built-in mathematical functions (sin, cos, tan, sqrt, log, ln, abs, ceil, floor, exp)
- **Smart Formatting**: Automatic integer/decimal result display

### 📐 Equation Solvers
- **Quadratic Solver**: Handles real, complex, and repeated roots
- **Cubic Solver**: Advanced Cardano's method for all cubic equation types
- **Error Handling**: Comprehensive domain validation and user-friendly error messages

### 🛡️ Robust Error Handling
- **Mathematical Errors**: Division by zero, square root of negatives, logarithm domain errors
- **Input Validation**: Malformed expressions, invalid commands
- **Overflow Protection**: Factorial limits, power operation validation

## 🚀 Getting Started

### Prerequisites
- GCC compiler with math library support
- Windows/Linux/macOS terminal

### Compilation
```bash
gcc -g -o main main.c editor.c command_parser.c expression_parser.c evaluator.c stack.c utils.c hash_functions.c bst_variables.c -lm
```

### Running
```bash
./main
```

## 📖 Usage Guide

### Basic Commands
```
:help                    # Show comprehensive help
:eval(expression)        # Evaluate mathematical expressions
:solve_quad(a,b,c)      # Solve quadratic equations
:solve_cubic(a,b,c,d)   # Solve cubic equations
:set var = value        # Store variables
:vars                   # List all variables
:functions              # List available functions
:quit                   # Exit program
```

### Text Editor Commands
```
:show                   # Display all lines with numbers
:insert N text          # Insert text at line N
:delete N              # Delete line N
:clear                 # Clear all text
:save filename         # Save to file
:load filename         # Load from file
```

### Examples

#### Mathematical Expressions
```
:eval(2+3*4)           # Result: 14
:eval(sin(30)+cos(60)) # Result: 1.000000
:eval(sqrt(16)+log(1000)) # Result: 7
```

#### Variable Usage
```
:set pi = 3.14159
:set radius = 5
:eval(pi * radius^2)   # Result: 78.539816
```

#### Equation Solving
```
:solve_quad(1,-5,6)    # Solutions: x=3, x=2
:solve_cubic(1,-6,11,-6) # Solutions: x=3, x=2, x=1
```

## 🏗️ Architecture

### Data Structures
- **AVL Binary Search Tree**: Variable storage with O(log n) operations
- **Hash Table**: Function lookup with O(1) average case
- **Doubly Linked List**: Dynamic text line management
- **Stack**: Expression evaluation and parsing

### Core Components
- **`main.c`**: Program controller and user interface
- **`evaluator.c`**: Mathematical expression evaluation engine
- **`expression_parser.c`**: Infix to postfix conversion
- **`bst_variables.c`**: Variable management using AVL tree
- **`hash_functions.c`**: Function table management
- **`editor.c`**: Text editing functionality
- **`utils.c`**: Integration and utility functions

### Mathematical Algorithms
- **Shunting Yard Algorithm**: Expression parsing
- **Postfix Evaluation**: Stack-based computation
- **Quadratic Formula**: Standard quadratic solver
- **Cardano's Method**: Cubic equation solver with depressed cubic transformation

## 🧪 Testing

The project includes comprehensive testing for:
- ✅ All mathematical operations and functions
- ✅ Variable management and persistence
- ✅ Equation solvers (all root types)
- ✅ Error handling and edge cases
- ✅ Text editor functionality
- ✅ File I/O operations

## 🛠️ Development

### Build System
- **Manual Compilation**: GCC with explicit file listing
- **VS Code Integration**: Tasks.json for build automation
- **Cross-Platform**: Compatible with Windows, Linux, macOS

### Code Quality
- **Memory Management**: Proper allocation/deallocation
- **Error Handling**: Comprehensive validation
- **Documentation**: Extensive inline comments
- **Modularity**: Clean separation of concerns

## 📊 Performance

- **Variable Lookup**: O(log n) with AVL tree
- **Function Calls**: O(1) average with hash table
- **Expression Evaluation**: O(n) linear parsing
- **Memory Usage**: Dynamic allocation with cleanup

## 🎯 Use Cases

- **Educational**: Teaching mathematical concepts and programming
- **Engineering**: Quick calculations and equation solving
- **Research**: Mathematical expression testing and validation
- **General**: Advanced calculator with text editing capabilities

## 📝 License

This project is part of the DS-PBL (Data Structures - Project Based Learning) coursework.

## � Authors

**Team Members:**
- **Yashaeron12** - Project Lead & Core Implementation
- **Arjun Singh** - Team Member & Development Contributor
- **Vivek Choudhary** - Team Member & Development Contributor  
- **Vidyansh Dwivedi** - Team Member & Development Contributor

*Advanced Mathematical Text Editor - A collaborative DS-PBL project*

---

*Built with C, powered by advanced data structures and mathematical algorithms.*