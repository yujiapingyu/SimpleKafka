import json
import redis
import pickle
import os

from .base_consumer import BaseKafkaConsumer
from config import ConsumerConfig

class RedisConsumer(BaseKafkaConsumer):
    def __init__(self, conf, stop_event):
        super().__init__(conf, self.process_message, stop_event, self.read_buffer_from_file, self.save_buffer_to_file)
        self.redis = redis.Redis(host='localhost', port=6379, db=0)
        self.buffer = []
        self.pickle_file = 'buffer.pickle'
        
    def save_buffer_to_file(self):
        with open(self.pickle_file, 'wb') as f:
            pickle.dump(self.buffer, f)
        print('saved buffer to file.[{}]'.format(self.pickle_file))
        
    def read_buffer_from_file(self):
        if not os.path.exists(self.pickle_file):
            print('pickle file not exists.[{}]'.format(self.pickle_file))
            return
        with open(self.pickle_file, "rb") as file:
            self.buffer = pickle.load(file)
        print('read buffer from file.[{}]'.format(self.pickle_file))
        # 删除文件
        os.remove(self.pickle_file)
    
    def flush(self):
        print('Flushing buffer: {}'.format(self.buffer))
        for obj in self.buffer:
            self.redis.set(obj['key'], obj['value'])
        self.buffer = []

    def process_message(self, message):
        # print('{}: Received message: {}'.format(self.__class__.__name__, message))
        obj = json.loads(message)
        self.buffer.append(obj)
        if len(self.buffer) >= 5:
            self.flush()
        
if __name__ == '__main__':
    config = ConsumerConfig('consumer.conf')
    my_config = config.get_consumer_config('test_kafka_consumer')
    print(json.dumps(my_config, indent=4))
    consumer_thread = RedisConsumer(my_config)
    consumer_thread.start()
    consumer_thread.join()