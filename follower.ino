#include <Arduino.h>

#define IR_SENSOR_INPUT_LEFT 15
#define IR_SENSOR_INPUT_RIGHT 14
#define RIGHT_MOTOR_PIN 13
#define LEFT_MOTOR_PIN 12

#define LOW_SPEED 255
#define HIGH_SPEED 85       // 0 bad battery
#define CRUISE_SPEED 110    // 30
#define LINE_THRESHOLD 4050 // for analog, 0.9 for digital

enum state
{
    FORWARD,
    LEFT,
    RIGHT,
    STOP
};
int bufLen = 150;
int bufferCounter = 0;
state lastState = FORWARD; // just to initialize

char buf[64]; // print buffer for debug

void setup()
{
    Serial.begin(9600);
    pinMode(IR_SENSOR_INPUT_LEFT, INPUT);  // Sensor output
    pinMode(IR_SENSOR_INPUT_RIGHT, INPUT); // Sensor output
    pinMode(LEFT_MOTOR_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_PIN, OUTPUT);
}

void drive(float leftSensor, float rightSensor)
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
        // this is the return to track control

        // use this to stop immediately
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
        // an improvement would be to handle `else` case
    }
}

void drive_single(float leftSensor)
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
    float sensorOutLeft = analogRead(IR_SENSOR_INPUT_LEFT);
    float sensorOutRight = analogRead(IR_SENSOR_INPUT_RIGHT);

    // use this to test the sensors
    // delay(500);
    // sprintf(buf, "Left sensor: %3f, Right sensor: %3f\r\n", sensorOutLeft, sensorOutRight);
    // Serial.print(buf);

    drive(sensorOutLeft, sensorOutRight);

    // try this to drive with only one sensor
    // drive_single(sensorOutLeft);
}