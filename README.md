# 基于人体动作识别的四足机器人控制系统
设计了一种基于人体动作识别的四足机器人控制系统。在该系统的控制下，人机交互界面可实时显示用户身体动作和机器人周围环境，并能根据用户需要对四足机器人实现实时控制。

通过Kinect采集与显示人体彩色图像和深度图，准确读取多个关节坐标并根据阈值识别4个人体动作。在封装的底层代码的基础上设计四足机器人的程序，能实现机器人直线行走、转弯等运动控制；
设计上位机软件，实现了界面可视化、人体骨架显示、动作识别、远程控制等功能，设计控制系统程序，实现视频采集设备与机器人之间的无线通讯功能，四足机器人根据人体动作完成对应任务的有效调度。

机器人是宇树科技的laikago。
