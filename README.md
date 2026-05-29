# 🎮 Brickout Game (C++ Console Edition)

A classic **Brick Breaker / Breakout-style game** implemented in **C++ for the Windows console**. The game features real-time movement, collision physics, paddle control, scoring system, and ASCII-based rendering.

---

## 🚀 Features

- 🎯 Real-time ball physics (velocity-based movement)
- 🧱 Breakable brick system with multi-row layout
- 🏓 Interactive paddle control
- ❤️ Lives / HP system
- 🏆 Score and High Score tracking
- ⏸ Pause & resume system
- 🔊 Optional sound effects (Beep)
- 🧠 Angle-based paddle reflection system
- 🖥 ASCII/Unicode graphical rendering in terminal

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| ⬅ Arrow Left | Move paddle left |
| ➡ Arrow Right | Move paddle right |
| Space | Launch ball |
| ESC / P | Pause / Resume game |
| Enter (while paused) | Exit game |

---

## 🧠 How the Game Works

### 🟢 Ball Movement
The ball moves using velocity components:
```cpp
ballX += speedX;
ballY += speedY;
```

- `speedX` controls horizontal movement
- `speedY` controls vertical movement

Increasing `speed` makes the ball move faster because it increases displacement per frame.

---

### 🧱 Brick System
- Bricks are stored in a 2D array:
```cpp
bricks[row][column]
```

- Each brick is 2 rows tall for better visual structure
- When hit, a brick is removed and score increases

---

### 🏓 Paddle Physics
The ball bounce depends on where it hits the paddle:

- Center → straight bounce
- Edges → angled bounce

This is done by calculating distance from paddle center.

---

### 💥 Collision System
The game checks collisions with:

- Walls (left, right, top)
- Bricks
- Paddle

When collision happens, direction is reversed:
```cpp
speedX = -speedX;
speedY = -speedY;
```

---

### ❤️ Lives System
If the ball misses the paddle:
- Player loses 1 life
- Ball resets to paddle position
- Game pauses until player presses Space

---

## 📁 Project Structure

```
main.cpp
├── get_char_at()   → Rendering logic
├── drawAll()       → Screen drawing
├── cursor()        → Input handling
├── updateBall()    → Physics engine
├── initialBricks() → Reset bricks
├── endOfGame()     → Win condition
```

---

## ⚙️ Requirements

- Windows OS (uses `_getch()` and `Beep()`)
- C++ compiler (MinGW / MSVC)
- Console/Terminal support for Unicode characters

---

## 🎯 Game Objective

Break all bricks using the bouncing ball while keeping it from falling using the paddle.

---

## 🧩 Future Improvements (Ideas)

- Add multiple levels
- Improve physics (real collision vectors)
- Add power-ups (multi-ball, bigger paddle)
- Cross-platform support
- Menu system

---

## 🧑‍💻 Author

Created as a learning project for:
- C++ programming
- Game physics basics
- Console rendering techniques

---

🔥 Enjoy the game and keep breaking bricks!

