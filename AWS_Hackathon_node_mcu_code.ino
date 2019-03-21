unsigned long lastMillis = 0;
byte statusLed    = 13;
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266AWSIoTMQTTWS.h>
#include <Ticker.h>  //Ticker Library
Ticker blinker;
int flag = 0;
#define LED 2  //On board LED
// See `src/aws_iot_config.h` for formatting
char *region = (char* ) "xxxxxxxx";
char *endpoint = (char* ) "xxxxxxxxxxx";
char *mqttHost = (char* ) "xxxxxxxxxx-ats.iot.xxxxxx.amazonaws.com";
int mqttPort = 443;
char *iamKeyId = (char* ) "xxxxxxxxxxxxxxxxxx";
char *iamSecretKey = (char* ) "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* aws_topic  = "xxxxxxxxxxx";
/*
  #if !(ARDUINOJSON_VERSION_MAJOR == 6 and ARDUINOJSON_VERSION_MINOR == 5)
  #error "Install ArduinoJson v6.5.0-beta"
  #endif
*/
ESP8266DateTimeProvider dtp;
AwsIotSigv4 sigv4(&dtp, region, endpoint, mqttHost, mqttPort, iamKeyId, iamSecretKey);
AWSConnectionParams cp(sigv4);
AWSWebSocketClientAdapter adapter(cp);
AWSMqttClient client(adapter, cp);

byte sensorInterrupt = D7;
byte sensorPin       = D7;

char mystr[40];
unsigned long oldTime;

void ICACHE_RAM_ATTR onTimerISR() {
  digitalWrite(LED, !(digitalRead(LED))); //Toggle LED Pin
  timer1_write(1000);//12us
}

void checkStatus()
{
  digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN))); //Toggle LED Pin
  while (!client.isConnected())
  {
    int res = client.connect();
    Serial.printf("mqtt connect=%d\n", res);
    if (res == 0) {
      client.subscribe(aws_topic, 1,
                       [](const char* topic, const char* msg)
      {
        Serial.printf("Got msg '%s' on topic %s\n", msg, topic);
      }
                      );
    }
    else
    {
      Serial.println("Failed to connect!");
      Serial.println("Retry after 5 seconds");
      delay(5000);
    }

  }

  if (client.isConnected()) {
    DynamicJsonDocument jsonBuffer;
    JsonObject root = jsonBuffer.to<JsonObject>();
    root["status"] = "success";
    JsonObject data = root.createNestedObject("data");
    //JsonObject deviceid = data.createNestedObject("deviceid");
    data["deviceid"] = "A1:B2:C3:D1";
    data["flag"] = 0;
    //data["devicetype"] = "Bed";
    serializeJson(root, Serial);
    Serial.println();
    char shadow[measureJson(root) + 1];
    serializeJson(root, shadow, sizeof(shadow));

    client.publish(aws_topic, shadow, 0, false);
    client.yield();

  } else {
    Serial.println("Not connected...");
    // delay(2000);
  }


}

void SendMQ()
{
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D7, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  wifiManager.setTimeout(120);


  wifiManager.autoConnect();
  Serial.println("connected...yeey :)");
  int res = client.connect();
  Serial.printf("mqtt connect=%d\n", res);

  if (res == 0) {
    client.subscribe(aws_topic, 1,
                     [](const char* topic, const char* msg)
    {
      Serial.printf("Got msg '%s' on topic %s\n", msg, topic);
    }
                    );
  }
  //attachInterrupt(sensorInterrupt, SendMQ, RISING);
  blinker.attach(2700, checkStatus);
  //timer1_attachInterrupt(onTimerISR);
  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  //timer1_write(600000); //120000 us

}

void loop() {

  while (digitalRead(LED_BUILTIN))
  {
    while (!client.isConnected())
    {
      int res = client.connect();
      Serial.printf("mqtt connect=%d\n", res);
      if (res == 0) {
        client.subscribe(aws_topic, 1,
                         [](const char* topic, const char* msg)
        {
          Serial.printf("Got msg '%s' on topic %s\n", msg, topic);
        }
                        );
      }
      else
      {
        Serial.println("Failed to connect!");
        Serial.println("Retry after 5 seconds");
        delay(5000);
      }

    }

    if (client.isConnected()) {
      flag = 1;
      DynamicJsonDocument jsonBuffer;
      JsonObject root = jsonBuffer.to<JsonObject>();
      root["status"] = "success";
      JsonObject data = root.createNestedObject("data");
      data["deviceid"] = "A1:B2:C3:D1";
      data["flag"] = flag;
      //data["devicetype"] = "Bed";
      serializeJson(root, Serial);
      Serial.println();
      char shadow[measureJson(root) + 1];
      serializeJson(root, shadow, sizeof(shadow));

      client.publish(aws_topic, shadow, 0, false);
      client.yield();
      flag = 0;

    } else {
      Serial.println("Not connected...");
      delay(2000);
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}
