import json
import threading
import time
import signal

from consumer.test_consumer import TestConsumer
from consumer.redis_consumer import RedisConsumer
from config import ConsumerConfig

stop_event = threading.Event()

def exit(signum, frame):
    print('receive signal {}, exit...'.format(signum))
    stop_event.set()
    
# 注册信号处理函数
signal.signal(signal.SIGINT, exit)
signal.signal(signal.SIGTERM, exit)

def launch(config_path, consumers):
    config = ConsumerConfig(config_path)
    consumer_threads = []

    # 从配置文件读取读取配置并依次启动所有的消费者线程
    for consumer in consumers:
        my_config = config.get_consumer_config(consumer)
        class_name = my_config.pop('class_name')
        print(json.dumps(my_config, indent=4))
        consumer_thread = globals()[class_name](my_config, stop_event)
        consumer_thread.start()
        print('Consumer {} started.'.format(consumer))
        consumer_threads.append(consumer_thread)
           
    # 等待所有消费者线程结束
    for thread in consumer_threads:
        thread.join()

if __name__ == '__main__':
    CONF_PATH = 'consumer.conf' # 配置文件所在的路径
    consumers = [
        'test_kafka_consumer', 
        'redis_kafka_consumer'
    ] # 需要启动的消费者
    
    launch(CONF_PATH, consumers)