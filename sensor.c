#include <WiFi.h>
#include <WebServer.h>
#include "html.h"

WebServer server(80);

int _moisture,sensor_analog;
const int sensor_pin = A0;  /* Soil moisture sensor O/P pin */

const char* ssid = "*Your SSID*";         /*Enter Your SSID*/
const char* password = "*Your Password*"; /*Enter Your Password*/
 
void MainPage() {
  String _html_page = html_page;              /*Read The HTML Page*/
  server.send(200, "text/html", _html_page);  /*Send the code to the web server*/
}

void SoilMoisture() {
  String MoistuValue = String(_moisture);    //Convert it into string
  server.send(200, "text/plane", MoistuValue);  //Send updated temperature value to the web server
}

void setup(void){
  Serial.begin(115200);               /*Set the baudrate to 115200*/
  WiFi.mode(WIFI_STA);                /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
                          /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());     /*Print the Local IP*/

  server.on("/", MainPage);           /*Display the Web/HTML Page*/
  server.on("/readMoisture", SoilMoisture);       /*Display the updated Temperature and Humidity value*/
  server.begin();                    /*Start Server*/
  delay(1000);                       /*Wait for 1000mS*/
}

void loop(void){
  sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  server.handleClient(); 
  Serial.print("Moisture = ");
  Serial.print(_moisture);           /* Print Temperature on the serial window */
  Serial.println("%");
  delay(1000);                          /* Wait for 1000mS */
}