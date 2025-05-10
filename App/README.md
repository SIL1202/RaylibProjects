# Minimal To-Do List App (Raylib)

This is a simple interactive to-do list built using Raylib and C++. It is designed as a lightweight demo to showcase UI logic, mouse interaction, and custom drawing in a minimal graphics context.

## Features

* Click add button or press tab to add a new task
* Click on a task to remove it
* Dynamically renders tasks in blocks
* Animated pressable buttons with icon feedback
* Clean layout management using custom UILayout

## Built With

* C++17
* Raylib (via Homebrew installation)
* Custom UI components: `Button`, `UILayout`, `TextInput`

## How to Use

### Build

Make sure you have Raylib installed globally (e.g., via Homebrew on macOS):

```bash
brew install raylib
make
```

### Run

```bash
./app
```

## Demo

## File Structure

```
App/
├── app.cpp            # main logic
├── Botton.cpp/hpp     # pressable button with texture and hover detection
├── TextInput.cpp/hpp  # text input handling (optional usage)
├── Global.cpp/hpp     # global styles, color, and layout constants
├── Makefile
```

## Future Plans

*

---

> This project is part of a broader Raylib playground. Feel free to use or extend it!
