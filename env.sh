# 启动zookeeper
docker run -d --name zookeeper \
    --restart unless-stopped \
    --network dev-network \
    -p 2181:2181 \
    -e ZOOKEEPER_CLIENT_PORT=2181 \
    confluentinc/cp-zookeeper:latest

# 启动kafka
docker run -d --name kafka \
    --restart unless-stopped \
    --network dev-network  \
    -p 9092:9092 \
    -e KAFKA_ZOOKEEPER_CONNECT=zookeeper:2181 \
    -e KAFKA_ADVERTISED_LISTENERS=PLAINTEXT://kafka:9092 \
    -e KAFKA_LISTENERS=PLAINTEXT://:9092 \
    -e KAFKA_OFFSETS_TOPIC_REPLICATION_FACTOR=1 \
    confluentinc/cp-kafka:5.0.0

# 创建一个test的topic，注意需要等到kafka启动完成后才能创建topic
sleep 30
docker exec -it kafka kafka-topics --create --topic test --partitions 1 --replication-factor 1 --if-not-exists --zookeeper zookeeper:2181

# 启动redis
docker run -d --name my-redis \
    --restart unless-stopped \
    --network dev-network  \
    -p 6379:6379 \
    redis:latest
