#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char client_id[] = "microcontroller";     //arbitrary identification
const char client_key[] = "deepmusic";          //token KEY from shiftr.io
const char client_secret[] = "TumTumTum";       //token SECRET from shiftr.io

const char ssid[] = "x";     //name of the used Wi-Fi network
const char pass[] = "x";     //password of the Wi-Fi network

WiFiClient net;
MQTTClient client;
const int QoS = 1;

const int sound1_pin = D0;
//const int sound2_pin = D1;
const int dist_trigger_pin = D5;
const int dist_echo_pin = D6;

boolean sound1 = false;
boolean sound2 = false;
boolean presence = false;
int count = 0;
long duration, distance;

void setup()
{
    Serial.begin(115200);

    pinMode(sound1_pin, INPUT);
    //pinMode(sound2_pin, INPUT);
    pinMode(dist_trigger_pin, OUTPUT);
    pinMode(dist_echo_pin, INPUT);

    connectWIFI();
    client.begin("broker.shiftr.io", net);
    connectMQTT();
}

void loop()
{
    count++;
    if (count > 1000){
        verifyWIFI();
        count = 0;
    }

    if (digitalRead(sound1_pin) == HIGH)
    {        
        if (sound1 == false)
        {
            sound1 = true;
            client.publish("/sound1", "1", false, QoS);
            Serial.println("Published: /sound1 1");
        }
    }
    else if (digitalRead(sound1_pin) == LOW)
    {
        if(sound1 == true)
        {
            sound1 = false;
            client.publish("/sound1", "0", false, QoS);
            Serial.println("Published: /sound1 0");
        }
    }

    // if (digitalRead(sound2_pin) == HIGH)
    // {        
    //     if (sound2 == false)
    //     {
    //         sound2 = true;
    //         client.publish("/sound2", "1", false, QoS);
    //         Serial.println("Published: /sound2 1");
    //     }
    // }
    // else if (digitalRead(sound2_pin) == LOW)
    // {
    //     if(sound2 == true)
    //     {
    //         sound2 = false;
    //         client.publish("/sound2", "0", false, QoS);
    //         Serial.println("Published: /sound2 0");
    //     }
    // }

    
    digitalWrite(dist_trigger_pin, LOW);  
    delayMicroseconds(2); 
      
    digitalWrite(dist_trigger_pin, HIGH);
    delayMicroseconds(10); 
      
    digitalWrite(dist_trigger_pin, LOW);
    duration = pulseIn(dist_echo_pin, HIGH);
    distance = (duration/2) / 29.1;
    
    // Se detectar algo em 50 cm, liga presence
    if (distance <= 50){
        if (presence == false) {
            presence = true;
            client.publish("/presence", "1", false, QoS);
            Serial.println("Published: /presence 1");
        }
    }
    else {
        if (presence == true) {
            presence = false;
            client.publish("/presence", "0", false, QoS);
            Serial.println("Published: /presence 0");
        }
    }
}

void connectWIFI()
{
    Serial.print("Connecting Wi-Fi: ");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Wi-Fi connected!");
}

void connectMQTT()
{
    Serial.print("Connecting MQTT: ");
    while (!client.connect(client_id, client_key, client_secret))
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" MQTT connected!");
}


void verifyWIFI(){

    if (WiFi.status() != WL_CONNECTED)
    {
        connectWIFI();
    }
    client.loop();
    delay(10);  // fixes some issues with WiFi stability
    if (!client.connected())
    {
        connectMQTT();
    }
}