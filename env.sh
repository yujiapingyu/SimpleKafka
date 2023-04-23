# 启动zookeeper
docker run -d --name zookeeper \
    --network dev-network \
    -p 2181:2181 \
    -e ZOOKEEPER_CLIENT_PORT=2181 \
    confluentinc/cp-zookeeper:latest

# 启动kafka
docker run -d --name kafka \
    --network dev-network  \
    -p 9092:9092 \
    --link zookeeper:zookeeper \
    -e KAFKA_ZOOKEEPER_CONNECT=zookeeper:2181 \
    -e KAFKA_ADVERTISED_LISTENERS=PLAINTEXT://localhost:9092 \
    -e KAFKA_OFFSETS_TOPIC_REPLICATION_FACTOR=1 \
    confluentinc/cp-kafka:5.0.0

# 启动redis
docker run -d --name my-redis \
    -p 6379:6379 \
    redis:latest
