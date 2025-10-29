# Hash Language - Built-in Functions Reference

## Philosophy

Hash provides **two styles** of built-in functions:

1. **Python-style** (recommended): Clean, simple names like `int()`, `float()`, `abs()`, `sqrt()`
2. **Explicit** (verbose): Type-specific names like `i32_to_f64()`, `abs_i32()`, `sqrt_f64()`

**We recommend using the Python-style functions for cleaner, more readable code!**

## Console Output Functions

Hash provides several built-in functions for printing values to the console.

### Available Functions

#### `print_i32(value: i32) -> void`
Prints a 32-bit signed integer followed by a newline.

```hash
print_i32(42)        # Output: 42
print_i32(-17)       # Output: -17
```

#### `print_i64(value: i64) -> void`
Prints a 64-bit signed integer followed by a newline.

```hash
let big: i64 = 9223372036854775807
print_i64(big)
```

Note: Integer literals default to `i32`, so you'll need explicit type conversion (coming soon) to use with `print_i64`.

#### `print_f64(value: f64) -> void`
Prints a 64-bit floating point number followed by a newline.

```hash
print_f64(3.14159)   # Output: 3.141590
print_f64(-2.71828)  # Output: -2.718280
```

#### `print_bool(value: bool) -> void`
Prints a boolean value as "true" or "false" followed by a newline.

```hash
print_bool(true)     # Output: true
print_bool(false)    # Output: false
```

#### `println() -> void`
Prints a blank line (newline character).

```hash
print_i32(42)
println()            # Adds spacing
print_i32(100)
```

## Example Usage

```hash
fn main() -> i32:
    # Print calculation results
    let x: i32 = 10
    let y: i32 = 5

    let sum: i32 = x + y
    print_i32(sum)      # Output: 15

    # Print comparison results
    let is_greater: bool = x > y
    print_bool(is_greater)  # Output: true

    # Print floating point
    let pi: f64 = 3.14159
    print_f64(pi)       # Output: 3.141590

    # Add spacing
    println()

    return 0
```

## Coming Soon

- `print_str(str)` - Print strings (requires string literal support)
- Input functions (`read_line`, `read_i32`, etc.)

## Type Conversion Functions

Hash provides built-in functions to convert between numeric types.

### Python-Style Conversions (Recommended)

#### `int(value: f64) -> i32`
Converts a float to an integer (truncates decimal part).

```hash
let pi: f64 = 3.14159
let whole: i32 = int(pi)
print_i32(whole)  # Output: 3
```

#### `float(value: i32) -> f64`
Converts an integer to a floating point number.

```hash
let x: i32 = 100
let y: f64 = float(x)
print_f64(y)  # Output: 100.000000
```

### Explicit Type Conversions (Verbose but Clear)

These functions provide explicit type information in their names. Use them when you need to be very specific about types:

#### `i32_to_i64(value: i32) -> i64`
Converts a 32-bit integer to a 64-bit integer (sign extension).

```hash
let small: i32 = 42
let big: i64 = i32_to_i64(small)
print_i64(big)  # Output: 42
```

#### `i64_to_i32(value: i64) -> i32`
Converts a 64-bit integer to a 32-bit integer (truncation, may lose data).

```hash
let big: i64 = i32_to_i64(1000)
let small: i32 = i64_to_i32(big)
print_i32(small)  # Output: 1000
```

#### `i32_to_f64(value: i32) -> f64`
Converts a 32-bit integer to a 64-bit floating point number.

```hash
let int_val: i32 = 100
let float_val: f64 = i32_to_f64(int_val)
print_f64(float_val)  # Output: 100.000000
```

#### `f64_to_i32(value: f64) -> i32`
Converts a 64-bit floating point number to a 32-bit integer (truncates decimal part).

```hash
let pi: f64 = 3.14159
let truncated: i32 = f64_to_i32(pi)
print_i32(truncated)  # Output: 3
```

#### `i64_to_f64(value: i64) -> f64`
Converts a 64-bit integer to a 64-bit floating point number.

#### `f64_to_i64(value: f64) -> i64`
Converts a 64-bit floating point number to a 64-bit integer (truncates decimal part).

## Math Functions

Hash provides essential mathematical functions for common operations.

### Python-Style Math (Recommended)

#### `abs(value: i32) -> i32`
Returns the absolute value.

```hash
print_i32(abs(-42))   # Output: 42
print_i32(abs(42))    # Output: 42
```

#### `min(a: i32, b: i32) -> i32`
Returns the smaller of two values.

```hash
print_i32(min(10, 20))   # Output: 10
print_i32(min(100, 5))   # Output: 5
```

#### `max(a: i32, b: i32) -> i32`
Returns the larger of two values.

```hash
print_i32(max(10, 20))   # Output: 20
print_i32(max(100, 5))   # Output: 100
```

