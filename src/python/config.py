import configparser

class ConsumerConfig(object):
    def __init__(self, config_path):
        self.config_path = config_path
        self.config = configparser.ConfigParser()
        self.config.read("consumer.conf")
        
    def get_consumer_config(self, name):
        return {key: value for key, value in self.config.items(name)}