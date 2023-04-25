#pragma once

#include <mutex>
#include <map>
#include <string>
#include <thread>

#include "proto/kv.pb.h"

namespace simple_kafka {

class QueryServiceImpl : public QueryService {
public:
    QueryServiceImpl();

    virtual void GetValue(google::protobuf::RpcController *cntl_base,
                           const KeyRequest *request,
                           ValueResponse *response,
                           google::protobuf::Closure *done);

    void consume();
    void start_consumer_thread();
    void stop_consumer_thread();

private:
    std::map<std::string, std::string> kv_map;
    std::mutex map_mutex;
    std::atomic<bool> running{true};
    std::thread kafka_consumer_thread;
};

} // end of namespace simple_kafka
