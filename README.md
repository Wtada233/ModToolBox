# ModToolbox

A cross-platform, terminal-based toolbox for Minecraft Modding.

This tool provides a simple Text-based User Interface (TUI) for managing common tasks in Minecraft mod development, designed to work on both Windows and Unix-like systems.

## Implemented Features

- **Project Management**
  - Open and switch between different Minecraft mod project directories via the "Switch Project" option.
  - All build and configuration actions are performed within the context of the currently opened project.

- **Mod Configuration (fabric.mod.json)**
  - Reads and writes to the `src/main/resources/fabric.mod.json` file.
  - Allows live editing of the Mod Name (`name`), Mod ID (`id`), Version (`version`), Description (`description`), and Authors (`authors`).

- **Build System Integration**
  - A dedicated "Build" menu to interact with the Gradle wrapper.
  - Provides buttons to execute common tasks:
    - `build`: Compiles the mod.
    - `genSources`: Decompiles Minecraft sources (if applicable).
    - `runClient`: Starts the Minecraft client with the mod.
    - `clean`: Cleans the build outputs.
  - Displays the live output of the executed command directly within the UI.

- **Texture Editor**
  - A simple 16x16 pixel art editor for creating or modifying textures.
  - Supports basic drawing with a palette and transparent color.
  - Allows saving the texture as a PNG file.

## How to Build

### Prerequisites
- A C++17 compatible compiler (GCC, Clang, MSVC)
- CMake (version 3.16 or newer)
- Git

### Steps

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Configure the project with CMake
cmake ..

# 3. Compile the project
make

# 4. The executable will be generated in the build directory
# You can run it with:
./modtoolbox
```

## How to Use

1.  Run the executable: `./modtoolbox`.
2.  Use the **Arrow Keys** to navigate menus.
3.  Press **Enter** to select an option.
4.  The first step is to select **"Switch Project"** to open your mod project's root directory.
5.  Once a project is opened, you can navigate to the **"Configuration"**, **"Build"**, or **"Tools"** menus to perform actions.
    *   **Configuration:** Edit `fabric.mod.json` details.
    *   **Build:** Run Gradle tasks.
    *   **Tools:** Access the Texture Editor.
6.  In any sub-menu, select **"Back"** to return to the main menu.

---

[简体中文](README_zh.md)