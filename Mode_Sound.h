#ifndef MODE_SOUND_H
#define MODE_SOUND_H

#include "ModeBase.h"
#include "Globals.h"
#include <math.h>

class ModeMusic : public DoubleHeightMode
{
private:
    int lastLevels[64];
    unsigned long lastUpdate = 0;

    // =====================================================
    // VẼ PIXEL CHO MÀN HÌNH 16x64 GHÉP 2 TẦNG DỌC
    // =====================================================
    void drawPixel16(MD_MAX72XX* mx, int x, int y, bool state)
    {
        // tầng trên
        if(y >= 8)
        {
            mx->setPoint(y - 8, x, state);
        }
        else
        {
            // tầng dưới
            mx->setPoint(y, x + 64, state);
        }
    }

public:

    void setupMode(textEffect_t effect, uint16_t speed) override
    {
        P.displayClear();

        for(int i = 0; i < 64; i++)
        {
            lastLevels[i] = 0;
        }
    }

    void refreshData() override
    {
        MD_MAX72XX* mx = P.getGraphicObject();

        // Đọc âm thanh
        int raw = analogRead(SOUND_PIN);

        // Clear toàn màn hình
        mx->clear();

        // =====================================================
        // VẼ 64 CỘT
        // =====================================================
        for(int x = 0; x < 64; x++)
        {
            // Map âm thanh ra chiều cao 0 -> 16
            int targetLevel = map(raw, 0,10, 0, 16);

            // Sóng chạy
            float wave =
            sin(
                (millis() / 80.0) +
                (x * 0.45)
            );

            int waveOffset = wave * 4;

            targetLevel += waveOffset;

            // Random nhẹ
            targetLevel += random(-4, 4);

            // Giới hạn
            targetLevel = constrain(targetLevel, 0, 16);

            // Noise filter
            if(targetLevel < 2)
            {
                targetLevel = 0;
            }

            // Smooth
            lastLevels[x] =(lastLevels[x] * 2 +targetLevel) / 3;

            // =====================================================
            // VẼ CỘT
            // =====================================================
            for(int y = 0; y < lastLevels[x]; y++)
            {
                int drawY = 15 - y;

                drawPixel16(mx,x,drawY,true);
            }
        }

        mx->update();
    }

    void run() override
    {
        if(millis() - lastUpdate >= 5)
        {
            lastUpdate = millis();

            refreshData();
        }
    }
};

#endif