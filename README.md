# 🌞 Solar Engine

Solar 是一个基于 **C++20 + CMake + OpenGL** 构建的轻量级图形引擎与编辑器框架项目。

该项目专注于渲染架构设计、事件系统抽象、场景管理机制以及编辑器扩展能力，适合作为：

* 图形引擎学习项目
* OpenGL 架构实践模板
* 游戏引擎核心系统实验平台
* 编辑器系统开发基础框架

---

## 📌 项目特性

### 🚀 核心能力

* ✅ C++20 现代化代码风格
* ✅ 基于 OpenGL 的渲染抽象层
* ✅ 统一事件系统（键盘 / 鼠标 / 窗口事件）
* ✅ 分层架构（Layer System）
* ✅ ImGui 编辑器集成
* ✅ 可扩展场景与实体系统
* ✅ 跨平台构建支持（CMake）

---

## 🏗 项目结构

```
Solar/
│
├── Solar/                     # 引擎核心源码
│   │
│   ├── core/                  # 引擎核心模块
│   │   ├── base/              # Application、Layer、事件系统
│   │   ├── renderer/          # 渲染抽象（Shader、Buffer、Camera）
│   │   ├── scene/             # 场景与实体系统
│   │   ├── imgui/             # ImGui 封装层
│   │   └── utils/             # 工具模块
│   │
│   ├── platform/              # 平台相关实现（窗口、输入等）
│   └── editor/                # 编辑器层逻辑
│
├── vendor/                    # 第三方依赖
├── assets/                    # 资源文件（纹理、Shader等）
├── CMakeLists.txt             # 构建配置
└── README.md
```

---

## 🧠 架构说明

### 1️⃣ Application 架构

引擎采用入口工厂模式：

```cpp
namespace solar {
    Application* CreateApplication();
}
```

* 用户通过实现 `CreateApplication()` 创建自己的 App
* 引擎内部负责生命周期与主循环管理

---

### 2️⃣ Layer 分层系统

Solar 使用 LayerStack 架构：

* 每个功能模块封装为 Layer
* 支持 PushLayer / PushOverlay
* 生命周期函数：

  * `OnAttach()`
  * `OnDetach()`
  * `OnUpdate()`
  * `OnEvent()`

优势：

* 解耦系统逻辑
* 易于扩展功能模块
* 编辑器和运行时共用框架

---

### 3️⃣ 渲染系统

封装内容包括：

* VertexBuffer
* IndexBuffer
* VertexArray
* Shader
* Texture
* Camera

采用渲染抽象层设计，未来可扩展至：

* Vulkan
* DirectX
* Metal

---

### 4️⃣ 事件系统

事件架构包括：

* 基类 `Event`
* 派生事件（KeyEvent / MouseEvent / WindowEvent）
* `EventDispatcher` 分发逻辑

支持：

* 类型安全分发
* 冒泡机制
* 可扩展事件类别

---

### 5️⃣ 编辑器集成（ImGui）

* 实时调试界面
* 参数调节
* 面板式布局
* 可扩展自定义面板

---

## 🛠 构建与部署

### 📋 环境要求

| 工具 | 要求 |
| ------ | -------- |
| CMake | ≥ 3.20 |
| 编译器 | 支持 C++20 |
| OpenGL | ≥ 3.3 |
| Git | 用于子模块 |

---

### 🔧 克隆仓库

```bash
git clone https://github.com/Sylar129/Solar.git
cd Solar
```

---

### 📦 拉取依赖

```bash
git submodule update --init --recursive
```

---

### 🏗 构建项目

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

---

### ▶ 运行程序

Linux / macOS:

```bash
./bin/Solar
```

Windows:

```
Solar.exe
```

---

## 🎮 功能展示

当前实现包括：

* 物体平移
* 物体旋转
* 物体缩放
* 相机控制
* 基础编辑器调试面板

可用于演示：

* 变换矩阵计算
* 渲染管线流程
* UI 与渲染系统交互

---

## 🧩 示例使用

创建自定义应用：

```cpp
#include "Solar.h"

class Sandbox : public solar::Application {
public:
    Sandbox() : solar::Application("Sandbox") {}
};

solar::Application* solar::CreateApplication() {
    return new Sandbox();
}
```

---

## 🔮 可扩展方向

未来可扩展模块：

* ECS 架构重构
* 资源管理系统
* 场景序列化
* 物理系统
* 多线程渲染
* 资产导入工具
* 脚本系统（Lua / Python）

---

## 📄 License

本项目采用 **Apache 2.0 License**。

---

## 🙌 贡献

欢迎提交：

* Pull Request
* Issue
* 架构优化建议
* 渲染后端扩展

---
