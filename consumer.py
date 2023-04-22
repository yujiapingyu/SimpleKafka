from confluent_kafka import Consumer, KafkaError

conf = {
    'bootstrap.servers': 'localhost:9092',
    'group.id': 'python-consumer-group',
    'auto.offset.reset': 'earliest'
}

consumer = Consumer(conf)

consumer.subscribe(['test'])

n = 5  # 设定累积消息的阈值
messages_buffer = []

try:
    while True:
        msg = consumer.poll(1.0)

        if msg is None:
            # Received no message after a short wait
            continue
        if msg.error():
            print('Consumer error: {}'.format(msg.error()))
        else:
            messages_buffer.append(msg.value().decode('utf-8'))

            # 如果累积消息达到阈值n，则开始处理消息
            if len(messages_buffer) >= n:
                for message in messages_buffer:
                    print('Received message: {}'.format(message))
                # 清空消息缓冲区
                messages_buffer.clear()
except KeyboardInterrupt:
    pass
finally:
    # Close down consumer to commit final offsets.
    consumer.close()
