#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
// Replace with your network credentials
const char* ssid     = "DESKTOP-3BIVQBF 6844";
const char* password = "501,u46U";

// Create a Servo object
Servo servo;

// Servo pin
const int servoPin = 13;

// Define the stop position for the servo (you may need to adjust this)
const int SERVO_STOP = 90;

// Create an instance of the server
WebServer server(80);

void handleRoot() {
  String html = "<html><head><style>body { background-color: #303030; font-family: Arial, Helvetica, sans-serif; color: #f0f0f0; }.slider { width: 100%; }.output { width: 100%; text-align: center; margin: 25px 0; font-size: 2em; }</style><script>function setSpeed() {var speed = document.getElementById('a').value;document.getElementById('o').value = speed;var xhttp = new XMLHttpRequest();xhttp.open('GET', '/servo?speed=' + speed, true);xhttp.send();}</script></head><body><h2>ESP32 Servo Control</h2><p>Move the slider to control the servo speed:</p><form><input id='a' type='range' min='-90' max='90' value='0' class='slider' oninput='setSpeed()' onchange='setSpeed()'><output id='o'>0</output></form></body></html>";
  server.send(200, "text/html", html);
}

void handleServo() {
  String message;
  if (server.hasArg("speed")) {
    message = server.arg("speed");
    int speed = message.toInt();
    if(speed == 0)
      servo.write(SERVO_STOP); // Stop
    else if(speed > 0)
      servo.write(map(speed, 1, 90, SERVO_STOP, 180)); // Forward
    else if(speed < 0)
      servo.write(map(abs(speed), 1, 90, SERVO_STOP, 0)); // Reverse
  }
  else {
    message = "No message sent";
  }
  server.send(200, "text/plain", "Servo speed set to: " + message);
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  // attach servo
  servo.attach(servoPin);

  // Route for root / web page
  server.on("/", handleRoot);

  // send a GET request to <ESP_IP>/servo?speed=<servo_speed>
  server.on("/servo", handleServo);

  // Start the server
  server.begin();
}

void loop(){
  server.handleClient();
}