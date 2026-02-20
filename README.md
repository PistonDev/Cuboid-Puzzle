# Cuboid Puzzle (Raylib)

A 3D cuboid-rolling puzzle game built using raylib.

The game is based on grid movement and cuboid state rotation.  
You must roll the cuboid across platforms and reach the end tile.

---

## ▶ How to Run

1. Open the project using the `.vcxproj` file in Visual Studio.
2. Build and run `game.cpp`.
3. The game loads levels automatically.

4. Alternative : (after updating) just run .exe file.

### Dependencies
- raylib libraries are inside the **external/** folder
- They are included via `resources.h`
- `raylib.dll` is linked in the project settings
- You do NOT need to install raylib separately

---

## 🧩 Creating Your Own Levels

To create a new level:

1. Create a new text file.
2. Name it using the next number in ascending order  
   Example:
   ```
   0.txt
   1.txt
   2.txt
   → next must be 3.txt
   ```

3. Follow the same format used in existing level files.
4. Grid values:
   - `1` → block exists
   - `0` → empty space
5. Special markers:
   - `s` → starting position
   - `e` → ending position
6. You must also specify the starting cuboid state.
   - `h` -> horizontal state
   - `v` -> vertical state

---

## 💡 Easiest Way to Design Levels

For fastest level creation:

- Ask ChatGPT or another AI model
- Request a level in the same text format
- Copy → paste → save → run the game

---

## 🎯 Objective

Roll the cuboid from start to end.  
Different cuboid orientations change how it occupies grid space.

---

## 🛠 Project Structure

```
Cuboid Puzzle/
│
├── game.cpp
├── headers/
├── data/        → level files
├── external/    → raylib dependencies
├── resources.h
└── .vcxproj
```

---

## 👤 Author

PistoDev  
Game programming learning project
