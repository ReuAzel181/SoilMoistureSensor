#include <WiFi.h>
#include <WebServer.h>

WebServer server(80); // Creates a web server on port 80

int _moisture, sensor_analog; // Variables for moisture reading
const int sensor_pin = A0; // Soil moisture sensor pin

const char* ssid = "Jan Pol Uzi"; // Replace with your Wi-Fi SSID
const char* password = "12345678"; // Replace with your Wi-Fi password

const char html_page[] PROGMEM = R"rawSrting(
<!DOCTYPE html>
<html>
<style>
    body {font-family: sans-serif;}
    h1 {text-align: center; font-size: 30px;}
    p {text-align: center; color: #4CAF50; font-size: 40px;}
</style>
 
<body>
  <h1>Soil Moisture With ESP32</h1><br>
  <p>Moisture Level :<span id="MoistureVal">0</span>%</p><br>

  <script>
    setInterval(function() { 
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
         document.getElementById("MoistureVal").innerHTML = this.responseText
        }
      };
      xhttp.open("GET", "readMoisture", true);
      xhttp.send();
    },50);
  </script>
</body>
</html>
)rawSrting";

void MainPage() { // Function to send the HTML page
  server.send(200, "text/html", html_page);
}

void SoilMoisture() { // Function to send the moisture value
  String MoistuValue = String(_moisture);
  server.send(200, "text/plain", MoistuValue);
}

void setup() {
  Serial.begin(115200); // Open serial communication with baud rate 115200
  WiFi.mode(WIFI_STA); // Set Wi-Fi mode to Station (client)
  WiFi.begin(ssid, password); // Connect to the Wi-Fi network

  while (WiFi.waitForConnectResult() != WL_CONNECTED) { // Wait for Wi-Fi connection
    Serial.print("."); // Print a dot for each attempt
    delay(100);
  }

  Serial.println(); // Print a new line after connection
  Serial.print("Connected to "); // Print connection message
  Serial.print(ssid);
  Serial.print(" with IP address: ");

  // Print the ESP32's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.println(ip[3]);

  server.on("/", MainPage); // Set function to handle requests on root path "/"
  server.on("/readMoisture", SoilMoisture); // Set function to handle requests on "/readMoisture" path
  server.begin(); // Start the web server
  delay(1000); // Wait for a second
}

void loop() {
  sensor_analog = analogRead(sensor_pin); // Read sensor value
  _moisture = (100 - ((sensor_analog / 4095.00) * 100)); // Calculate moisture percentage

  server.handleClient(); // Handle any incoming web requests

  Serial.print("Moisture = "); // Print moisture value on serial monitor
  Serial.print(_moisture);
  Serial.println("%");

  delay(1000); 
}
