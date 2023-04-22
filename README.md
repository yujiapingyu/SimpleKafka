# Simple Kafka Consumer

## Description
一个简单的python Kafka消费者客户端，用于消费Kafka中的消息。
支持配置多个消费者，使用多线程方式启动多个消费者。

## Usage
### 一、配置
配置文件为consumer.conf，示例：
```
[test_kafka_consumer]
bootstrap.servers = localhost:9092
group.id = python-consumer-group
auto.offset.reset = earliest
topic = test
class_name = TestConsumer
```
其中：
1. test_kafka_consumer为消费者名字
2. bootstrap.servers为Kafka集群地址
3. group.id为消费者组名
4. auto.offset.reset为消费者启动时的offset设置，可选值为earliest和latest
5. topic为消费的topic名字
6. class_name为消费者类名，需要在consumer目录下实现，可继承BaseConsumer类

### 二、启动
```
bash start.sh
```
启动后会将pid写入procee.pid文件中，方便停止时使用。

### 三、停止
```
bash stop.sh
```
支持优雅退出，会等待所有消费者线程处理完当前消息后再退出。

