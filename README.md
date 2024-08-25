
# Simple Interpreter

This program is a simple interpreter for a subset of the C programming language, specifically designed to handle basic operations such as integer and string variable declarations, assignments, arithmetic operations, string concatenations, and input/output operations. The program processes the input line by line and maintains a symbol table to keep track of declared variables.

## Features

- **Integer and String Declarations**: Supports the declaration of integer and string variables.
- **Assignment Operations**: Handles assignments for both integer and string variables.
- **Arithmetic Operations**: Supports basic arithmetic operations (`+`, `-`, `*`, `/`) on integer variables.
- **String Concatenation**: Allows concatenation of string variables.
- **Input and Output**: Supports `scanf` for user input and `print` for output.

## Input Syntax

The interpreter recognizes the following commands:

### 1. Integer Declaration and Assignment

- **Syntax**: `int <variable_name> = <expression>` or `int <variable_name>`
- **Example**:
  ```c
  int a = 5
  int b = a + 3
  int c
  ```

### 2. String Declaration and Assignment

- **Syntax**: `string <variable_name> = "<value>"` or `string <variable_name>`
- **Example**:
  ```c
  string s = "hello"
  string t = s + " world"
  string u
  ```

### 3. Arithmetic Operations

- **Syntax**: `<variable_name> = <expression>`
- **Example**:
  ```c
  int x = a + b * 2
  ```

### 4. String Concatenation

- **Syntax**: `<variable_name> = <string_expression>`
- **Example**:
  ```c
  string greeting = "hello" + " world"
  ```

### 5. Print Statement

- **Syntax**: `print <variable_name>`
- **Example**:
  ```c
  print a
  print greeting
  ```

### 6. Scanf Statement

- **Syntax**: `scanf <variable_name>`
- **Example**:
  ```c
  scanf a
  scanf s
  ```

### 7. Exit Command

- **Syntax**: `exit`
- **Description**: Terminates the interpreter.

## Error Handling

The interpreter provides basic error handling, including:
- **Undefined Variables**: Reports an error if a variable is used before it is declared.
- **Uninitialized Variables**: Reports an error if a variable is used before it is initialized.
- **Type Mismatch**: Reports an error if operations are attempted between incompatible types (e.g., adding a string and an integer).
- **Division by Zero**: Reports an error if an attempt is made to divide by zero.

## Running the Program

1. Compile the program using a C compiler:
   ```bash
   gcc sample.c -o interpreter
   ```

2. Run the interpreter:
   ```bash
   ./interpreter
   ```

3. Start typing commands as per the syntax described above. The interpreter will continue to process commands until you type `exit`.

## Example Usage

```c
>>> int a = 10
>>> int b = 20
>>> int c = a + b
>>> print c
30
>>> string s = "Hello"
>>> string t = s + " World"
>>> print t
Hello World
>>> scanf c
Enter value for c: 50
>>> print c
50
>>> exit
```

## Limitations

- The interpreter currently supports only basic integer arithmetic and string concatenation.
- No support for floating-point numbers or complex data types.
- No support for control structures (e.g., loops, conditionals).

