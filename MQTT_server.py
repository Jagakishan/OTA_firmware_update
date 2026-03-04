import json
import paho.mqtt.client as mqtt

BROKER= "broker.hivemq.com"
PORT=1883
DEVICE_ID="esp01s"

UPDATE_AVAILABLE= True
NEW_FIRMWARE_VERSION="1.2.0"

topic=f"device/{DEVICE_ID}/update/response"
check_topic=f"device/{DEVICE_ID}/update/check"

def on_connect(client, userdata, flags, reason_code, properties):
    print("Connected to broker")
    client.subscribe(check_topic)

def on_message(client, userdata, msg):
    print("Received message:", msg.payload.decode())

    if msg.topic==check_topic:
        message = {
            "update": UPDATE_AVAILABLE,
            "version": NEW_FIRMWARE_VERSION
        }

        client.publish(topic, json.dumps(message))
        print("Response message sent. Meaning update info published")


client=mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2)
client.on_connect=on_connect
client.on_message=on_message

client.connect(BROKER, PORT, 60)
client.loop_forever()

