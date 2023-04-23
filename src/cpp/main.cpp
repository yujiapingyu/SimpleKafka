#include <iostream>
#include <thread>
#include <csignal>
#include <cppkafka/cppkafka.h>

using std::string;
using std::cout;
using std::endl;
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
        consumer_.pause();
        consumer_thread_.join();
    }

    void consume() {
        while (running) {
            auto msg = consumer_.poll();
            if (msg) {
                if (msg.get_error()) {
                    cout << "Error while consuming: " << msg.get_error() << endl;
                }
                else {
                    cout << "Received message: " << string(msg.get_payload()) << endl;
                }
            }
        }
    }
private:
    Consumer consumer_;
    thread consumer_thread_;
};

int main() {
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
