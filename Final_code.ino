
#include <ESP8266WiFi.h>

const int M1_I1 = 5;//d1
const int M1_I2 = 4;//d2
const int M2_I3 = 2;//d4
const int M2_I4 = 14;//d5
const int trig = 12;//d6
const int echo = 13;//d7
float D;

float Detect() {
  float obstacleDetect, obstacleDist;
  digitalWrite(trig, LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  obstacleDetect = pulseIn(echo, HIGH);
  obstacleDist = obstacleDetect * (0.034 / 2);
  return obstacleDist;
}

void tacle() {
  int subDist = 0;
  int addDist = 0;
  int countDist = 0;
subRoute_1: 
  subDist++;
  if (subDist >= 10) {
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    Serial.println("Ooops! Large size obstacle!!");
    loop();
  }
  digitalWrite(M1_I1, LOW); //Left
  digitalWrite(M1_I2, HIGH);
  digitalWrite(M2_I3, HIGH);
  digitalWrite(M2_I4, LOW);
  delay(200);
  digitalWrite(M1_I1, HIGH); //Forward
  digitalWrite(M1_I2, LOW);
  digitalWrite(M2_I3, HIGH);
  digitalWrite(M2_I4, LOW);
  delay(1000);
  digitalWrite(M1_I1, HIGH); //Right
  digitalWrite(M1_I2, LOW);
  digitalWrite(M2_I3, LOW);
  digitalWrite(M2_I4, HIGH);
  delay(200);
  D = Detect();
  if (D < 20) {
    D = 0;
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    D = Detect();
    if (D < 20) {
      Serial.println("Ooops!I'm blocked!!!");
      delay(500);
      loop();
    }
  }
  else {
    goto addRoute;
  }
addRoute: 
  addDist++;
  if (addDist >= 10) {
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
    Serial.println("Ooops! Large size obstacle!!");
    delay(1000);
    loop();
  }
  digitalWrite(M1_I1, HIGH); //Forward
  digitalWrite(M1_I2, LOW);
  digitalWrite(M2_I3, HIGH);
  digitalWrite(M2_I4, LOW);
  delay(1000);
  digitalWrite(M1_I1, HIGH); //Right
  digitalWrite(M1_I2, LOW);
  digitalWrite(M2_I3, LOW);
  digitalWrite(M2_I4, HIGH);
  delay(200);
  D = Detect();
  if (D < 20) {
    D = 0;
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    digitalWrite(M1_I1, LOW); //Left
    digitalWrite(M1_I2, HIGH);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    delay(200);
    D = Detect();
    if (D < 20) {
      D = 0;
      D = Detect();
      if (D < 20) {
        D = 0;
        goto subRoute_1;
      }
    }
    else {
      goto addRoute;
    }
  }
  else {
    goto subRoute_2;
  }
subRoute_2: 
    while (subDist != 0) {
    subDist--;
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    D = Detect();
    if (D < 20) {
      D = 0;
      digitalWrite(M1_I1, LOW); //Stop
      digitalWrite(M1_I2, LOW);
      digitalWrite(M2_I3, LOW);
      digitalWrite(M2_I4, LOW);
      delay(1000);
      digitalWrite(M1_I1, LOW); //Left
      digitalWrite(M1_I2, HIGH);
      digitalWrite(M2_I3, HIGH);
      digitalWrite(M2_I4, LOW);
      delay(200);
      D = Detect();
      if (D < 20) {
        D = 0;
        goto subRoute_1;
      }
      else {
        goto addRoute;
      }
    }
  }
  digitalWrite(M1_I1, LOW); //Left
  digitalWrite(M1_I2, HIGH);
  digitalWrite(M2_I3, HIGH);
  digitalWrite(M2_I4, LOW);
}

void travelDist() {
  float T = 0;
  while (T == 0) {
    delay(1000);
    D = Detect();
    if (D < 20) {
      tacle();
      D = 0;
    }
  }
}

WiFiClient client;
WiFiServer server(80);
const char* ssid = "RATAN TATA"; //YOUR SSID
const char* password = "rushirushi";//YOUR PASSWORD

void connectWiFi() {
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(500);
    Serial.print(" . ");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));
}

// check command received from Android Device
String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}

// send command echo back to android device
/*void sendBackEcho(String echo)
  {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println(echo);
  client.println("</html>");
  client.stop(); // closing connection to server
  delay(1);
  }*/

String command = "";

void setup() {
  Serial.begin(115200);
  pinMode(M1_I1, OUTPUT);
  pinMode(M1_I2, OUTPUT);
  pinMode(M2_I3, OUTPUT);
  pinMode(M2_I4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  connectWiFi();
  server.begin();
}

void loop() {
  client = server.available();
  if (!client) return;
  command = checkClient();
  Serial.println(command);
  if (command == "F" ) {
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
  }
  else if (command == "B") {
    digitalWrite(M1_I1, LOW); //Backward
    digitalWrite(M1_I2, HIGH);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, HIGH);
  }
  else if (command == "L") {
    digitalWrite(M1_I1, LOW); //Left
    digitalWrite(M1_I2, HIGH);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
  }
  else if (command == "R") {
    digitalWrite(M1_I1, HIGH); //Right
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, HIGH);
    delay(1000);
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
  }
  else if (command == "S") {
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
  }
  else if (command == "forward" ) {
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    travelDist();
  }
  else if (command == "backward") {
    digitalWrite(M1_I1, LOW); //Backward
    digitalWrite(M1_I2, HIGH);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, HIGH);
    travelDist();
  }
  else if (command == "left") {
    digitalWrite(M1_I1, LOW); //Left
    digitalWrite(M1_I2, HIGH);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    delay(1000);
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    travelDist();
  }
  else if (command == "right") {
    digitalWrite(M1_I1, HIGH); //Right
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, HIGH);
    delay(1000);
    digitalWrite(M1_I1, HIGH); //Forward
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, HIGH);
    digitalWrite(M2_I4, LOW);
    travelDist();
  }
  else if (command == "stop") {
    digitalWrite(M1_I1, LOW); //Stop
    digitalWrite(M1_I2, LOW);
    digitalWrite(M2_I3, LOW);
    digitalWrite(M2_I4, LOW);
    travelDist();
  }
  //sendBackEcho(command);
  command = "";
}
