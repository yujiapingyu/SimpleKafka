import datetime
from confluent_kafka import Producer

conf = {'bootstrap.servers': 'localhost:9092'}

producer = Producer(conf)

def delivery_report(err, msg):
    if err is not None:
        print('Message delivery failed: {}'.format(err))
    else:
        print('Message delivered to {} [{}]'.format(msg.topic(), msg.partition()))

message = 'Hello, Kafka from Python! Current time is: {}'.format(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

producer.produce('test', key=None, value=message, callback=delivery_report)

# Wait for any outstanding messages to be delivered and delivery reports to be received.
producer.flush()
