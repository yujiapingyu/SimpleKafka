#include <iostream>
#include <thread>
#include <csignal>
#include <string>
#include <mutex>
#include <brpc/server.h>
#include <butil/logging.h>
#include <cppkafka/cppkafka.h>
#include <nlohmann/json.hpp>

#include "query_service_impl.h"
#include "config_manager.h"
#include "proto/kv.pb.h"

using json = nlohmann::json;

using cppkafka::Configuration;
using cppkafka::Consumer;
using cppkafka::Message;

namespace simple_kafka {

QueryServiceImpl::QueryServiceImpl() {
    start_consumer_thread();
}

void QueryServiceImpl::GetValue(google::protobuf::RpcController *cntl_base,
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

void QueryServiceImpl::consume() {
    LOG(INFO) << "Starting consumer thread...";
    auto config = ConfigManager::get_instance().get_config();
    Configuration kafka_config = {
        { "bootstrap.servers", config["kafka"]["bootstrap.servers"].get<std::string>() },
        { "group.id", config["kafka"]["group.id"].get<std::string>() },
        { "auto.offset.reset", config["kafka"]["auto.offset.reset"].get<std::string>() }
    };
    Consumer consumer(kafka_config);
    std::string topic = "test";
    consumer.subscribe({ topic });
    while (running) {
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
    LOG(INFO) << "Stopping consumer thread...";
}

void QueryServiceImpl::start_consumer_thread() {
    kafka_consumer_thread = std::thread(&QueryServiceImpl::consume, this);
}

void QueryServiceImpl::stop_consumer_thread() {
    running = false;
    if (kafka_consumer_thread.joinable()) {
        kafka_consumer_thread.join();
    }
}

} // end of namespace simple_kafka