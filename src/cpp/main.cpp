#include <string>
#include <brpc/server.h>
#include <butil/logging.h>

#include "service/query_service_impl.h"
#include "proto/kv.pb.h"

int main(int argc, char* argv[]) {

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
