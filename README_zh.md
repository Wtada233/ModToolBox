# ModToolbox

一个跨平台的、基于终端的 Minecraft Mod 开发工具箱。

该工具提供了一个简单的基于文本的用户界面（TUI），用于管理 Minecraft Mod 开发中的常见任务，旨在同时支持 Windows 和类 Unix 系统。

## 已实现功能

- **项目管理**
  - 通过“切换项目”选项打开和切换不同的 Minecraft Mod 项目目录。
  - 所有构建和配置操作都在当前打开的项目上下文中执行。

- **Mod 配置 (fabric.mod.json)**
  - 读取和写入 `src/main/resources/fabric.mod.json` 文件。
  - 允许实时编辑 Mod 名称 (`name`)、Mod ID (`id`)、版本 (`version`)、描述 (`description`) 和作者 (`authors`)。

- **构建系统集成**
  - 专门的“构建”菜单，用于与 Gradle 包装器交互。
  - 提供执行常见任务的按钮：
    - `build`: 编译 Mod。
    - `genSources`: 反编译 Minecraft 源代码（如果适用）。
    - `runClient`: 启动带有 Mod 的 Minecraft 客户端。
    - `clean`: 清理构建输出。
  - 在 UI 中直接显示执行命令的实时输出。

- **纹理编辑器**
  - 一个简单的 16x16 像素艺术编辑器，用于创建或修改纹理。
  - 支持使用调色板和透明色进行基本绘图。
  - 允许将纹理保存为 PNG 文件。

## 如何构建

### 先决条件
- 兼容 C++17 的编译器 (GCC, Clang, MSVC)
- CMake (3.16 或更高版本)
- Git

### 步骤

```bash
# 1. 创建一个构建目录
mkdir build
cd build

# 2. 使用 CMake 配置项目
cmake ..

# 3. 编译项目
make

# 4. 可执行文件将在 build 目录中生成
# 你可以运行它：
./modtoolbox
```

## 如何使用

1.  运行可执行文件：`./modtoolbox`。
2.  使用 **方向键** 导航菜单。
3.  按 **Enter** 键选择一个选项。
4.  第一步是选择 **“切换项目”** 来打开你的 Mod 项目的根目录。
5.  一旦项目打开，你可以导航到 **“配置”**、**“构建”** 或 **“工具”** 菜单来执行操作。
    *   **配置：** 编辑 `fabric.mod.json` 详细信息。
    *   **构建：** 运行 Gradle 任务。
    *   **工具：** 访问纹理编辑器。
6.  在任何子菜单中，选择 **“返回”** 返回主菜单。

---

[English](README.md)
