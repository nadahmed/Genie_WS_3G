#include "head.h"
#include <JC_Button.h>

const uint8_t relay[] = {12, 5, 4};
const uint8_t button[] = {0, 9, 10};

const uint8_t sizeOfRtss = 3;
const uint8_t wifi_search = 13;
uint8_t id;
uint8_t power[3];

bool enabled[3];
bool isOn[3];
bool isActivated[3];
bool searchLight;
bool keepLEDon = true;
bool reset;
bool swState[3];
uint32_t blinkTime = 0;

bool ledState;        // current LED status
unsigned long ms;     // current time from millis()
unsigned long msLast; // last time the LED was switched

Button myBtn[] = {
    Button(button[0]),
    Button(button[1]),
    Button(button[2]),
};

void switchAction()
{

    for (uint8_t i = 0; i < sizeOfRtss; i++)
    {
        myBtn[i].read(); // read the button
    }

    for (uint8_t i = 0; i < sizeOfRtss; i++)
    {
        if (myBtn[i].wasReleased())
        {
            enabled[i] = false;
            isOn[i] = !isOn[i];
            Serial.println(i);
        }
    }

    if (myBtn[1].pressedFor(5000))
    {
    }
}

void pinInitialization()
{
    for (uint8_t i = 0; i < sizeOfRtss; i++)
    {
        pinMode(relay[i], OUTPUT);
        myBtn[i].begin();
        digitalWrite(relay[i], LOW);
        swState[i] = false;
        enabled[i] = false;
        isOn[i] = true;
        isActivated[i] = false;
    }

    pinMode(wifi_search, OUTPUT);

    searchLight = true;
    keepLEDon = true;
    reset = false;
    blinkTime = 0;
}

void operate()
{
    switchAction();
    for (uint8_t i = 0; i < sizeOfRtss; i++)
    {
        digitalWrite(relay[i], isOn[i]);
    }
}

void printValues()
{
    Serial.println("");
    for (unsigned int i = 0; i < sizeOfRtss; i++)
    {
        Serial.print(enabled[i]);
        Serial.print('\t');
        Serial.print(isOn[i]);
        Serial.print('\t');
        Serial.print(isActivated[i]);
        Serial.print('\t');
        Serial.println(power[i]);
    }

    Serial.println(searchLight);
    Serial.println(keepLEDon);
    Serial.println(reset);
    Serial.println('\n');
}
