#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Set DS18B20 on pin 13 (marked as D7 pin on the board) 
#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

MDNSResponder mdns;

// Wi-Fi point settings
const char* ssid = "AirPort-2";
const char* password = "1803200820082008";

byte arduino_mac[] = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };

//Set the settings for your local network
IPAddress ip(192,168,100,40);
IPAddress gateway(192,168,100,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int D6_pin = 12;   //D6 on the board
int D5_pin = 14;   //D5 on the board

int k = 0;
float temperature = 0;
unsigned long previousMillis = 0;

String D5_status;
String D6_status;
String web;


void setup(void){
// Setup GPIOs
  pinMode(D5_pin, OUTPUT);
  digitalWrite(D5_pin, LOW);
  pinMode(D6_pin, OUTPUT);
  digitalWrite(D6_pin, LOW);
  
// Start working with ds18b20
  sensors.begin();

  delay(100);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  
 
  Serial.println("");

// Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

//------ OUT1 control endpoint ------
  server.on("/led1",process_D5);
//------ OUT2 control endpoint ------
  server.on("/led2",process_D6);

  server.on("/", [](){
    server.send(200, "text/html", webPage());
  });

//----- DATA endpoint ----
  server.on("/data", [](){     
//----- LED1 START -----     
     if (digitalRead(D5_pin) == HIGH){
         D5_status = "ON";
         }
     else{
         D5_status = "OFF";
//----- LED1 END ----- 
         
//----- LED2 START -----
    }
    if (digitalRead(D6_pin) == HIGH){
         D6_status = "ON";
         }
     else{
         D6_status = "OFF";
    }
//----- LED2 END -----

//Join all data in a string like-->> "28.09,ON,OFF"
    String data = String(temperature)+','+ D5_status+','+ D6_status ;
    server.send(200, "text/html", data);
  });
  
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();

// Get data from ds18b20 with 10 seconds interval 
  if (millis() - previousMillis >= 10000) 
  {
      previousMillis = millis();
      sensors.requestTemperatures();
      temperature = sensors.getTempCByIndex(0);
  }
      
} 

void process_D5() {
  int pin_value = server.arg("set").toInt();
   digitalWrite(D5_pin, pin_value);
   server.send(200, "text/html", "Done" );
}

void process_D6() {
  int pin_value = server.arg("set").toInt();
   digitalWrite(D6_pin, pin_value);
   server.send(200, "text/html", "Done" );
}

String webPage()
{
web =
"<!DOCTYPE html>"
"<html>"
" <head>"
" <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>"
" <meta charset=\"utf-8\">"
" <title>ESP 8266</title>"
" <style>button{color:red;padding: 10px 27px;}</style>"
"</head>"
"<body>"
" <h1 style=\"text-align: center;font-family: Open sans;font-weight: 100;font-size: 20px;\">ESP8266 Web Server</h1>"
// ----- LED1 START -----
" <p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----LED 1----</p>"
" <div id=\"led_1\" style=\"text-align: center;width: 98px;color:white; padding: 10px 30px; background-color:#cbcbcb; margin: 0 auto;\">loading...</div>"
"  <div style=\"text-align: center;margin: 5px 0px;\">"
"   <button onclick=\"httpGet('led1?set=1', null)\">ON</button>"
"   <button onclick=\"httpGet('led1?set=0', null)\">OFF</button></div>"
// ----- LED1 END -----

// ----- LED2 START -----
" <p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">----LED 2----</p>"
" <div id=\"led_2\" style=\"text-align: center;width: 98px;color:white; padding: 10px 30px; background-color:#cbcbcb; margin: 0 auto;\">loading...</div>"
"  <div style=\"text-align: center;margin: 5px 0px;\">"
"   <button onclick=\"httpGet('led2?set=1', null)\">ON</button>"
"   <button onclick=\"httpGet('led2?set=0', null)\">OFF</button></div>"
// ----- LED2 END -----

// ----- TEMPERATURE START -----
"  <div>"
"  <p style=\"text-align: center;margin-top: 0px;margin-bottom: 5px;\">---Temperature---</p>"
"  <div id=\"temperature\" style=\"text-align: center;width:98px; color:white; padding: 10px 30px; background-color:#02a2ea; margin: 0 auto;\">0</div>"
" </div>"
// ----- TEMPERATURE END -----
"<body>"
"</html>"
"<script defer>"
// Set data refresh interval in millicesonds (1000 = 1 second)
" setInterval(loadData,1000);"
" function loadData(){"
"  let data = httpGet(\"data\").split(\",\");"

// ----- TEMPERATURE JS CODE START -----
"  document.getElementById(\"temperature\").innerHTML = data[0];"
// ----- TEMPERATURE JS CODE END -----

// ----- LED1 JS CODE START -----
"  document.getElementById(\"led_1\").innerHTML = data[1];"
"  if (data[1] === \"ON\"){"
"   document.getElementById(\"led_1\").style.backgroundColor = \"#43a209\";"
"  }" 
"  else if (data[1] === \"OFF\"){"
"   document.getElementById(\"led_1\").style.backgroundColor = \"#ec1212\";"
"  }"
// ----- LED1 JS CODE END -----

// ----- LED2 JS CODE START -----
"  document.getElementById(\"led_2\").innerHTML = data[2];"
" if (data[2] === \"ON\"){"
"   document.getElementById(\"led_2\").style.backgroundColor = \"#43a209\";"
"  }" 
"  else if (data[2] === \"OFF\"){"
"   document.getElementById(\"led_2\").style.backgroundColor = \"#ec1212\";"
"  }"
// ----- LED2 JS CODE END -----

" }"
" function httpGet(theUrl){"
"  var xmlHttp = new XMLHttpRequest();"
"  xmlHttp.open( \"GET\", theUrl, false );"
"  xmlHttp.send( null );"
"  return xmlHttp.responseText;"
" }"
"</script>";  
  return(web);
}
