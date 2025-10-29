# Hash Error Handling Guide

## Improved Error Messages

Hash now provides **context-aware error messages** for all built-in math functions with specific suggestions for fixes.

---

## Math Function Errors

### Type Mismatch Errors

####  Wrong Type for Arguments

```hash
let x: f64 = pow(2, 8)  # Error
```

**Error Message:**
```
error: Argument 1 type mismatch in call to 'pow': expected f64, got i32
  help: Use f64 literal (e.g., 2.0 instead of 2) or convert with float().
```

**Fix:**
```hash
let x: f64 = pow(2.0, 8.0)           # Use float literals
let y: f64 = pow(float(2), float(8)) # Or convert explicitly
```

---

####  Wrong Return Type Assignment

```hash
let x: i32 = sqrt(16.0)  # Error
```

**Error Message:**
```
error: Type mismatch in variable initialization: expected i32, got f64
  help: Change the variable type to 'f64' or provide a value of type 'i32'
```

**Fix:**
```hash
let x: f64 = sqrt(16.0)       # Correct type
let y: i32 = int(sqrt(16.0))  # Convert to int
```

---

### Argument Count Errors

####  Wrong Number of Arguments

```hash
let x: f64 = pow(2.0)  # Error
```

**Error Message:**
```
error: Function 'pow' expects 2 arguments, got 1
  help: Provide exactly 2 arguments when calling 'pow'.
```

**Fix:**
```hash
let x: f64 = pow(2.0, 8.0)  # Provide both base and exponent
```

---

## Type Conversion Function Errors

### int() and float() Errors

####  Wrong Type for int()

```hash
let x: i32 = int(42)  # Error
```

**Error Message:**
```
error: Argument 1 type mismatch in call to 'int': expected f64, got i32
  help: Type conversion function 'int()' expects f64 but got i32.
```

**Fix:**
```hash
let x: i32 = int(42.0)  # int() converts f64 → i32
let y: f64 = float(42)  # float() converts i32 → f64
```

---

## All Math Functions - Quick Reference

### Functions Requiring f64 Arguments

These functions **only accept f64** (floating point) arguments:

- `pow(f64, f64) -> f64`
- `sqrt(f64) -> f64`
- `floor(f64) -> f64`
- `ceil(f64) -> f64`
- `round(f64) -> f64`

**Common mistake:** Using integer literals
```hash
pow(2, 8)      #  Error
pow(2.0, 8.0)  #  Correct
```

### Functions Accepting i32 Arguments

These functions work with integers:

- `abs(i32) -> i32`
- `min(i32, i32) -> i32`
- `max(i32, i32) -> i32`

```hash
let x: i32 = abs(-5)      #  Correct
let y: i32 = min(10, 20)  #  Correct
```

---

## Edge Cases Handled Gracefully

Hash handles mathematical edge cases safely:

### Special Float Values

```hash
# Negative square root → NaN
let nan: f64 = sqrt(-1.0)
print_f64(nan)  # Outputs: -nan(ind)

# Overflow → Infinity
let inf: f64 = pow(10.0, 1000.0)
print_f64(inf)  # Outputs: inf

# 0^0 → 1 (by convention)
let one: f64 = pow(0.0, 0.0)
print_f64(one)  # Outputs: 1.000000
```

### Rounding Edge Cases

```hash
# Works correctly with zeros
floor(0.0)   # → 0.0
ceil(-0.0)   # → -0.0

# Standard rounding (round-to-even)
round(0.5)   # → 1.0
round(1.5)   # → 2.0
round(-0.5)  # → -0.0
```

---

## Common Patterns and Solutions

### Pattern 1: Converting Integer to Use in Math Function

```hash
let radius: i32 = 5

#  Wrong:
let area: f64 = 3.14159 * pow(radius, 2.0)

#  Correct:
let area: f64 = 3.14159 * pow(float(radius), 2.0)
```

### Pattern 2: Getting Integer Result from Math Function

```hash
#  Wrong:
let rounded: i32 = round(3.7)

#  Correct:
let rounded: i32 = int(round(3.7))
```

### Pattern 3: Chaining Conversions

```hash
let x: i32 = 10
let y: i32 = 3

# Calculate x/y rounded
let ratio: f64 = float(x) / float(y)  # 3.333...
let rounded: i32 = int(round(ratio))  # 3
```

---

## Error Prevention Checklist

When using math functions, remember:

-  Use `.0` for float literals: `2.0` not `2`
-  Match variable type to function return type
-  Provide correct number of arguments
-  Use `float()` to convert int → float
-  Use `int()` to convert float → int (truncates)
-  Use `round()`, `floor()`, or `ceil()` before `int()` for better rounding

---

## Complete Working Example

```hash
fn main() -> i32:
    # Read user input
    print_str("Enter radius: ")
    let radius: f64 = read_f64()
    
    # Calculate area (π * r²)
    let pi: f64 = 3.14159
    let area: f64 = pi * pow(radius, 2.0)
    
    # Round and display
    print_str("Area: ")
    print_f64(round(area))
    println()
    
    # Convert to integer square units
    let area_int: i32 = int(round(area))
    print_str("Approximate: ")
    print_i32(area_int)
    print_str(" square units")
    println()
    
    return 0
```

---

## See Also

- [BUILTINS_v2.md](BUILTINS_v2.md) - Complete built-in functions reference
- [examples/14_error_guide.hash](../examples/14_error_guide.hash) - Common mistakes and fixes
- [examples/13_edge_cases.hash](../examples/13_edge_cases.hash) - Edge case behaviors
- [examples/12_advanced_math.hash](../examples/12_advanced_math.hash) - Math function examples
