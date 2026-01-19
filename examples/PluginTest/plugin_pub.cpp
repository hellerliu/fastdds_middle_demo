
#include "RpcCommu/ChannelPublisher.h"
#include "helloworld/msg/HelloWorldPubSubTypes.hpp"
#include "RpcCommu/Node.h"
#include "RpcCore/CoreGlobal.h"
#include "RpcCore/TmFiberThread.h"

#include <functional>
#include <unistd.h>

using namespace RpcCommu;
using namespace helloworld::msg;

class PubTestNode : public RpcCommu::Node
{

public:
    PubTestNode()
    {
        printf("PubTestNode\n");
        publisher_ = ChannelPublisher<HelloWorldPubSubType>::create("hello_world");
        msg_.index(0);

        auto func = [this]()
        {
            msg_.index(msg_.index() + 1);
            msg_.data()[0] = 1;
            msg_.data()[1] = 2;
            msg_.message("---HelloWorld---");
            int res = publisher_->publish(msg_);
            printf("pub:%d %d\n", msg_.index(), res);
        };
        timer_pub_ = rpc::GlobalTimer::create(std::move(func), 1000, true);
    }

    ~PubTestNode()
    {
        // rpc::get_handle_thread()->killTimer(time_id_, true);
        printf("PubTestNode release\n");
    }

private:
    ChannelPublisher<HelloWorldPubSubType>::Ptr publisher_;
    HelloWorld msg_;
    rpc::GlobalTimer::Ptr timer_pub_;
};

COMPONENTS_REGISTER_NODE(PubTestNode)