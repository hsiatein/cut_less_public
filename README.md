# 一刀切约束下可变尺寸零件的3D切割原料问题（variable-size parts 3D cutting stock problem with guillotine constraint）

## Description

此问题下零件不仅有基础的尺寸，其实际尺寸还可以在一定范围内变化，允许变化的尺寸主要目的是尽可能减少切割次数，因此此问题实际上要同时优化两个目标，利用率和切割次数。

## Build From Source

### Dependencies

* 求解器本身应该不需要依赖，但是可视化使用polyscope，可能需要根据build时的报错信息用apt等包管理器安装相关库。
* 正在尝试支持Windows系统，Ubuntu22.04和MacOS均可使用，其余系统暂未测试。
* 以下构建方法需要CMake，Git，Ninja。

### Build

克隆此仓库
```
git clone https://github.com/hsiatein/cut_less_public.git
cd cut_less_public/
git submodule update --init --recursive
mkdir build
cd build/
```
使用Ninja构建
```
cmake -G Ninja ..
ninja -j4
```

### Executing program

* 目标main包括了求解和可视化的例子
```
./main -p ../assets/test_problem.json -c ../config.json
```
进入可视化界面后，空格刷新，上/下切换解，左/右切换当前解的其他母板切割方案，A/D切换切割刀数

### Input/Output

* cut_less_copy/assets/test_probelms.json是可以接受的数据格式
* json格式的解默认输出到当前工作目录./output/文件夹，解文件名为main : solution.json
* 无论配置文件还是问题或解的json，其中尺寸数值均为1对应0.1mm，qty为数量

## Docker

### Build and Pack
构建并打包成docker镜像，同时.tar格式的镜像会保存在docker目录
```
git clone https://github.com/hsiatein/cut_less_public.git
cd cut_less_public/
git submodule update --init --recursive
bash docker/pack.sh
```
### Build in Debian Container
如果当前平台不能构建，也可以在debian的容器中构建
```
bash docker/build.sh
```
### Build in Alpine Container
需要小体积镜像时可以使用alpine版本的Dockerfile进行构建
```
bash docker/alpine.sh
```
### WEB API Docs
运行容器，可以访问6002接口发送请求
```
docker run -p 6002:6002 cut_less:latest
```
[接口文档](https://bggvbrqt9f.apifox.cn)

## Authors

HsiaTein

Email: mugilovemilk@mail.ustc.edu.cn

## Version History

* 0.1
    * Initial Release

* 2025.9.28
    * 增加了算法自我检查的代码，来定位问题。
    * 错误原因：PatternNode::match在选取方案的时候没有考虑cutloss，导致PatternNode::split时，如果剩余尺寸在cutloss范围里，会得到一个cutloss与一个负尺寸的节点。

* 2025.10.10
    * 错误原因：使用sheet时，未减少对应的数量。

* 2025.10.15
    * 错误原因：没有进行尺寸检查导致sheet放不下pattern。

* 2025.10.17
    * 错误原因：distribute计算错误，本来cutloss不需要分配，但是实际是leftover不分配，导致部分问题尺寸对不上。

## License

This project is licensed under the AGPLv3 License - see the LICENSE file for details

## Acknowledgments

感谢以下仓库的作者（顺序不分先后）
* [json](https://github.com/nlohmann/json)
* [argparse](https://github.com/p-ranav/argparse)
* [googletest](https://github.com/google/googletest)
* [HiGHS](https://github.com/ERGO-Code/HiGHS)
* [xlnt](https://github.com/tfussell/xlnt)
* [polyscope](https://github.com/nmwsharp/polyscope)
* [README-Template](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc)
