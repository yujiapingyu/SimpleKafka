import threading
from confluent_kafka import Consumer, KafkaError

class BaseKafkaConsumer(threading.Thread):
    def __init__(self, conf, process_message_func):
        threading.Thread.__init__(self)
        self.conf = conf
        self.topic = self.conf.pop('topic')
        self.process_message = process_message_func
        self.consumer = Consumer(self.conf)
        self.consumer.subscribe([self.topic])

    def run(self):
        try:
            while True:
                msg = self.consumer.poll(1.0)

                if msg is None:
                    continue
                if msg.error():
                    print('Consumer error: {}'.format(msg.error()))
                else:
                    self.process_message(msg.value().decode('utf-8'))
        except KeyboardInterrupt:
            pass
        finally:
            self.consumer.close()


