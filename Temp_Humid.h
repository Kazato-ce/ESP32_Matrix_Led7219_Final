#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include "Globals.h"
#include "ModeBase.h"

class ModeSensor : public DoubleHeightMode
{
private:
    char _buffer[30];

    bool showTemp = true;

    unsigned long lastSwitch = 0;

public:

    void refreshData() override
    {
        float t = dht.readTemperature();
float h = dht.readHumidity();

if (isnan(t) || isnan(h))
{
    strcpy(_buffer, "SENSOR ERROR");
    return;
}

char buf = ' ';

if (showTemp)
{
    buf = char(127);
    sprintf(_buffer, "%c %.0fC", buf, t);
}
else
{ 
    if (h < 40)
    {
        buf = char(30);
    }
    else if (h <= 65)
    {
        buf = char(31);
    }
    else
    {
        buf = char(126);
    }

    sprintf(_buffer, "%c %.0f%%", buf, h);
}
    }

    void setupMode(textEffect_t effect, uint16_t speed) override
    {
        refreshData();

        P.displayClear();

        // clear zone cũ
        P.displayZoneText(ZONE_LOWER,"",PA_CENTER,0,0,PA_PRINT,PA_NO_EFFECT);

        // hiện text
        P.displayZoneText(ZONE_UPPER,_buffer,PA_CENTER,40,1000,PA_PRINT,PA_NO_EFFECT);

        P.synchZoneStart();
    }

    void run() override
    {
        // đổi TEMP/HUMI mỗi 3 giây
        if(millis() - lastSwitch >= 3000)
        {
            lastSwitch = millis();

            showTemp = !showTemp;

            refreshData();

            P.displayZoneText(ZONE_LOWER,_buffer,PA_CENTER,40,1000,PA_PRINT,PA_NO_EFFECT);
            P.displayZoneText(ZONE_UPPER,_buffer,PA_CENTER,40,1000,PA_PRINT,PA_NO_EFFECT);
            P.displayReset(ZONE_LOWER);
            P.displayReset(ZONE_UPPER);
        }

        P.displayAnimate();
    }
};

#endif