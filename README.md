# demo说明：

基于fastdds的轻量级通信消息中间件，简单易用，功能强大。运行环境为ubuntu24，python3.12。

1. 支持dds消息的订阅发布，c++版本见commu_pub.cpp，commu_sub.cpp；python版本见commu_pub.py，commu_sub.py(python3.12)；在ros2中topic命令可以见到对应topic，和ros2生态系统无缝集成。
2. 支持共享内存零拷贝，c++版本commu_pub_zc.cpp，commu_sub_zc.cpp，用于大数据的发送接收，同样支持ros2收发；
3. 同一进程内订阅发布，见commu_p2s.cpp。
4. 简单的线程模型，回调在单一线程内处理，支持异步回调和有栈协程两种异步模型。支持postCall，sendCall线程非阻塞/异步通知、阻塞/同步通知函数调用，postMessage，sendMessage消息调用,线程模型的测试见thread_test.cpp；支持定时器（见plugin_pub.cpp GlobalTimer）。有栈协程，可以无阻塞的处理多个基于marl条件变量的等待。例如可在协程中进行rpc请求，或者将耗时任务分配到别的线程等，可避免client rpc同步请求或耗时任务阻塞线程，详见fiber_client，fiber_server。
5. 支持c/c++结构体，json，protobuf消息的发送，还可以扩展ros1等序列化。中间件在内部类型萃取，并序列化和解析，使用简单方便。c/c++结构体(可包含字符串，数组，map等stl数据结构)序列化简单，不用编译中间的idl接口，可方便的用于中间件内部层通信，和下行MCU，RTOS 系统的通信；protobuf可用于soc内部通信，或者上行grpc与云端通信；json可用于上行http，mqtt，websocket，zenoh与云端通信，和内部的和python语言编写的节点通信。见normal_pub.cpp、normal_sub.cpp演示了c/c++数据类型的订阅发布；normal_client, normal_server，fiber_client，fiber_server示例中，rpc通信的接口文件中IMyTestCall.h、IFiberCall.h中 ，请求应答消息中混合使用了结构体、protobuf、josn数据。结构体的序列化测试见struct_serialize_test，json序列化测试见json_serialize_test。
6. normal_pub.cpp，normal_sub.cpp支持c++结构体、protobuf、json数据的订阅发布；python版本订阅发布json数据测试见normal_pub.py，normal_sub.py。上述用于自定义非ros2 idl数据订阅发布。
7. 服务的实现和使用简单，采用c++的宏定义和虚函数即可完成接口定义，和server端的功能描述。rpc支持同步调用，异步回调(async)两种模型。见ServiceTest文件夹。
8. normal_client, normal_server是请求应答的演示，客户端先调用fiber_test函数，后调用addOne，由于有协程支持，fiber_test在服务端被挂起并且不阻塞后面消息，最后是addOne先返回。fiber_client，fiber_server演示了复杂的协程使用和函数调用，请求为结构体，回复为protobuf；并且有四种协程切换方式：marl sleep，新建线程，单线程队列，rpc请求。json_client.py，fiber_server演示python客户端和json格式数据的请求和回复。
9. 外部接口API简单，并均为多线程安全。应用层调用不涉及到复杂的c++特性，只涉及常规的c++知识，能快速的掌握使用。提供c api，并方便其他语言如云服务中go,python,nodejs等FFI调用，现在已完成python版本的FFI调用，见上面的normal_pub.py、normal_sub.py、json_client.py。回调安全，在reset订阅者和client后，能立即释放回调函数涉及的对象指针。 
10. 支持插件方式加载，见plugin_pub.cpp,plugin_sub.cpp,运行plugin_test.sh加载pub sub到同一个进程。
11. launch统一启动各个节点进程，配置见launcher.yml文件，运行launch命令。
