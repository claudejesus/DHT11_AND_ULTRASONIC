#include <DHT.h>
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>
#define WLAN_SSID       "CLAUDE"
#define WLAN_PASS       "12345678"
WiFiClient client;

#define DHTPIN D1

#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);


float temperature;
float humidity;
Ultrasonic ultrasonic(D3, D2);
int distance;
int ECHOPin = D3;
int TRIGPin = D2;




//const char* host = "elder.hss.rw";
const char* host ="192.168.137.1";
void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
   dht.begin();

   pinMode(ECHOPin, OUTPUT);
   pinMode(TRIGPin, INPUT);
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
void loop()
{  delay(2000);
 float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

 temperature=t;
 humidity=h;

  sendInfo();
 
  }
  void sendInfo()
   {
   Serial.print("Connecting to ");
   Serial.println( host);
   const int httpPort = 80;
   if(!client.connect(host, httpPort)){
     Serial.println("connection Failed");
     return;
   }
   String url = "/12/post_data.php?";     
   url+="&temperature=";
   url+=temperature;
    url+="&humidity=";
   url+=humidity;
   url+="&distance=";
   url+=distance;
   
  Serial.print("url: ");
  Serial.println(url);
   
         url += "temperature=233&humidity=78&distance=478";
         url += "&eng=";

         url += ""; 
   Serial.print("Requesting URL :");
  Serial.println(url);


  //code for insertion of data
client.print(String("GET /") + url + "HTTP/1.0 \r\n" +
               "HOST :" + host + "\r\n" +
               "Connection: close \r\n\r\n");
               
unsigned long timeout =millis();            
  while(client.available() == 0){
     if(millis() - timeout > 5000){
      Serial.println(">>>Client Timeout");
      client.stop();
      return;
      }

  while(client.available())
  {
     String line = client.readStringUntil('\r');
     Serial.print(line);
  }
  Serial.println();
  Serial.println("Closing Connection");
 delay(5000);
  }
    delay(3000); //3000 = 3 seconds
  Serial.println();
  distance = ultrasonic.read();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  delay(1000);
}
