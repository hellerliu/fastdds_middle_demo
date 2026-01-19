
#include <unordered_map>
#include "RpcBase/Serialization.h"

namespace test
{
    template <class Type>
    struct TempTest
    {
    };
}

struct Base1
{
    int xxx = 1;
};

struct Base2
{
    int yyy = 1;
};

struct Derive : public Base1, public Base2
{
};

struct BaseStr
{
    std::string xxx;
    template <typename T>
    void serialize(T &ar)
    {
        ar &xxx;
    }
};

struct DeriveStr : public BaseStr, public Base2
{
    template <typename T>
    void serialize(T &ar)
    {
        BaseStr *ptr = static_cast<BaseStr *>(this);
        Base2 *ptr1 = static_cast<Base2 *>(this);
        ar &(*ptr) & (*ptr1);
    }
};

struct DeriveStr1 : public BaseStr
{
};

struct DeriveStr2 : public Base1
{
    std::string str;
    template <typename T>
    void serialize(T &ar)
    {
        Base1 *ptr1 = static_cast<Base1 *>(this);
        ar &str &(*ptr1);
    }
};

struct ArrayTest
{
    int a[10];
    std::string b;

    template <typename T>
    void serialize(T &ar)
    {
        ar &a &b;
    }
};

struct ArrayTest1
{
    std::string a[2];
    std::string b;

    template <typename T>
    void serialize(T &ar)
    {
        ar &a &b;
    }
};



int main()
{
    Derive data, data1;
    data.xxx = 5;
    data.yyy = 6;
    std::string out;
    rpc::serializeToString(data, out);
    rpc::parseFromString(out, data1);
    printf("Derive: %d %d\n", data1.xxx, data1.yyy);

    DeriveStr tmp, tmp1;
    tmp.xxx = "123";
    tmp.yyy = 8;
    rpc::serializeToString(tmp, out);
    rpc::parseFromString(out, tmp1);
    printf("DeriveStr: %s %d\n", tmp1.xxx.c_str(), tmp1.yyy);

    DeriveStr1 dev, dev1;
    dev.xxx = "456";
    rpc::serializeToString(dev, out);
    rpc::parseFromString(out, dev1);
    printf("DeriveStr1: %s\n", dev1.xxx.c_str());

    DeriveStr2 des, des1;
    des.str = "789";
    des.xxx = 1;
    rpc::serializeToString(des, out);
    rpc::parseFromString(out, des1);
    printf("DeriveStr2: %d %s\n", des1.xxx, des1.str.c_str());

    ArrayTest arr, arr1;
    for (int i = 0; i < 10; i++)
    {
        arr.a[i] = i;
    }
    rpc::serializeToString(arr, out);
    rpc::parseFromString(out, arr1);
    printf("ArrayTest: %d %d %s\n", arr1.a[0], arr1.a[9], rpc::getTypeName(arr.a).c_str());

 

    std::string strArray[2], strArray1[2];
    strArray[0] = "123";
    strArray[1] = "456";
    rpc::serializeToString(strArray, out);
    rpc::parseFromString(out, strArray1);
    printf("strArray: %s %s %s %s\n", rpc::getTypeName<std::string[]>().c_str(), rpc::getTypeName(strArray1).c_str(),
           strArray[0].c_str(), strArray[1].c_str());

    ArrayTest1 ass, ass1;
    ass.a[0] = "12";
    ass.a[1] = "34";
    ass.b = "56";

    rpc::serializeToString(ass, out);
    rpc::parseFromString(out, ass1);
    printf("ArrayTest1: %s %s %s\n", ass1.a[0].c_str(), ass1.a[1].c_str(), ass1.b.c_str());

    std::map<int, std::string> map = {{1, "123"}, {2, "456"}}, map1;
    rpc::serializeToString(map, out);
    rpc::parseFromString(out, map1);
    for (auto &item : map1)
    {
        printf("map: %d %s\n", item.first, item.second.c_str());
    }

    std::vector<std::string> vec = {"123", "456"}, vec1;
    rpc::serializeToString(vec, out);
    rpc::parseFromString(out, vec1);
    for (auto &item : vec1)
    {
        printf("vector: %s\n", item.c_str());
    }

    std::optional<int> opt, opt1;
    rpc::serializeToString(opt, out);
    rpc::parseFromString(out, opt1);
    printf("opt:%d \n", out.size());
    assert(opt1.has_value() == false);

    std::optional<std::string> opts = "123", opts1;
    rpc::serializeToString(opts, out);
    rpc::parseFromString(out, opts1);
    printf("opts:%d \n", out.size());
    assert(opts1.has_value() == true);
    printf("opts1:%s \n", opts1.value().c_str());
    return 0;
}