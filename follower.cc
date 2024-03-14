#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define IR_SENSOR_INPUT_LEFT 15
#define IR_SENSOR_INPUT_RIGHT 4
#define DUTY 50
#define RIGHT_MOTOR_PIN 13
#define LEFT_MOTOR_PIN 12
#define FILL 1000
#define Kp 1
#define Kd 1
#define Ki 1

#define LOW_SPEED 255
#define HIGH_SPEED 80    // 0 bad battery
#define CRUISE_SPEED 120 // 30
#define LINE_THRESHOLD 4000

enum state
{
    FORWARD,
    LEFT,
    RIGHT,
    STOP
};
int bufLen = 150;
int bufferCounter = 0;
state lastState = FORWARD;

int counter = 0;

const char *ssid = "";     // Replace with your Wi-Fi SSID
const char *password = ""; // Replace with your Wi-Fi password

float last_error = 0;
float integral = 0;
char buf[64];

void setup()
{
    Serial.begin(9600);
    pinMode(IR_SENSOR_INPUT_LEFT, INPUT);  // Sensor output
    pinMode(IR_SENSOR_INPUT_RIGHT, INPUT); // Sensor output
    pinMode(LEFT_MOTOR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_PIN, OUTPUT);

    // WiFi.begin(ssid, password);
    // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //   Serial.println("Connection Failed! Rebooting...");
    //   delay(5000);
    //   ESP.restart();
    // }
    // // Initialize OTA
    // ArduinoOTA.begin();
    // ArduinoOTA.setHostname("esp32-ota-example");
    // // ArduinoOTA.setPassword("YourOTAPassword"); // Replace with your OTA password

    // Serial.println("Ready");
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());
}

void PID_pseudo(float leftSensor, float rightSensor)
{
    if ((leftSensor > LINE_THRESHOLD) && (rightSensor > LINE_THRESHOLD))
    {
        // symmetric forward
        analogWrite(LEFT_MOTOR_PIN, HIGH_SPEED);
        analogWrite(RIGHT_MOTOR_PIN, HIGH_SPEED);
        Serial.println("FORWARD");
        lastState = FORWARD;
        bufferCounter = 0;
    }
    else if ((leftSensor > LINE_THRESHOLD) && (rightSensor < LINE_THRESHOLD))
    {
        // left is on line, right is outside
        // turn left
        analogWrite(LEFT_MOTOR_PIN, LOW_SPEED);
        analogWrite(RIGHT_MOTOR_PIN, CRUISE_SPEED);
        Serial.println("LEFT");
        lastState = LEFT;
        bufferCounter = 0;
    }
    else if ((leftSensor < LINE_THRESHOLD) && (rightSensor > LINE_THRESHOLD))
    {
        // right is on line, left is outside
        // turn right
        analogWrite(LEFT_MOTOR_PIN, CRUISE_SPEED);
        analogWrite(RIGHT_MOTOR_PIN, LOW_SPEED);
        Serial.println("RIGHT");
        lastState = RIGHT;
        bufferCounter = 0;
    }
    else
    {
        // stop
        // analogWrite(LEFT_MOTOR_PIN, 255);
        // analogWrite(RIGHT_MOTOR_PIN, 255);
        if (bufferCounter >= bufLen)
        {
            analogWrite(LEFT_MOTOR_PIN, 255);
            analogWrite(RIGHT_MOTOR_PIN, 255);
            Serial.println("STOP");
        }
        else if (lastState == RIGHT)
        {
            // repeat last state
            analogWrite(LEFT_MOTOR_PIN, CRUISE_SPEED);
            analogWrite(RIGHT_MOTOR_PIN, LOW_SPEED);
            Serial.println("CORRECT RIGHT");
            bufferCounter++;
        }
        else if (lastState == LEFT)
        {
            analogWrite(LEFT_MOTOR_PIN, LOW_SPEED);
            analogWrite(RIGHT_MOTOR_PIN, CRUISE_SPEED);
            Serial.println("CORRECT LEFT");
            bufferCounter++;
        }
    }
}

void PID_pseudo_single(float leftSensor)
{
    if (leftSensor > LINE_THRESHOLD)
    {
        // turn left
        analogWrite(LEFT_MOTOR_PIN, LOW_SPEED);
        analogWrite(RIGHT_MOTOR_PIN, CRUISE_SPEED);
        Serial.println("LEFT");
    }
    else if (leftSensor < LINE_THRESHOLD)
    {
        analogWrite(LEFT_MOTOR_PIN, CRUISE_SPEED);
        analogWrite(RIGHT_MOTOR_PIN, LOW_SPEED);
        Serial.println("RIGHT");
    }
}
void loop()
{
    // ArduinoOTA.handle();

    float sensorOutLeft = analogRead(IR_SENSOR_INPUT_LEFT);
    float sensorOutRight = analogRead(IR_SENSOR_INPUT_RIGHT);
    // delay(500);
    // delay(200);
    // sprintf(buf, "Left sensor: %3f, Right sensor: %3f\r\n", sensorOutLeft, sensorOutRight);
    // Serial.print(buf);

    PID_pseudo(sensorOutLeft, sensorOutRight);
    // PID_pseudo_single(sensorOutLeft);
}