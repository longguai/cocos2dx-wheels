# cocos2dx-wheels
收集cocos2dx使用中的发明的一些轮子


##VS2013配置：
1. 下载cocos2dx v3.4 final
2. clone项目
3. 随便创建一个c++工程
4. 将生成的工程下的cocos2d目录拷贝到cocos2dx-wheels目录下
5. 用VS2013打开proj.win32目录下的cocos2dx-wheels.sln

##xcode配置
1. 前4步同VS2013
2. 用xcode打开proj.ios_mac目录下的cocos2dx-wheels.xcodeproj
PS: 如果编译ios项目报找不到Prefix.pch，则新建一个，复制粘贴如下代码
```Objective-C
#ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #import <UIKit/UIKit.h>
#endif
```

