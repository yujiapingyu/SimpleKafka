import json
from test_consumer import TestConsumer
from config import ConsumerConfig

def launch(config_path, consumers):
    config = ConsumerConfig(config_path)
    consumer_threads = []
    for consumer in consumers:
        my_config = config.get_consumer_config(consumer)
        print(json.dumps(my_config, indent=4))
        consumer_thread = TestConsumer(my_config)
        consumer_thread.start()
        print('Consumer {} started.'.format(consumer))
        consumer_threads.append(consumer_thread)
        
    # join所有的消费者线程
    for thread in consumer_threads:
        thread.join()

if __name__ == '__main__':
    CONF_PATH = 'consumer.conf' # 配置文件所在的路径
    consumers = ['test_kafka_consumer'] # 需要启动的消费者
    
    launch(CONF_PATH, consumers)