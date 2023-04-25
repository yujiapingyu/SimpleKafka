#include <string>
#include <brpc/server.h>
#include <butil/logging.h>
#include <gflags/gflags.h>

#include "service/query_service_impl.h"
#include "service/config_manager.h"
#include "proto/kv.pb.h"

DEFINE_string(config_file, "config.json", "Path to the configuration file");

int main(int argc, char* argv[]) {

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // 加载配置文件，如果失败则退出
    try {
        ConfigManager::get_instance().load_config(FLAGS_config_file);
    } catch (const std::exception& e) {
        LOG(ERROR) << "Failed to load config: " << e.what();
        return -1;
    }

    simple_kafka::QueryServiceImpl query_service_impl;
    brpc::Server server;

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
    query_service_impl.stop_consumer_thread();
    
   
    return 0;
}
