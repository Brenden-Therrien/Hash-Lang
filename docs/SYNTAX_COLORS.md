# Hash Language - Syntax Highlighting Color Reference

##  Vibrant Color Scheme

###  SPECIAL KEYWORDS (Hash's Unique Features)

| Keyword | Color | Hex | Style | Purpose |
|---------|-------|-----|-------|---------|
| `pure` | bright pink  | `#ff6b9d` | **Bold** | Marks pure functions (no side effects) |
| `pure_local` | vibrant orange | `#ff9d00` | **Bold** | Behavior-aware access control |

**These colors stand out.** They're intentionally the brightest colors in the scheme to draw immediate attention to Hash's unique functional programming features.

---

### General Keywords

| Element | Color | Hex | Example |
|---------|-------|-----|---------|
| Control Flow | Bright Red | `#ff7b72` | `if`, `else`, `while`, `return` |
| Keywords | Purple | `#d2a8ff` | `fn`, `let`, `mut` |

---

### Types

| Element | Color | Hex | Examples |
|---------|-------|-----|----------|
| All Types | Cyan Blue | `#79c0ff` | `i32`, `f64`, `bool`, `str`, `void` |

---

###  Functions

| Element | Color | Hex | Description |
|---------|-------|-----|-------------|
| Built-in Functions | Warm Orange | `#ffa657` | All 50 Hash built-ins |
| User Functions | Purple | `#d2a8ff` | Your function names |

---

###  Data

| Element | Color | Hex | Examples |
|---------|-------|-----|----------|
| Strings | Light Blue | `#a5d6ff` | `"Hello World"` |
| String Escapes | Cyan | `#79c0ff` | `\n`, `\t`, `\\` |
| Numbers | Cyan Blue | `#79c0ff` | `42`, `3.14`, `1.5e-10` |
| Booleans | Cyan Blue | `#79c0ff` | `true`, `false` |

---

### Operators

| Element | Color | Hex | Examples |
|---------|-------|-----|----------|
| All Operators | Bright Red | `#ff7b72` | `+`, `-`, `*`, `/`, `==`, `&&`, `\|`, `<<` |

---

### Comments

| Element | Color | Hex | Style |
|---------|-------|-----|-------|
| Comments | Gray | `#7d8590` | *Italic* |

---

##  Why These Colors?

### PINK for `pure`
- **Brightest** color in the scheme
- **Bold** font weight
- distinctively highlighted in code
- Represents Hash's **functional purity**
- Think: "Pure = Pink = Perfect!"

### ORANGE for `pure_local`
- **Second brightest** color
- **Bold** font weight
- Distinct from `pure` but equally important
- Represents **exclusive access control**
- Think: "Orange = Only pure functions!"

### Why So Bright?
These are Hash's **unique innovations**:
- No other language has this exact combination
- They deserve to stand out visually
- Helps you quickly identify pure code
- Makes code reviews easier
- Educational for learning Hash

---

## Visual Examples

### Pure Function (PINK)
```hash
pure fn calculate_total(a: i32, b: i32) -> i32:  # 'pure' in bright pink
    return a + b
```

### Pure Local Variable (ORANGE)
```hash
let pure_local secret: i32 = 42  # 'pure_local' in vibrant orange
```

### Built-in Functions (Warm Orange)
```hash
print_str("Hello!")  # Built-ins in warm orange
let length: i32 = len("test")
file_write("data.txt", "content")
```

### Types (Cyan)
```hash
fn demo(x: i32, y: f64, s: str) -> bool:  # Types in cyan
    return true
```

### Strings (Light Blue)
```hash
let message: str = "Hello\nWorld"  # String in light blue, \n in cyan
```

---

##  Color Hierarchy

**Brightest (Most Important):**
1. `pure` - PINK (#ff6b9d)
2. `pure_local` - ORANGE (#ff9d00)

**Bright (Keywords & Operators):**
3. Control Flow & Operators - Red (#ff7b72)
4. Keywords & Functions - Purple (#d2a8ff)
5. Built-in Functions - Orange (#ffa657)

**Medium (Data):**
6. Types & Numbers - Cyan (#79c0ff)
7. Strings - Light Blue (#a5d6ff)

**Dim:**
8. Comments - Gray (#7d8590)

---

## Theme Installation

```powershell
.\install_syntax.ps1
```

Then in VS Code:
1. Reload window (Ctrl+R)
2. Open any `.hash` file
3. (Optional) Switch theme: `Ctrl+K Ctrl+T` → "Hash Vibrant"

---

##  Pro Tips

- **Dark background recommended** - Colors pop best on dark themes
- **Look for pink and orange** - They indicate Hash's unique features
- **All 50 built-ins highlighted** - Easy to spot standard library usage
- **Consistent across all .hash files** - No configuration needed

---
