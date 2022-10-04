# 概述
rendu-cxx-template是c++的cmake构建模版。

目前包含cmake，deps，example，src，tests模块，后续添加 配置文件，脚本等模块。
模版目的是简化项目构建配置，以达到开发过程中不用编写cmake，尽量只关注c++代码实现。
模版结构如下图所示

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664115392523-39d2e152-d5ab-477a-8713-0da3d3eb8a71.png#clientId=uf32e7b2e-aa61-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=223&id=u9847b3f4&margin=%5Bobject%20Object%5D&name=image.png&originHeight=223&originWidth=251&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14605&status=done&style=none&taskId=u108babb1-92ce-4e83-8598-0b2d6a07b96&title=&width=251)

# 简介
## cmake模块

cmake项目框架通用cmake封装

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664870318106-1455f244-38ac-4bf6-bece-37a3d0fc14d8.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=161&id=u3a21a85e&margin=%5Bobject%20Object%5D&name=image.png&originHeight=161&originWidth=200&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22417&status=done&style=none&taskId=u81972000-d44b-4e0f-b366-3f95847809f&title=&width=200)
## deps模块

一些外部依赖库

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664870659832-c0eabac6-0ed5-4adb-b63d-6d0a7f68c3b9.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=205&id=u983d954f&margin=%5Bobject%20Object%5D&name=image.png&originHeight=205&originWidth=234&originalType=binary&ratio=1&rotation=0&showTitle=false&size=11656&status=done&style=none&taskId=u759100a4-b8cb-4447-8fbd-33ca79b5a6d&title=&width=234)

## src模块
源代码（就是实际开中自己写的代码）

包括以下4个模块。commom，core，example，genrev

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871056442-4bd3761d-a4f7-496b-87fe-733f97070336.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=124&id=u8fda5d61&margin=%5Bobject%20Object%5D&name=image.png&originHeight=124&originWidth=155&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5383&status=done&style=none&taskId=ud36f7896-8237-451a-a1ee-ba21d1a8ca7&title=&width=155)

### common模块

通用模块。基础的通用代码模块

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871375159-8a568116-cdfa-47da-9e1b-496ecca8defe.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=379&id=u4d307df2&margin=%5Bobject%20Object%5D&name=image.png&originHeight=379&originWidth=239&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17023&status=done&style=none&taskId=ucba2be25-e2c0-4dbc-9f85-04d97b6d2dd&title=&width=239)

### core模块

项目核心，不同项目有各自的不同的实现

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871633499-7c7eb6f9-9a60-474a-8d4d-e502ac505b7f.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=100&id=udb533c61&margin=%5Bobject%20Object%5D&name=image.png&originHeight=100&originWidth=198&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12440&status=done&style=none&taskId=u67aa63a8-10e6-4a1b-9fdf-4d5cb0c6966&title=&width=198)

### example模块
例子模块

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871521718-5c5547a3-d950-48ee-97a0-ce5153563318.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=57&id=kbu2k&margin=%5Bobject%20Object%5D&name=image.png&originHeight=57&originWidth=167&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7924&status=done&style=none&taskId=ub4de07e4-4375-4fa9-b26d-beb3433208d&title=&width=167)

### genrev模块

生成项目版本信息 

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871549064-39cae7a6-80b7-4f00-b748-56504c3472ec.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=39&id=bzUgu&margin=%5Bobject%20Object%5D&name=image.png&originHeight=39&originWidth=178&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3175&status=done&style=none&taskId=u9d665256-68a1-427d-bc56-073309a3e9a&title=&width=178)
从项目中的revision_data.h.in.cmake文件生成版本内容代码（revision_data.h）

## tests模块

unittest单元测试模块

![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871072168-bbea7d13-7bd7-48e9-8008-ebd9fd7499fe.png#clientId=u0bc2c7f2-1f26-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=141&id=u846986fe&margin=%5Bobject%20Object%5D&name=image.png&originHeight=141&originWidth=183&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6759&status=done&style=none&taskId=u2d4ffad2-1478-482f-be92-ea4ec8ebefd&title=&width=183)
# 使用

这个框架只需要添加对映模块的文件夹和代码即可，几乎不用关心cmake代码。
以下为概要（详细说明参照后续）
## 添加第三方依赖
### 源码方式引入
在deps文件夹添加对映库源码
### git方式引入
在deps文件夹cmakelist.txt文件中添加对映cmake引入代码
## 添加单元测试
tests文件夹中添加单元测试的cpp代码（依赖catch2单元测试）
## 添加例子程序
example文件夹中添加例子程序代码
## 添加项目相关核心代码
src文件夹中添加功能代码
### 1、添加通用实现
common文件夹中通用代码
### 2、添加项目相关实现
core文件夹中添加实现代码
### 3、添加例子
example文件夹中添加例子代码

