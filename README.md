# JSBSim HOTAS 飞行仿真系统

## 项目概述

本项目是一个基于 Windows 平台的**六自由度（6-DOF）飞行仿真系统**，飞行动力学仿真核心由开源飞行动力学模型引擎 [JSBSim](https://github.com/JSBSim-Team/jsbsim) 实现，默认仿真机型为 **F-16 战斗机**。

该系统通过**图马斯特 A10C（Thrustmaster A-10C）**套件完成外部飞行控制量的采集，套件包含以下三款设备：`Joystick - HOTAS Warthog`（摇杆）、`Throttle - HOTAS Warthog`（油门台）、`Thrustmaster T-Pendular-Rudder`（方向舵踏板）。软件层面通过 SDL3 库实时采集各设备的轴量、按钮及帽形开关状态，并将操纵指令输入到 JSBSim 飞行仿真引擎中，驱动六自由度飞行模型实时运算。

## 主要功能

- **六自由度飞行仿真**：由 JSBSim 完成气动模型解算，输出飞机位置（经纬度、海拔高度）、姿态角（偏航角、俯仰角、滚转角）、速度（真速、表速、地速）、攻角、侧滑角等飞行参数。
- **HOTAS 硬件接入**：通过 SDL3 实时读取**图马斯特 A10C** 套件中以下三款设备的操纵量：
  - 摇杆：`Joystick - HOTAS Warthog`（俯仰、滚转轴）
  - 油门台：`Throttle - HOTAS Warthog`（油门、升降舵配平、按钮等）
  - 方向舵踏板：`Thrustmaster T-Pendular-Rudder`（偏航轴）
- **飞行数据显示**：通过 **HSChartCtrl** 图表控件在 MFC 界面上实时显示仿真数据，包括：
  - HOTAS 系统的摇杆量、油门量、踏板量曲线图
  - 飞机位置（水平轨迹图、高度–时间曲线）
  - 飞行速度（真速、表速、地速）
  - 发动机参数（N2 转速、推力、推重比）等信息
- **Tacview 实时遥测**：仿真过程中通过 TCP 接口将实时飞行信息推送至 Tacview，飞行数据以三维视景实时呈现；同时将飞行信息写入 `.acmi` 结果文件，支持仿真结束后的三维回放。
- **仿真控制**：支持初始化、启动、暂停、继续、停止、复位等仿真流程控制。

## 技术栈

| 组件 | 说明 |
|---|---|
| 飞行仿真引擎 | [JSBSim](https://github.com/JSBSim-Team/jsbsim) |
| 操纵设备输入 | [SDL3](https://github.com/libsdl-org/SDL)（摇杆/HOTAS 驱动） |
| 三维可视化 | [Tacview](https://www.tacview.net/)（TCP 实时流 + ACMI 文件） |
| 应用框架 | Windows MFC（C++，Visual Studio） |
| 图表控件 | HSChartCtrl |

## 仿真参数

| 参数 | 值 |
|---|---|
| 默认仿真机型 | F-16 |
| 系统主循环步长 | 10 ms（100 Hz） |
| JSBSim 积分步长 | 1/120 s（120 Hz） |
| Tacview 推送间隔 | 500 ms |

## 开发环境

- 操作系统：Windows 10/11（x64）
- 开发工具：Visual Studio（MFC 工作负载）
- 构建配置：`x64 / Debug`

## 目录结构

```
jsbsim-hotas-eval/
├── MFCApplication-jsbsim-hotas/   # 主工程源码
│   ├── CMyHotas.h / .cpp          # HOTAS 设备接入封装（SDL3）
│   ├── CMyJSBSim.h / .cpp         # JSBSim 飞行仿真封装
│   ├── Simulator.h / .cpp         # 仿真主控逻辑
│   ├── SimThread.h / .cpp         # 仿真线程管理
│   ├── ShareVar.h                 # 系统共享数据结构与消息定义
│   ├── CSysCtrlView               # 仿真控制视图
│   ├── CSysHotasShowView          # HOTAS 数据显示视图
│   ├── CSysJsbsimShowView         # JSBSim 数据显示视图
│   ├── HSChartCtrl/               # 图表控件库
│   ├── SDL3-3.3.4/                # SDL3 依赖库
│   ├── tacview/                   # Tacview 接口库
│   └── results/                   # 仿真结果输出目录（ACMI 文件）
└── MFCApplication-jsbsim-hotas.sln  # Visual Studio 解决方案文件
```
