# demo说明：

基于fastdds的轻量级通信消息中间件，简单易用，功能强大。

1. 支持dds消息的订阅发布，c++版本见commu_pub.cpp，commu_sub.cpp；python版本见commu_pub.py，commu_sub.py；在ros2中topic命令可以见到对应topic，和ros2生态系统无缝集成；
2. 支持共享内存零拷贝，c++版本commu_pub_zc.cpp，commu_sub_zc.cpp，用于大数据的发送接收，同样支持ros2收发；
3. 同一进程内订阅发布，见commu_p2s.cpp；
4. 简单的线程模型，回调在单一线程内处理，支持postCall，sendCall异步同步函数调用，postMessage，sendMessage异步同步消息调用；支持定时器（见plugin_pub.cpp setTimer）。支持协程，可以无阻塞的处理多个基于marl条件变量的等待。例如可在协程中进行rpc请求，或者将耗时任务分配到别的线程等，可避免client rpc同步请求或耗时任务阻塞线程。见fiber_test函数：normal_client客户端先调用fiber_test函数，后调用addOne，由于有协程支持，fiber_test在服务端被挂起并且不阻塞后面消息，最后是addOne先返回。线程模型的测试见thread_test.cpp。
5. 支持c/c++结构体，json，protobuf消息的发送。、c/c++结构体(可包含字符串，数组，map等stl数据结构)序列化简单，不用编译中间的idl接口，可方便的用于中间件内部层通信，和MCU 的通信；protobuf可用于soc内部通信，或者网关转grpc与云端通信；json可用于网关转http，mqtt，websocket与云端通信，和内部的和python语言编写的节点通信。见normal_pub.cpp，normal_sub.cpp，演示了c/c++数据类型的订阅发布；rpc通信的接口文件中IMyTestCall.h 请求回复数据也使用的是结构体。json，protobuf使用和结构体一样，中间件在内部类型萃取，并序列化和解析，使用简单方便。结构体的序列化测试见struct_serialize_test，json序列化测试见json_serialize_test。
6. 服务的定义使用简单，采用c++的class和虚函数重载即可完成服务的接口定义，和server端的功能描述。rpc支持同步调用，异步回调两种模型。见ServiceTest文件夹。
7. 外部接口简单，不涉及到复杂的c++特性如模板等，只需要常规的c++知识，就能快速的使用。易于编写c api，并方便其他语言如go,python等FFI调用c接口。外部接口API均为多线程安全。
8. 支持插件方式加载，见plugin_pub.cpp,plugin_sub.cpp,运行plugin_test.sh加载pub sub到同一个进程。
9. launch统一启动各个节点进程，配置见launcher.yml文件，运行launch命令。
