#include <iostream>
#include <thread>
#include <csignal>
#include <string>
#include <mutex>
#include <brpc/server.h>
#include <butil/logging.h>
#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>

#include "proto/kv.pb.h"

using json = nlohmann::json;

using cppkafka::Configuration;
using cppkafka::Consumer;
using cppkafka::Message;

std::map<std::string, std::string> kv_map;
std::mutex map_mutex;

void consume() {
    LOG(INFO) << "Starting consumer thread...";
    Configuration config = {
        { "bootstrap.servers", "kafka:9092" },
        { "group.id", "cpp-consumer-group" },
        { "auto.offset.reset", "earliest" }
    };
    Consumer consumer(config);
    std::string topic = "test";
    consumer.subscribe({ topic });
    while (true) {
        auto msg = consumer.poll();
        if (msg) {
            if (msg.get_error()) {
                LOG(ERROR) << "Error while consuming: " << msg.get_error();
            }
            else {
                json j = json::parse(msg.get_payload());
                std::string key = j["key"];
                std::string value = j["value"];
                {
                    std::unique_lock<std::mutex> lock(map_mutex);
                    kv_map[key] = value;
                }
                LOG(INFO) << j.dump();
            }
        }
    }
}

namespace simple_kafka {
class QueryServiceImpl : public QueryService {
public:
    QueryServiceImpl() {}

    virtual void GetValue(google::protobuf::RpcController *cntl_base,
                           const KeyRequest *request,
                           ValueResponse *response,
                           google::protobuf::Closure *done) {
        brpc::ClosureGuard done_guard(done);
        brpc::Controller *cntl = static_cast<brpc::Controller *>(cntl_base);

        std::string key = request->key();
        std::string value;

        {
            std::unique_lock<std::mutex> lock(map_mutex);
            auto it = kv_map.find(key);
            if (it != kv_map.end()) {
                value = it->second;
            }
        }

        if (value.empty()) {
            std::string s = "key not found: " + key;
            cntl->SetFailed(s);
        } else {
            response->set_value(value);
        }
    }
};
} // namespace simple_kafka

int main(int argc, char* argv[]) {

    std::thread kafka_consumer_thread(consume);
    brpc::Server server;
    simple_kafka::QueryServiceImpl query_service_impl;

    if (server.AddService(&query_service_impl,
                           brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Failed to add query_service_impl";
        return -1;
    }

    brpc::ServerOptions options;
    options.idle_timeout_sec = -1;

    if (server.Start(8888, &options) != 0) {
        LOG(ERROR) << "Failed to start server";
        return -1;
    }

    server.RunUntilAskedToQuit();
    kafka_consumer_thread.join();

   
    return 0;
}
