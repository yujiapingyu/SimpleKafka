#include <iostream>
#include <thread>
#include <csignal>
#include <glog/logging.h>
#include <cppkafka/cppkafka.h>

using std::string;
using std::thread;
using std::atomic_bool;

using cppkafka::Configuration;
using cppkafka::Consumer;
using cppkafka::Message;

atomic_bool running(true);

void signal_handler(int) {
    running = false;
}

class KafkaConsumerThread {
public:
    KafkaConsumerThread(const Configuration& config, const string& topic)
    : consumer_(config) {
        consumer_.subscribe({topic});
        consumer_thread_ = thread(&KafkaConsumerThread::consume, this);
    }

    ~KafkaConsumerThread() {
        LOG(INFO) << "Stopping consumer thread...";
        consumer_.pause();
        consumer_thread_.join();
        LOG(INFO) << "Consumer thread stopped";
    }

    void consume() {
        LOG(INFO) << "Starting consumer thread...";
        while (running) {
            auto msg = consumer_.poll();
            if (msg) {
                if (msg.get_error()) {
                    LOG(ERROR) << "Error while consuming: " << msg.get_error();
                }
                else {
                    LOG(INFO) << "Received message: " << string(msg.get_payload());
                }
            }
        }
    }
private:
    Consumer consumer_;
    thread consumer_thread_;
};

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    signal(SIGINT, signal_handler);

    Configuration config = {
        { "bootstrap.servers", "kafka:9092" },
        { "group.id", "cpp-consumer-group" },
        { "auto.offset.reset", "earliest" }
    };

    string topic = "test";
    KafkaConsumerThread consumer_thread(config, topic);

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


    return 0;
}
