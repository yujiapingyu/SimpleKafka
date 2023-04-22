import json
import threading
import time
from consumer.test_consumer import TestConsumer
from consumer.redis_consumer import RedisConsumer
from config import ConsumerConfig

def launch(config_path, consumers):
    config = ConsumerConfig(config_path)
    consumer_threads = []
    stop_event = threading.Event()
    
    # 从配置文件读取读取配置并依次启动所有的消费者线程
    for consumer in consumers:
        my_config = config.get_consumer_config(consumer)
        class_name = my_config.pop('class_name')
        print(json.dumps(my_config, indent=4))
        consumer_thread = globals()[class_name](my_config, stop_event)
        consumer_thread.start()
        print('Consumer {} started.'.format(consumer))
        consumer_threads.append(consumer_thread)
        
    try:
        while True:
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("Stopping consumer threads...")
        stop_event.set()
        
    # join所有的消费者线程
    for thread in consumer_threads:
        thread.join()

if __name__ == '__main__':
    CONF_PATH = 'consumer.conf' # 配置文件所在的路径
    consumers = [
        'test_kafka_consumer', 
        'redis_kafka_consumer'
    ] # 需要启动的消费者
    
    launch(CONF_PATH, consumers)