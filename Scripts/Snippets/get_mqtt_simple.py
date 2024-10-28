import paho.mqtt.client as mqtt

broker = "PLACEHOLDER"
port = 1883
username = "PLACEHOLDER"
password = "PLACEHOLDER"
topic = "projector/1"

def on_connect(client, userdata, flags, rc, properties):
  if rc == 0:
    print("Connected to MQTT Broker")
    client.subscribe("projector/out/1")
  else:
    print("Failed to connect, return code %d\n", rc);

def on_message(client, userdata, msg):
  print("got message");
  print(msg.payload.decode());
  client.disconnect()
  client.loop_stop()
  # only once, for testing

def init_client():
  client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
  client.username_pw_set(username, password)
  client.on_connect = on_connect
  client.on_message = on_message
  client.connect(broker, port)
  return client

client = init_client()
client.loop_start()
