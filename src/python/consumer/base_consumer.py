import threading
from confluent_kafka import Consumer, KafkaError

class BaseKafkaConsumer(threading.Thread):
    def __init__(self, conf, process_message_func, stop_event, start_func=None, end_func=None):
        threading.Thread.__init__(self)
        self.conf = conf
        self.topic = self.conf.pop('topic')
        self.process_message = process_message_func
        self.consumer = Consumer(self.conf)
        self.consumer.subscribe([self.topic])
        self.stop_event = stop_event
        self.start_func = start_func
        self.end_func = end_func

    def run(self):
        try:
            if self.start_func is not None:
                self.start_func()
                
            while not self.stop_event.is_set():
                msg = self.consumer.poll(1.0)
                if msg is None:
                    continue
                if msg.error():
                    print('Consumer error: {}'.format(msg.error()))
                else:
                    self.process_message(msg.value().decode('utf-8'))
                    
            if self.end_func is not None:
                self.end_func()
                
            print('Consumer [{}] stopped.'.format(self.__class__.__name__))
        except KeyboardInterrupt:
            print('KeyboardInterrupt...')
        finally:
            self.consumer.close()


