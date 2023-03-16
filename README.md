# 概述
RenduCore是一个modern c++项目。
先从项目结构开始。使用rendu-cxx-template模版。
rendu-cxx-template是c++的cmake构建模版。
模版 ：[https://github.com/FRBoiling/rendu-cxx-template](https://github.com/FRBoiling/rendu-cxx-template)
目前包含cmake，dep，example，rendu，test，tool模块，后续添加 配置文件，脚本等模块。
模版目的是简化项目构建配置，以达到开发过程中尽量减少编写cmake，只关注c++代码实现的目的。
模版结构如下图所示
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953231938-9f5bf8a0-6292-4882-ad4f-bc7e44c5dca0.png#averageHue=%233d4144&clientId=ueae1df6c-2a2c-4&from=paste&height=266&id=u06701cab&name=image.png&originHeight=266&originWidth=332&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9148&status=done&style=none&taskId=uc949724b-3a2c-4e23-be44-3b8f4f833bb&title=&width=332)
# 简介
## cmake模块
cmake项目框架通用cmake封装
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953278074-7d93df36-db53-42cd-a46a-e501d5813a85.png#averageHue=%233e4246&clientId=ueae1df6c-2a2c-4&from=paste&height=144&id=u40632b56&name=image.png&originHeight=144&originWidth=218&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3886&status=done&style=none&taskId=u3fa1e6c6-d647-470e-9d95-3b00b32dc98&title=&width=218)
## deps模块
一些外部依赖库
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953292026-80743314-c840-437b-86bb-b13daf6adcd6.png#averageHue=%233f4346&clientId=ueae1df6c-2a2c-4&from=paste&height=278&id=ufb0fb7be&name=image.png&originHeight=278&originWidth=196&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6063&status=done&style=none&taskId=u339e1214-25f4-425b-a9ba-22ec190f6e1&title=&width=196)
## rendu模块
源代码（就是实际开中自己写的代码）
包括以下4个模块。commom，core，example，genrev
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953309861-c5e8e508-5622-415b-9227-ab9afbd0226e.png#averageHue=%23404447&clientId=ueae1df6c-2a2c-4&from=paste&height=116&id=u931a0ebf&name=image.png&originHeight=116&originWidth=158&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3049&status=done&style=none&taskId=uda0e1d9d-146d-43c2-814d-060be7a91cd&title=&width=158)
### common模块
通用模块。基础的通用代码模块
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953337188-ea6140f6-d5d3-488d-a3a1-3b61fab603ea.png#averageHue=%233e4246&clientId=ueae1df6c-2a2c-4&from=paste&height=140&id=ue2a04dc3&name=image.png&originHeight=140&originWidth=220&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4057&status=done&style=none&taskId=u6c5aa61a-ee7a-4ece-8994-9ab2e27baf0&title=&width=220)
### core模块
项目核心，不同项目有各自的不同的实现
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953350209-5141d448-7649-41c0-944a-75367aba0e75.png#averageHue=%233b4145&clientId=ueae1df6c-2a2c-4&from=paste&height=280&id=u8cd0574f&name=image.png&originHeight=280&originWidth=248&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6849&status=done&style=none&taskId=u5dfaa025-3747-4da5-9efe-4441db4c10b&title=&width=248)
### example模块
例子模块
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953370256-f5d466ba-3012-4647-9088-5e43c16dfd4d.png#averageHue=%233f4346&clientId=ueae1df6c-2a2c-4&from=paste&height=296&id=uc345badc&name=image.png&originHeight=296&originWidth=202&originalType=binary&ratio=1&rotation=0&showTitle=false&size=6647&status=done&style=none&taskId=u1e727338-a997-4cf6-9a76-8c9b1e13989&title=&width=202)
### genrev模块
生成项目版本信息
![image.png](https://cdn.nlark.com/yuque/0/2022/png/29336172/1664871549064-39cae7a6-80b7-4f00-b748-56504c3472ec.png#averageHue=%23404547&clientId=u0bc2c7f2-1f26-4&from=paste&height=39&id=bzUgu&name=image.png&originHeight=39&originWidth=178&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3175&status=done&style=none&taskId=u9d665256-68a1-427d-bc56-073309a3e9a&title=&width=178)
从项目中的revision_data.h.in.cmake文件生成版本内容代码（revision_data.h）
## tests模块
unittest单元测试模块
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953394796-0773f88b-6fba-4e4b-bc21-c4585f502b29.png#averageHue=%233f4346&clientId=ueae1df6c-2a2c-4&from=paste&height=159&id=u0777159d&name=image.png&originHeight=159&originWidth=218&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4380&status=done&style=none&taskId=u56747be6-0001-43de-8af6-0e3d495be29&title=&width=218)
# 使用
这个框架只需要添加对映模块的文件夹和代码即可，几乎不用关心cmake代码。
以下为概要（详细说明参照后续）
## 添加第三方依赖
### 源码方式引入
在dep文件夹添加对应第三方库源码
比如：fmt库，只需添加类似如下
```cmake
set(target_name fmt)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-dependency-interface
    DEPS
)

```
## 添加单元测试
tests文件夹中添加单元测试的cpp代码（依赖googletest单元测试）
比如：enum单元测试
![image.png](https://cdn.nlark.com/yuque/0/2023/png/29336172/1678953551489-8a10e9eb-962d-4891-b1f7-f4a162a2af9c.png#averageHue=%233e4246&clientId=ueae1df6c-2a2c-4&from=paste&height=107&id=ue3b64b1d&name=image.png&originHeight=107&originWidth=207&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3630&status=done&style=none&taskId=ue79572af-fa5f-4277-bd7a-44d876e500b&title=&width=207)
```cmake
set(target_name enum)
rendu_add_test(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
    dep::magic_enum
    rendu::test
    DEFINES
    LINKOPTS
)
```
## 添加例子程序
example文件夹中添加例子程序代码。
例如：argparse的example 可执行文件如下。
```cmake
set(target_name argparse)
rendu_add_executable(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
    dep::argparse
    dep::magic_enum
    DEFINES
    LINKOPTS
)
```
## 添加项目相关核心代码
rendu文件夹中添加功能代码
### 1、添加通用实现库
common文件夹中通用代码
```cmake
set(target_name common)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
)

```
### 2、添加逻辑实现库
core文件夹中添加实现代码
```cmake
set(target_name core)
rendu_add_library(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
    rendu::common
)


```
### 3、添加启动进程
launcher文件夹中添加启动进程代码
```cmake
set(target_name launcher)
rendu_add_executable(
    DIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    PROJECT
    ${PROJECT_NAME}
    NAME
    ${target_name}
    SETTING
    rendu-core-interface
    DEPS
    rendu::core
    rendu::common
    DEFINES
    LINKOPTS
)
```
