import json
import random
import string

port_start = 1025
port_end = 2048

def random_string(length):
    return ''.join(random.choice(string.ascii_uppercase) for i in range(length))

config = {}
config['server'] = '45.63.124.180'
config['port_password'] = {}
config['local_address'] = '127.0.0.1'
config['local_port'] = 9999
config['fast_open'] = True
config['timeout'] = 600
config['method'] = 'aes-256-cfb'


for i in xrange(0, 500):
    config['port_password'][random.randint(port_start,
                                           port_end)] = random_string(5)

# config['server_port'] =  config['port_password'].keys()

fout = open('config.json', 'w')
json.dump(config,  fout, indent = 4)