#### `sqrt(value: f64) -> f64`
Returns the square root.

```hash
print_f64(sqrt(4.0))    # Output: 2.000000
print_f64(sqrt(16.0))   # Output: 4.000000
print_f64(sqrt(2.0))    # Output: 1.414214
```

### Explicit Math Functions (Verbose)

These are the type-specific versions if you need them:

#### `abs_i32(value: i32) -> i32`
Returns the absolute value of a 32-bit integer.

```hash
print_i32(abs_i32(-42))   # Output: 42
print_i32(abs_i32(42))    # Output: 42
```

#### `min_i32(a: i32, b: i32) -> i32`
Returns the smaller of two 32-bit integers.

```hash
print_i32(min_i32(10, 20))   # Output: 10
print_i32(min_i32(100, 5))   # Output: 5
```

#### `max_i32(a: i32, b: i32) -> i32`
Returns the larger of two 32-bit integers.

```hash
print_i32(max_i32(10, 20))   # Output: 20
print_i32(max_i32(100, 5))   # Output: 100
```

#### `sqrt_f64(value: f64) -> f64`
Returns the square root of a 64-bit floating point number.

```hash
print_f64(sqrt_f64(4.0))    # Output: 2.000000
print_f64(sqrt_f64(16.0))   # Output: 4.000000
print_f64(sqrt_f64(2.0))    # Output: 1.414214
```

### Practical Example: Distance Calculation (Clean Syntax)

```hash
fn main() -> i32:
    # Calculate distance using Pythagorean theorem
    # With clean Python-like functions
    let dx: i32 = 3
    let dy: i32 = 4

    let dx_sq: i32 = dx * dx
    let dy_sq: i32 = dy * dy
    let sum_sq: i32 = dx_sq + dy_sq

    let distance: f64 = sqrt(float(sum_sq))

    print_f64(distance)  # Output: 5.000000

    return 0
```

**Compare to verbose version:**
```hash
# Old way (still works, but verbose):
let distance: f64 = sqrt_f64(i32_to_f64(sum_sq))

# New way (recommended):
let distance: f64 = sqrt(float(sum_sq))  # Much better
```

## Advanced Math Functions

Hash provides comprehensive mathematical capabilities including trigonometry, logarithms, and power functions.

### Power and Rounding

#### `pow(base: f64, exponent: f64) -> f64`
Raises base to the power of exponent (base^exponent).

```hash
print_f64(pow(2.0, 8.0))     # Output: 256.000000
print_f64(pow(10.0, 3.0))    # Output: 1000.000000
print_f64(pow(2.0, 0.5))     # Output: 1.414214 (square root of 2)
```

#### `floor(value: f64) -> f64`
Rounds down to the nearest integer.

```hash
print_f64(floor(3.7))   # Output: 3.000000
print_f64(floor(-2.3))  # Output: -3.000000
```

#### `ceil(value: f64) -> f64`
Rounds up to the nearest integer.

```hash
print_f64(ceil(3.2))    # Output: 4.000000
print_f64(ceil(-2.7))   # Output: -2.000000
```

#### `round(value: f64) -> f64`
Rounds to the nearest integer.

```hash
print_f64(round(3.5))   # Output: 4.000000
print_f64(round(3.4))   # Output: 3.000000
```

### Trigonometric Functions

#### `sin(angle: f64) -> f64`
Returns the sine of angle (in radians).

```hash
let pi: f64 = 3.141592653589793
print_f64(sin(0.0))        # Output: 0.000000
print_f64(sin(pi / 2.0))   # Output: 1.000000
```

#### `cos(angle: f64) -> f64`
Returns the cosine of angle (in radians).

```hash
let pi: f64 = 3.141592653589793
print_f64(cos(0.0))        # Output: 1.000000
print_f64(cos(pi))         # Output: -1.000000
```

#### `tan(angle: f64) -> f64`
Returns the tangent of angle (in radians).

```hash
let pi: f64 = 3.141592653589793
print_f64(tan(0.0))        # Output: 0.000000
print_f64(tan(pi / 4.0))   # Output: 1.000000 (45 degrees)
```

#### `asin(value: f64) -> f64`
Returns the arc sine (inverse sine) in radians. Input must be in range [-1, 1].

```hash
print_f64(asin(0.0))   # Output: 0.000000
print_f64(asin(1.0))   # Output: 1.570796 (π/2)
```

#### `acos(value: f64) -> f64`
Returns the arc cosine (inverse cosine) in radians. Input must be in range [-1, 1].

```hash
print_f64(acos(1.0))   # Output: 0.000000
print_f64(acos(0.0))   # Output: 1.570796 (π/2)
```

#### `atan(value: f64) -> f64`
Returns the arc tangent (inverse tangent) in radians.

