import paho.mqtt.client as mqtt
import json
import base64

# ---------------- Configuración TTN ----------------
broker = "eu1.cloud.thethings.network"
port = 1883  # Usa 8883 si configuras TLS

# Sustituye estos valores con los tuyos
app_id = "lorawan-app-iot"       # ID de la aplicación en TTN
api_key = "NNSXS.ADTRC6JMH5SKLJR6XB67GTS6PI6JKNKDLFG532Y.GBU5ZMWXR4FLWGN5VI4JOML7YASOQSMRADZCB77JYULDPBCGKJQQ"      # La API Key generada en TTN

# IDs de los dispositivos
device_id_emitter = "cubecell-1"    # Emisor
device_id_receiver = "cubecell-2"   # Receptor

# Tópicos MQTT (TTN v3)
uplink_topic = f"v3/{app_id}@ttn/devices/{device_id_emitter}/up"
downlink_topic = f"v3/{app_id}@ttn/devices/{device_id_receiver}/down/push"

# ---------------- Callbacks ----------------
def on_connect(client, userdata, flags, rc):
    print("Conectado al broker MQTT con código de resultado:", rc)
    # Suscribirse al tópico uplink del emisor para monitoreo
    client.subscribe(uplink_topic)

def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}:")
    print(msg.payload.decode())
    
    # Extraer el mensaje del emisor y preparar el mismo texto para el receptor
    # Suponiendo que el payload sea un JSON con campo "frm_payload" en Base64
    try:
        payload = json.loads(msg.payload.decode())
        # Por ejemplo, extraemos el frm_payload (si lo tiene)
        frm_payload_b64 = payload.get("frm_payload", "")
    except Exception as e:
        print("Error al decodificar el JSON:", e)
        frm_payload_b64 = ""
        
    # Si no se obtiene payload o prefieres usar un mensaje fijo, puedes asignarlo:
    if not frm_payload_b64:
        # Si el mensaje es "Hello", su Base64 es "SGVsbG8="
        frm_payload_b64 = "SGVsbG8="
    
    # Construir el mensaje de downlink para el receptor
    downlink_message = {
        "downlinks": [
            {
                "f_port": 2,
                "frm_payload": frm_payload_b64,
                "priority": "NORMAL"
            }
        ]
    }
    json_message = json.dumps(downlink_message)
    # Publicar el downlink
    client.publish(downlink_topic, json_message)
    print("Downlink enviado:", json_message)

# ---------------- Configuración del Cliente MQTT ----------------
client = mqtt.Client()
client.username_pw_set(username=f"{app_id}@ttn", password=api_key)
client.on_connect = on_connect
client.on_message = on_message

print("Conectando al broker MQTT...")
client.connect(broker, port, 60)
client.loop_forever()
