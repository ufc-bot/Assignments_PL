#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VARS 100

// Variable struct to store the name and value of a variable
struct Variable {
  char name[100];
  int intValue;
  char strValue[100];
  int isString;      // 1 if string, 0 if integer
  int isInitialized; // 1 if initialized, 0 if not
};

// Global symbol table
struct Variable symbolTable[MAX_VARS];
int symbolCount = 0;

// Function to find a variable in the symbol table
int findVariable(const char *name) {
  for (int i = 0; i < symbolCount; i++) {
    if (strcmp(symbolTable[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

// Function to evaluate an arithmetic expression (only for integers)
int evaluateExpression(char *expr) {
  int value = 0;
  char operator= '+';
  char token[100];
  int idx = 0;

  for (int i = 0; i <= strlen(expr); i++) {
    if (isdigit(expr[i])) {
      token[idx++] = expr[i];
    } else if (isalpha(expr[i])) {
      token[idx++] = expr[i];
    } else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' ||
               expr[i] == '/' || expr[i] == '\0') {
      token[idx] = '\0';
      if (idx > 0) {
        int operand;
        if (isdigit(token[0])) {
          operand = atoi(token);
        } else {
          int varIndex = findVariable(token);
          if (varIndex == -1) {
            printf("Error: Undefined variable '%s'\n", token);
            exit(1);
          }
          if (symbolTable[varIndex].isString ||
              !symbolTable[varIndex].isInitialized) {
            printf("Error: Cannot perform arithmetic on non-integer or "
                   "uninitialized variable '%s'\n",
                   token);
            exit(1);
          }
          operand = symbolTable[varIndex].intValue;
        }

        switch (operator) {
        case '+':
          value += operand;
          break;
        case '-':
          value -= operand;
          break;
        case '*':
          value *= operand;
          break;
        case '/':
          if (operand == 0) {
            printf("Error: Division by zero\n");
            exit(1);
          }
          value /= operand;
          break;
        }
      }
      operator= expr[i];
      idx = 0;
    }
  }
  return value;
}

// Function to evaluate a string concatenation expression
void evaluateStringConcatenation(char *result, char *expr) {
  char token[100];
  int idx = 0;
  result[0] = '\0'; // Initialize result

  for (int i = 0; i <= strlen(expr); i++) {
    if (isalnum(expr[i]) || expr[i] == '_') {
      token[idx++] = expr[i];
    } else if (expr[i] == '+' || expr[i] == '\0') {
      token[idx] = '\0';
      if (idx > 0) {
        int varIndex = findVariable(token);
        if (varIndex == -1) {
          printf("Error: Undefined variable '%s'\n", token);
          exit(1);
        }
        if (!symbolTable[varIndex].isString ||
            !symbolTable[varIndex].isInitialized) {
          printf("Error: Cannot concatenate non-string or uninitialized "
                 "variable '%s'\n",
                 token);
          exit(1);
        }
        strcat(result, symbolTable[varIndex].strValue);
      }
      idx = 0;
    }
  }
}

// Function to process a single line of code
void processLine(char *line) {
  char id[100], expr[100];

  // Handle integer declaration and assignment: int a = 4 or int a;
  if (sscanf(line, "int %s = %[^\n]", id, expr) == 2) {
    if (findVariable(id) == -1) {
      strcpy(symbolTable[symbolCount].name, id);
      symbolTable[symbolCount].intValue = evaluateExpression(expr);
      symbolTable[symbolCount].isString = 0;      // Integer variable
      symbolTable[symbolCount].isInitialized = 1; // Initialized
      symbolCount++;
    } else {
      printf("Error: Variable '%s' already declared\n", id);
      exit(1);
    }
  } else if (sscanf(line, "int %s", id) == 1) {
    if (findVariable(id) == -1) {
      strcpy(symbolTable[symbolCount].name, id);
      symbolTable[symbolCount].isString = 0;      // Integer variable
      symbolTable[symbolCount].isInitialized = 0; // Not initialized
      symbolCount++;
    } else {
      printf("Error: Variable '%s' already declared\n", id);
      exit(1);
    }
  }
  // Handle string declaration and assignment: string s = "hello"
  else if (sscanf(line, "string %s = \"%[^\"]\"", id, expr) == 2) {
    if (findVariable(id) == -1) {
      strcpy(symbolTable[symbolCount].name, id);
      strcpy(symbolTable[symbolCount].strValue, expr);
      symbolTable[symbolCount].isString = 1;      // String variable
      symbolTable[symbolCount].isInitialized = 1; // Initialized
      symbolCount++;
    } else {
      printf("Error: Variable '%s' already declared\n", id);
      exit(1);
    }
  } else if (sscanf(line, "string %s", id) == 1) {
    if (findVariable(id) == -1) {
      strcpy(symbolTable[symbolCount].name, id);
      symbolTable[symbolCount].isString = 1;      // String variable
      symbolTable[symbolCount].isInitialized = 0; // Not initialized
      symbolCount++;
    } else {
      printf("Error: Variable '%s' already declared\n", id);
      exit(1);
    }
  }
  // Handle string concatenation: string s = s1 + s2 or s = s1 + s2
  else if (sscanf(line, "string %s = %[^\n]", id, expr) == 2) {
    if (findVariable(id) == -1) {
      strcpy(symbolTable[symbolCount].name, id);
      symbolTable[symbolCount].isString = 1;      // String variable
      symbolTable[symbolCount].isInitialized = 1; // Initialized
      evaluateStringConcatenation(symbolTable[symbolCount].strValue, expr);
      symbolCount++;
    } else {
      printf("Error: Variable '%s' already declared\n", id);
      exit(1);
    }
  } else if (sscanf(line, "%s = %[^\n]", id, expr) == 2) {
    int varIndex = findVariable(id);
    if (varIndex != -1) {
      if (symbolTable[varIndex].isString) {
        evaluateStringConcatenation(symbolTable[varIndex].strValue, expr);
        symbolTable[varIndex].isInitialized = 1;
      } else {
        symbolTable[varIndex].intValue = evaluateExpression(expr);
        symbolTable[varIndex].isInitialized = 1;
      }
    } else {
      printf("Error: Undefined variable '%s'\n", id);
      exit(1);
    }
  }
  // Handle print statement: print a or print s
  else if (sscanf(line, "print %[^\n]", expr) == 1) {
    int varIndex = findVariable(expr);
    if (varIndex != -1) {
      if (!symbolTable[varIndex].isInitialized) {
        printf("Error: Uninitialized variable '%s'\n", expr);
        exit(1);
      }
      if (symbolTable[varIndex].isString) {
        printf("%s\n", symbolTable[varIndex].strValue);
      } else {
        printf("%d\n", symbolTable[varIndex].intValue);
      }
    } else {
      printf("Error: Undefined variable '%s'\n", expr);
      exit(1);
    }
  }
  // Handle scanf statement: scanf a
  else if (sscanf(line, "scanf %s", id) == 1) {
    int varIndex = findVariable(id);
    if (varIndex != -1) {
      if (symbolTable[varIndex].isString) {
        printf("Error: Scanf does not support strings for '%s'\n", id);
        exit(1);
      }
      printf("Enter value for %s: ", id);
      scanf("%d", &symbolTable[varIndex].intValue);
      symbolTable[varIndex].isInitialized = 1;
      getchar(); // To consume the newline character left after scanf
    } else {
      printf("Error: Undefined variable '%s'\n", id);
      exit(1);
    }
  } else {
    printf("Error: Invalid syntax\n");
    exit(1);
  }
}

int main() {
  char line[100];

  // Keep taking input until user types "exit"
  while (1) {
    printf(">>> ");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = 0; // Remove newline character at the end

    if (strcmp(line, "exit") == 0) {
      break;
    }

    processLine(line);
  }

  return 0;
}