```hash
print_f64(atan(0.0))   # Output: 0.000000
print_f64(atan(1.0))   # Output: 0.785398 (π/4)
```

### Logarithmic & Exponential Functions

#### `exp(value: f64) -> f64`
Returns e^value (Euler's number raised to the power of value).

```hash
print_f64(exp(0.0))    # Output: 1.000000
print_f64(exp(1.0))    # Output: 2.718282 (e)
print_f64(exp(2.0))    # Output: 7.389056
```

#### `log(value: f64) -> f64`
Returns the natural logarithm (ln) of value. Input must be > 0.

```hash
let e: f64 = 2.718281828459045
print_f64(log(e))      # Output: 1.000000
print_f64(log(1.0))    # Output: 0.000000
```

#### `log2(value: f64) -> f64`
Returns the base-2 logarithm of value. Input must be > 0.

```hash
print_f64(log2(8.0))   # Output: 3.000000
print_f64(log2(1024.0)) # Output: 10.000000
```

#### `log10(value: f64) -> f64`
Returns the base-10 logarithm of value. Input must be > 0.

```hash
print_f64(log10(100.0))  # Output: 2.000000
print_f64(log10(1000.0)) # Output: 3.000000
```

## System Functions

Hash provides system functions for program control, timing, and random number generation.

### Program Control

#### `exit(status: i32) -> void`
Exits the program immediately with the given status code.

```hash
fn main() -> i32:
    let value: i32 = read_i32()
    if value < 0:
        print_str("Error: negative value!")
        println()
        exit(1)  # Exit with error code

    print_i32(value)
    return 0
```

### Timing Functions

#### `hash_time() -> i64`
Returns the current Unix timestamp (seconds since January 1, 1970).

```hash
let timestamp: i64 = hash_time()
print_i64(timestamp)  # Output: 1761767960 (current time)
```

#### `hash_clock() -> f64`
Returns the CPU time used by the program in seconds. Useful for benchmarking.

```hash
let start: f64 = hash_clock()
# ... do some work ...
let elapsed: f64 = hash_clock() - start
print_f64(elapsed)  # Output: 0.000123 (time in seconds)
```

### Random Number Generation

#### `random() -> f64`
Returns a random floating-point number between 0.0 (inclusive) and 1.0 (exclusive).

```hash
# Seed first for different results each run
seed_random(i64_to_i32(hash_time()))

let rand1: f64 = random()
print_f64(rand1)  # Output: 0.513352 (example)
```

#### `seed_random(seed: i32) -> void`
Sets the seed for the random number generator. Use the same seed for reproducible results.

```hash
seed_random(12345)  # Fixed seed = same sequence
let r1: f64 = random()
let r2: f64 = random()
```

#### `random_range(min: i32, max: i32) -> i32`
Returns a random integer between min and max (both inclusive).

```hash
seed_random(i64_to_i32(hash_time()))

let dice: i32 = random_range(1, 6)      # Dice roll
let rand_byte: i32 = random_range(0, 255)  # Random byte
let coin: i32 = random_range(0, 1)      # Coin flip
```

## String Manipulation Functions

Hash provides clean, Python-inspired string functions for text processing.

### String Information

#### `len(str) -> i32`
Returns the length of a string.

```hash
let message: str = "Hello"
let length: i32 = len(message)
print_i32(length)  # Output: 5

let greeting: str = "Hello World"
print_i32(len(greeting))  # Output: 11
```

### String Operations

#### `str_concat(str, str) -> str`
Concatenates two strings and returns a new string.

```hash
let first: str = "Hello"
let second: str = "World"
let combined: str = str_concat(first, second)
print_str(combined)  # Output: "HelloWorld"

let space: str = " "
let temp: str = str_concat(first, space)
let message: str = str_concat(temp, second)
print_str(message)  # Output: "Hello World"
```

#### `str_eq(str, str) -> bool`
Compares two strings for equality. Returns true if they match.

```hash
let pass1: str = "secret123"
let pass2: str = "secret123"
let pass3: str = "wrong"

print_bool(str_eq(pass1, pass2))  # Output: true
print_bool(str_eq(pass1, pass3))  # Output: false
```

### Case Conversion

#### `upper(str) -> str`
Converts a string to uppercase.

```hash
let text: str = "hello world"
let loud: str = upper(text)
print_str(loud)  # Output: "HELLO WORLD"

let tag: str = "<div>"
let tag_upper: str = upper(tag)
print_str(tag_upper)  # Output: "<DIV>"
```

#### `lower(str) -> str`
Converts a string to lowercase.

```hash
let text: str = "HELLO WORLD"
let quiet: str = lower(text)
print_str(quiet)  # Output: "hello world"

let cmd: str = "START"
let cmd_lower: str = lower(cmd)
print_str(cmd_lower)  # Output: "start"
```

### Practical Example: Performance Benchmarking

```hash
fn fibonacci(n: i32) -> i32:
    if n <= 1:
        return n
    else:
        return fibonacci(n - 1) + fibonacci(n - 2)

fn main() -> i32:
    let start: f64 = hash_clock()
    let result: i32 = fibonacci(30)
    let elapsed: f64 = hash_clock() - start

    print_str("Result: ")
    print_i32(result)
    println()
    print_str("Time: ")
    print_f64(elapsed)
    print_str(" seconds")
    println()

    return 0
```

### Practical Example: Random Number Game

```hash
fn main() -> i32:
    # Seed with current time
    let timestamp: i64 = hash_time()
    seed_random(i64_to_i32(timestamp))

    # Generate random number 1-100
    let secret: i32 = random_range(1, 100)

    print_str("Guess the number (1-100)!")
    println()

    let guess: i32 = read_i32()

    if guess == secret:
        print_str("Correct!")
    else:
        print_str("Wrong! It was: ")
        print_i32(secret)

    println()
    return 0
```

### Practical Example: Text Processing

```hash
fn main() -> i32:
    # Case-insensitive command parser
    let cmd_input: str = "start"
    let cmd_normalized: str = upper(cmd_input)

    let cmd_start: str = "START"
    let cmd_stop: str = "STOP"

    if str_eq(cmd_normalized, cmd_start):
        print_str("Starting system...")
    else:
        if str_eq(cmd_normalized, cmd_stop):
            print_str("Stopping system...")
        else:
            print_str("Unknown command!")

    println()
    return 0
```

### Practical Example: String Builder

```hash
fn main() -> i32:
    # Build a formatted string
    let prefix: str = "Hello, "
    let name: str = "World"
    let suffix: str = "!"

    let temp: str = str_concat(prefix, name)
    let message: str = str_concat(temp, suffix)

    print_str(message)  # "Hello, World"
    println()

    # Check length
    let msg_len: i32 = len(message)
    print_str("Length: ")
    print_i32(msg_len)  # 13
    println()

    return 0
```

## File I/O Functions

Hash provides essential file operations for reading, writing, and managing files.

### Reading Files

#### `file_read(filename: str) -> str`
Reads the entire contents of a file and returns it as a string. Returns an empty string if the file cannot be read.

```hash
let content: str = file_read("config.txt")
let length: i32 = len(content)

if length == 0:
    print_str("File not found or empty")
else:
    print_str("File contents:")
    println()
    print_str(content)
```

### Writing Files

#### `file_write(filename: str, content: str) -> bool`
Writes a string to a file, creating it if it doesn't exist, or overwriting if it does. Returns `true` on success, `false` on failure.

```hash
let data: str = "Hello, Hash!"
let success: bool = file_write("output.txt", data)

if success:
    print_str("File written successfully")
else:
    print_str("Failed to write file")
```

### File Management

#### `file_exists(filename: str) -> bool`
Checks if a file exists. Returns `true` if the file exists, `false` otherwise.

```hash
if file_exists("data.txt"):
    let content: str = file_read("data.txt")
    print_str(content)
else:
    print_str("File does not exist")
```

#### `file_delete(filename: str) -> bool`
Deletes a file. Returns `true` if the file was successfully deleted, `false` if deletion failed.

```hash
if file_exists("temp.txt"):
    let deleted: bool = file_delete("temp.txt")
    if deleted:
        print_str("Temp file cleaned up")
    else:
        print_str("Failed to delete temp file")
```

### Complete File I/O Example

```hash
fn main() -> i32:
    # Write a config file
    let config: str = str_concat("app_name=HashLang", "\n")
    let config2: str = str_concat(config, "version=0.3.0")

    if file_write("config.txt", config2):
        print_str("Config saved")
        println()

        # Read it back
        let saved: str = file_read("config.txt")
        print_str(saved)
        println()

        # Clean up
        file_delete("config.txt")

    return 0
```

## See Also

- `examples/00_hello.hash` - Basic I/O demonstration
- `examples/09_builtins.hash` - All built-in functions showcase
- `examples/10_pythonic_syntax.hash` - Python-style function examples
- `examples/11_interactive.hash` - User input examples
- `examples/12_advanced_math.hash` - Advanced math functions
- `examples/15_trigonometry.hash` - Trigonometry examples
- `examples/16_logarithms.hash` - Logarithm examples
- `examples/17_random.hash` - Random number generation
- `examples/20_strings.hash` - String manipulation examples
- `examples/21_text_processor.hash` - Advanced text processing
- `examples/22_file_io.hash` - File I/O operations
- `examples/18_timing.hash` - Performance benchmarking
- `examples/19_guessing_game.hash` - Interactive game with random numbers
- `examples/20_strings.hash` - String manipulation examples  NEW
- `examples/21_text_processor.hash` - Text processing patterns  NEW
