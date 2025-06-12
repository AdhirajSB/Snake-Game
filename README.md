# Snake Game (C++ & raylib)

A very basic implementation of the classic Snake game built using C++ and the [raylib](https://www.raylib.com/) graphics library.

## Clone the Repository

```bash
git clone https://github.com/AdhirajSB/Snake-Game.git
cd Snake-Game
```

## How to Build and Run

### macOS

```bash
clang++ -std=c++17 \
  src/main.cpp \
  -o app \
  -Idependencies/include \
  -Ldependencies/lib -lraylib \
  -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL -framework AudioToolbox

./app
```

---

### Linux

Make sure you have `raylib` installed via your package manager or use the static build in `dependencies`.

```bash
g++ -std=c++17 \
  src/main.cpp \
  -o app \
  -Idependencies/include \
  -Ldependencies/lib -lraylib \
  -lGL -lm -lpthread -ldl -lrt -lX11

./app
```

---

### Windows (MinGW / MSYS2)

Ensure raylib is available and linked properly (via `.lib`, `.dll`, or `.a`).

```bash
g++ -std=c++17 \
  src/main.cpp \
  -o app.exe \
  -Idependencies/include \
  -Ldependencies/lib -lraylib \
  -lopengl32 -lgdi32 -lwinmm

./app.exe
```

---
