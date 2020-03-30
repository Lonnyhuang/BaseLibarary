# BaseLibarary
基础组件，使用标准C++开发，需要考虑跨平台，不使用Qt等依赖性较强的库，引入使用标准C++开发的第三方库。
该模块各组件之间需要按各自目录建立。直接引入相关库文件即可

filesytem: 文件操作库
Global：公共使用，必须引入
HThread: 封装的线程库
HThreadPool: 根据C++11线程封装的线程池
HTimer: 定时器
Http： http网络库
Libraryload: 动态库加载
socket: 根据libuv封装的网络库
TimeSync: ntp时间同步