# 学生成绩管理系统
C语言课程作业，做着玩的。

版本：v1.13 (20240221)

## 特色
- 酷炫命令行 UI <kbd>方向键控制、绚丽色彩、转场动画</kbd>
- 统计信息
- 查看数据和快速排序
- 单独添加和编辑学生
- 导入导出 CSV 文件
- 一键生成演示数据 <kbd>~~猫猫生成器~~</kbd>
- 打开和保存对话框使用系统组件 <kbd>~~直接调的 powershell~~</kbd>
- 写 死 的 三 项 课 程
- 没 有 头 文 件 <kbd>全是直接塞进去的 `#include "xxx.c"`</kbd>


> [!TIP]
> #### 即将推出 `VoidEra UI` python 库，轻松制作交互式命令行工具
> ###### 这里的烂摊子就别管它了（（

## 运行
直接下载 [发布版本](https://github.com/zetaloop/stuman/releases)，或者手动编译。

## 编译
仅支持 Windows 系统。
|文件|信息|
|---|---|
stuman.c|主程序，请打开这个
utils.c|杂项功能
printflim.c|输出控制
tablemath.c|统计属性计算和排序
fileloader.c|文件读写
filedlg.c|弹出选择文件对话框

用 VSCode 打开项目文件夹，通过 Code Runner 插件用 GCC 编译即可。

（会玩的话用别的编辑器也无妨）

> [!CAUTION]
> 请勿使用 <kbd>DevCpp</kbd>、<kbd>CodeBlocks</kbd> 等不支持 UTF-8 编码的编辑器，输出会乱码。

## 展示

![image](https://github.com/zetaloop/stuman/assets/36418285/86145437-120d-4495-a68a-5a6bf9e088ea)
![image](https://github.com/zetaloop/stuman/assets/36418285/2e917b73-79b9-4437-bc87-443958198b24)
###### MIT 许可证开源 ~~谢谢惠顾喜欢记得点个收藏喵~~
