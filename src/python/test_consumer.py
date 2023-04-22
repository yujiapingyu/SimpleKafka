import json
from base_consumer import BaseKafkaConsumer
from config import ConsumerConfig

class TestConsumer(BaseKafkaConsumer):
    def __init__(self, conf):
        super().__init__(conf, self.process_message)

    def process_message(self, message):
        print('Received message: {}'.format(message))
        
if __name__ == '__main__':
    config = ConsumerConfig('consumer.conf')
    my_config = config.get_consumer_config('test_kafka_consumer')
    print(json.dumps(my_config, indent=4))
    consumer_thread = TestConsumer(my_config)
    consumer_thread.start()
    consumer_thread.join()