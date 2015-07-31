# cocos2dx-wheels
收集cocos2dx使用中的发明的一些轮子


##VS2013/VS2015配置
1. 下载cocos2dx v3.7
2. clone项目
3. 随便创建一个c++工程
4. 将生成的工程下的cocos2d目录拷贝到cocos2dx-wheels目录下
5. 针对VS2015可能要做如下改动：打开cocos2d\cocos\platform\win32\CCStdC-win32.h，在53行的#ifndef snprintf前，加#if _MSC_VER < 1900，别忘了匹配#endif
6. 用VS2013/VS2015打开proj.win32目录下的cocos2dx-wheels.sln
7. 编译完成后可以将另外五个项目都卸载了

##xcode配置
1. 前4步同VS2013
2. 用xcode打开proj.ios_mac目录下的Cocos2dxWheels.xcodeproj
PS: 如果编译ios项目报找不到Prefix.pch，则新建一个，复制粘贴如下代码
```Objective-C
#ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #import <UIKit/UIKit.h>
#endif
```
