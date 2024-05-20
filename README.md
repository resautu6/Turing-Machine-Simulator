## 软件依赖

本框架依赖于cmake进行编译，需安装3.13.0以上版本的cmake方可使用。



## 使用方法

本框架的使用方法为：

1. 将所有`.h` 以及`.cpp`文件置于`/turing-project`文件夹下，或者将该框架的`CMakeLists.txt`复制至你的工程中的相应位置；
2. 在含有`CMakeLists.txt`的文件夹下，使用指令`cmake -B build`；
3. 在含有`CMakeLists.txt`的文件夹下，使用指令`cd ./build; make`。

测试框架将使用上述方法对工程进行编译。执行完毕后，在工程根目录下应当出现`/bin`文件夹，其中含有可执行文件`turing`，测试框架将对该可执行文件进行测试。

**重点**：提交时，你的工程根目录下应当有一个`CMakeLists.txt`文件用于指导cmake进行编译，且通过这种方式能够在`/bin`文件夹下生成可执行文件`turing`。在满足以上条件的前提下，你可以按实际需求对`CMakeLists.txt`的内容自行修改。同时，在`/programs`文件夹下，应有你编写`case1.tm`和`case2.tm`作为测试样例。

