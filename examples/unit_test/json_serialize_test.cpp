#include <RpcBase/Serialization.h>

namespace Data
{
    struct SimpleStruct
    {
        bool bool_;
        uint64_t int_;
        double double_;
        std::string string_;
        std::optional<bool> optional_;
    };

    struct NestedStruct
    {
        SimpleStruct nested_;
        std::optional<std::vector<SimpleStruct>> vector_;
    };

    struct ArrayStruct
    {
        float float_[2];
    };

    struct JsonValue
    {
        nlohmann::json value;
    };
}

namespace reflect
{
    DEFINE_STRUCT_SCHEMA(Data::SimpleStruct,
                         DEFINE_STRUCT_FIELD(bool_),
                         DEFINE_STRUCT_FIELD(int_),
                         DEFINE_STRUCT_FIELD(double_),
                         DEFINE_STRUCT_FIELD(string_),
                         DEFINE_STRUCT_FIELD(optional_));

    DEFINE_STRUCT_SCHEMA(Data::NestedStruct,
                         DEFINE_STRUCT_FIELD(nested_),
                         DEFINE_STRUCT_FIELD(vector_));

    DEFINE_STRUCT_SCHEMA(Data::ArrayStruct,
                         DEFINE_STRUCT_FIELD(float_));

    DEFINE_STRUCT_SCHEMA(Data::JsonValue,
                         DEFINE_STRUCT_FIELD(value));

    struct MapStruct
    {
        std::map<int, std::string> map_;
    };
    DEFINE_STRUCT_SCHEMA(MapStruct,
                         DEFINE_STRUCT_FIELD(map_));

}

int main()
{
    std::string out;

    Data::JsonValue v1;
    v1.value["t1"] = 1;
    v1.value["t2"] = "123";
    rpc::serializeToString(v1, out);
    printf("nlohmann:%s\n", out.c_str());

    nlohmann::json_array<Data::SimpleStruct> array, array1;
    printf("getSerializeType %s %d %d\n", rpc::getTypeName(array).c_str(), rpc::getSerializeType<Data::SimpleStruct>(), rpc::getSerializeType<nlohmann::json_array<Data::SimpleStruct>>());
    array.array.push_back({false, 1, 2.3, "123", true});
    array.array.push_back({true, 3, 4.5, "678", false});
    rpc::serializeToString(array, out);
    rpc::parseFromString(out, array1);
    printf("json:%s\n", out.c_str());
    assert(array1.array.size() == 2);

    nlohmann::json_array<std::string> arrayStr;
    arrayStr.array.push_back({"123"});
    arrayStr.array.push_back({"456"});
    rpc::serializeToString(arrayStr, out);
    printf("json:%s\n", out.c_str());
    printf("getSerializeType %s\n", rpc::getTypeName(arrayStr).c_str());

    printf("getSerializeType %s %d %d %d %d\n", rpc::getTypeName<Data::NestedStruct>().c_str(), rpc::getSerializeType<Data::NestedStruct>(),
           rpc::getSerializeType<const Data::NestedStruct>(), rpc::getSerializeType<Data::NestedStruct &>(), rpc::getSerializeType<Data::NestedStruct *>());

    Data::NestedStruct data, data1;
    data.nested_.bool_ = true;
    data.nested_.double_ = 1.2;
    data.nested_.int_ = std::numeric_limits<uint64_t>::max() - 1;
    data.nested_.string_ = "te\x01\x02st";
    //data.nested_.optional_ = false;
    std::vector<Data::SimpleStruct> tmp;
    tmp.push_back({false, 1, 2.3, "123", true});
    tmp.push_back({true, 3, 4.5, "678", false});
    data.vector_ = std::move(tmp);

    rpc::serializeToString(data, out);
    rpc::parseFromString(out, data1);
    printf("json:%s\n", out.c_str());
    assert(data1.nested_.optional_.has_value() == false);
    //assert(data1.nested_.int_ == 123);
    assert(data1.nested_.string_ == "te\x01\x02st");
    assert(data1.nested_.string_.size() == 6);

    std::string out1 = "{"
                       "  \"nested_\": {"
                       "    \"bool_\": false,"
                       "    \"int_\": 0,"
                       "    \"double_\": 0,"
                       "    \"string_\": \"foo\""
                       "  },"
                       "  \"vector_\": [{"
                       "    \"bool_\": true,"
                       "    \"int_\": 1,"
                       "    \"double_\": 1,"
                       "    \"string_\": \"bar\","
                       "    \"optional_\": true"
                       "  },{"
                       "    \"bool_\": true,"
                       "    \"int_\": 2,"
                       "    \"double_\": 2.0,"
                       "    \"string_\": \"baz\","
                       "    \"optional_\": false"
                       "  }]"
                       "}";
    rpc::parseFromString(out1, data1);
    rpc::serializeToString(data1, out);
    printf("json:%s\n", out.c_str());

    Data::ArrayStruct arr, arr1;
    arr.float_[0] = 1.0f;
    arr.float_[1] = 2.0f;
    rpc::serializeToString(arr, out);
    rpc::parseFromString(out, arr1);
    printf("json:%s\n", out.c_str());
    printf("arr1 :%f %f\n", arr1.float_[0], arr1.float_[1]);

    reflect::MapStruct map, map1;
    map.map_ = {{1, "123"}, {2, "456"}};
    rpc::serializeToString(map, out);
    rpc::parseFromString(out, map1);
    printf("json:%s\n", out.c_str());
    printf("map1:\n");
    for (auto &item : map1.map_)
    {
        printf("%d %s\n", item.first, item.second.c_str());
    }
    return 0;
}