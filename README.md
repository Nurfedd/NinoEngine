# NinoEngine

**NinoEngine** is a low-level 3D rendering engine built on top of a **Vulkan RHI (Rendering Hardware Interface)**, developed with the goal of exploring modern rendering techniques all the way to **path tracing**, alongside a built-in editor based on **ImGui**.

> ⚠️ **Actively under development.** NinoEngine is a personal project built in my spare time, for learning and experimentation purposes. The API is unstable and subject to frequent changes. It is not (yet) a production-ready engine.

---

## Table of Contents

- [Project Goals](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#project-goals)
- [Architecture](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#architecture)
- [Current Status](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#current-status)
- [Requirements](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#requirements)
- [Installation](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#installation)
- [Usage](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#usage)
- [Roadmap](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#roadmap)
- [Contributing](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#contributing)
- [License](https://claude.ai/chat/23330d63-b956-4660-8e0f-7c0da3c5f125#license)

---

## Project Goals

NinoEngine ultimately aims to:

- Provide a **path tracing** rendering pipeline, leveraging modern Vulkan capabilities (ray tracing pipeline, bindless resources, etc.).
- Offer an **editor** built with ImGui to visualize, debug, and manipulate scenes directly during development.
- Serve as a hands-on playground to deeply understand how a modern rendering engine works, from the RHI up to the final render.

This project is primarily a personal learning exercise. The focus is on understanding and clean architecture rather than fast iteration.

---

## Architecture

NinoEngine is built as a series of layers, each one relying on the previous:

```
├─────────────────────────────────────────┤
│     RDG (Render Dependency Graph)        │
├─────────────────────────────────────────┤
│   Abstraction Layer (Global Shaders,     │
│        resource management, etc.)        │
├─────────────────────────────────────────┤
│         Low-Level Vulkan RHI             │
└─────────────────────────────────────────┘
```

### Vulkan RHI (low-level)

The lowest layer of the engine, wrapping Vulkan objects and concepts directly (instances, devices, swapchains, command buffers, synchronization, etc.) without imposing any higher-level logic. It serves as a stable foundation for the rest of the engine.

### Abstraction Layer

Built on top of the RHI, this layer introduces more ergonomic, higher-level concepts:

- Management of **global shaders** (shaders compiled and shared across the engine, similar to engines like Unreal Engine).
- Resource management abstraction (buffers, textures, pipelines) to reduce Vulkan boilerplate.
- Simplified synchronization and rendering command submission.

### RDG — Render Dependency Graph

The RDG is the system that describes a frame's rendering as a **graph of passes** with their dependencies (read/written resources), instead of manually handling synchronization and scheduling. It notably enables:

- Automatic resolution of barriers and layout transitions.
- Better readability and modularity of rendering code.
- A solid foundation for integrating complex rendering techniques (path tracing, post-processing passes, etc.) without complicating calling code.

---

## Current Status

| Component                          | Status            |
| ---------------------------------- | ----------------- |
| Low-level Vulkan RHI               | ✅ Functional      |
| Abstraction Layer / Global Shaders | 🚧 In development |
| RDG (Render Dependency Graph)      | 🚧 In development |
| Editor (ImGui)                     | 🚧 In development |
| Path Tracing                       | ⏳ Planned         |

---

## Requirements

- A GPU and drivers compatible with **Vulkan 1.2**
- [Vulkan SDK](https://vulkan.lunarg.com/) installed
- A C++ compiler supporting C++20
- [CMake](https://cmake.org/) (specify version)

---

## Installation

To use it simply clones the git and build it and launch the editor executable !

---

## Roadmap

- [x] Low-level Vulkan RHI
- [ ] Finalize the abstraction layer (global shaders, resource management)
- [ ] Finalize the RDG
- [ ] Path tracer implementation
- [ ] ImGui editor (scene, resources, render debugging)

---

## Contributing

As this is primarily a personal learning project, it is not actively open to external contributions at this time. That said, feedback, suggestions, and discussions via issues are welcome.

---


