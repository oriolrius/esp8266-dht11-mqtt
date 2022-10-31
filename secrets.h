#define PUBLISH_TOPIC   "esp8266/group20/env"
#define SUBSCRIBE_TOPIC "esp8266/group20/msg"

#define FREQ 10000 

const char WIFI_SSID[] = "AP";
const char WIFI_PASSWORD[] = "lavit2ap_";
 
#define THINGNAME "group20"
 
#define TIME_ZONE "CET-1CEST"

//const char MQTT_HOST[] = "10.2.0.122"; // broker.hivemq.com
const char MQTT_HOST[] = "broker.hivemq.com";
//const uint8_t MQTT_HOST[] = {10, 2, 0, 123};
const int MQTT_PORT = 1883;
//const char MQTT_USER[] = "miimetiq/idm/devices/esp8266/myESP";
//const char MQTT_PASSWORD[] = "abcd1234";
