# Computer Graphics Algorithms

[![Language](https://img.shields.io/badge/language-C%2B%2B-00599C?style=flat&logo=c%2B%2B)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=flat&logo=windows)](https://www.microsoft.com/en-us/windows)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A comprehensive collection of **fundamental computer graphics algorithms** implemented from scratch in C++ using only Windows GDI. Each program demonstrates classic rasterization techniques with interactive click-based interfaces.

---

## 📁 Repository Structure
computer-graphics-algorithms/

├── BresLine.cpp # Bresenham Line Algorithm

├── circle.cpp # Bresenham Circle & Concentric Ring Fill

├── curve.cpp # Hermite Cubic Spline Curve

├── floodPolygon.cpp # Polygon Flood Fill (Boundary Fill)

└── README.md


---

## Demos Overview

### 1. Bresenham Line Algorithm (`task1Gr.cpp`)

**Description:**  
Classic midpoint line drawing algorithm optimized for integer arithmetic. Demonstrates case with slope between 0 and 1.

**Features:**
- Integer-only calculations (no floating point)
- Decision parameter tracking
- Console output showing pixel coordinates and decision values

**Key Formula:**
d = Δx - 2Δy (initial decision parameter)

dd1 = 2Δx - 2Δy (diagonal step increment)

dd2 = -2Δy (horizontal step increment)


**Usage:**  
Runs automatically on window creation, drawing a line from (200,300) to (450,150).

---

### 2. Circle Drawing & Concentric Ring Fill (`circle.cpp`)

**Description:**  
Bresenham circle algorithm using 8-way symmetry. Fills a circular ring between two user-selected radii.

**Interactive Workflow:**
1. **First click** → Center of circles
2. **Second click** → Point on inner circle (defines R₁)
3. **Third click** → Point on outer circle (defines R₂)

**Features:**
- 8-octant symmetry with color-coded pixels
- Concentric circle fill (draws every integer radius between R₁ and R₂)
- Lines from center to both boundary points
- Automatic radius sorting (ensures R₁ < R₂)

**Color-Coded Octants:**
| Octant | Color |
|--------|-------|
| (xc+x, yc+y) | Red |
| (xc-x, yc+y) | Green |
| (xc-x, yc-y) | Blue |
| (xc+x, yc-y) | Yellow |
| (xc+y, yc+x) | Magenta |
| (xc-y, yc+x) | Cyan |
| (xc-y, yc-x) | Orange |
| (xc+y, yc-x) | Purple |

---

### 3. Hermite Cubic Spline (`curve.cpp`)

**Description:**  
Interactive Hermite curve drawing using basis matrix formulation. Users define curve shape by clicking four control points.

**Interactive Workflow:**
1. **Click 1** → P₀ (start point, curve passes through)
2. **Click 2** → P₁ (handle controlling start tangent)
3. **Click 3** → P₂ (handle controlling end tangent)
4. **Click 4** → P₃ (end point, curve passes through)

**Mathematical Foundation:**

The Hermite basis matrix:
[α₀] [1 0 0 0] [x₁]
[α₁] = [0 1 0 0] [u₁]
[α₂] [-3 -2 3 -1] [x₂]
[α₃] [ 2 1 -2 1] [u₂]

Where:
- `u₁ = 3(P₁ - P₀)` — tangent at t=0
- `u₂ = 3(P₃ - P₂)` — tangent at t=1

Curve evaluation:
x(t) = α₀ + α₁·t + α₂·t² + α₃·t³

y(t) = β₀ + β₁·t + β₂·t² + β₃·t³ (0 ≤ t ≤ 1)


**Visual Elements:**
- **Black curve** — The Hermite spline (500 segments)
- **Red lines** — Control handles (P₀-P₁ and P₃-P₂)
- **Blue dots** — Endpoints (P₀, P₃)
- **Green dots** — Control handles (P₁, P₂)

---

### 4. Polygon Flood Fill (`floodPolygon.cpp`)

**Description:**  
Stack-based boundary fill algorithm that floods enclosed regions with a specified fill color.

**Interactive Workflow:**
1. **Click 1-5** → Define 5 vertices of a pentagon (points marked in red)
2. **Click anywhere inside polygon** → Flood fill with cyan color

**Algorithm (Stack-based):**
FloodFill(x, y, boundaryColor, fillColor):
push (x, y) onto stack
while stack not empty:
pop pixel
if pixel color != boundaryColor and pixel color != fillColor:
set pixel to fillColor
push (x, y+1), (x, y-1), (x+1, y), (x-1, y)


**Features:**
- 4-connected flood fill
- Recursive-equivalent using explicit stack
- Prevents stack overflow issues
- Visual feedback for vertex placement

---

## 🔧 Compilation & Execution

### Requirements
- Windows OS (GDI required for graphics)
- C++ compiler with Windows SDK
- **Recommended:** Visual Studio, MinGW-w64, or Clang

### Compilation Commands

#### Using MSVC (Visual Studio Developer Command Prompt):
```bash
# Bresenham Line
cl task1Gr.cpp user32.lib gdi32.lib

# Circle & Ring Fill
cl circle.cpp user32.lib gdi32.lib

# Hermite Curve
cl curve.cpp user32.lib gdi32.lib

# Polygon Flood Fill
cl floodPolygon.cpp user32.lib gdi32.lib
```

#### Using MinGW-w64 (g++):
```bash
# Bresenham Line
g++ task1Gr.cpp -o task1Gr.exe -lgdi32 -luser32

# Circle & Ring Fill
g++ circle.cpp -o circle.exe -lgdi32 -luser32

# Hermite Curve
g++ curve.cpp -o curve.exe -lgdi32 -luser32

# Polygon Flood Fill
g++ floodPolygon.cpp -o floodPolygon.exe -lgdi32 -luser32
```

#### Running
Simply double-click the compiled .exe file or run from command prompt.







