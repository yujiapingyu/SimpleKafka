import json
import redis
from .base_consumer import BaseKafkaConsumer
from config import ConsumerConfig

class RedisConsumer(BaseKafkaConsumer):
    def __init__(self, conf, stop_event):
        super().__init__(conf, self.process_message, stop_event)
        self.redis = redis.Redis(host='localhost', port=6379, db=0)
        self.buffer = []
        
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