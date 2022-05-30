#include "stdinclude.hpp"
#include <EEPROM.h>
#include <FastLED.h>

namespace component
{
    namespace ongeki_hardware
    {
        const int LEVER = A0;
        const int LED_PIN = A1;
        const uint8_t PIN_MAP[10] = {
            // L: A B C SIDE MENU
            4, 5, 6, 2, 10,
            // R: A B C SIDE MENU
            7, 8, 9, 3, 16};

        CRGB lightColors[6];
        CRGB RightColors[1];
        CRGB LeftColors[1];

        uint_fast16_t filtedLever(uint8_t pin){
            uint_fast16_t temp = 0;
            for (int i = 0; i < 8; i++){
            temp += analogRead(pin) >> 3;
        }
            return temp;
        }
        void start()
        {
            // setup pin modes for button
            for (unsigned char i : PIN_MAP)
            {
                pinMode(i, INPUT_PULLUP);
            }

            // setup led_t
            FastLED.addLeds<WS2812B, LED_PIN, GRB>(lightColors, 6);
            FastLED.addLeds<WS2812B, 14, GRB>(RightColors, 1);
            FastLED.addLeds<WS2812B, 15, GRB>(LeftColors, 1);
        }

        bool read_io(raw_hid::output_data_t *data)
        {
            bool updated = false;
            for (auto i = 0; i < 10; i++)
            {
                auto read = digitalRead(PIN_MAP[i]) == LOW;
                if (read != data->buttons[i])
                {
                    data->buttons[i] = read;
                    updated = true;
                }
            }

            auto read = filtedLever(LEVER);
            if(read != data->lever){
                data->lever = read;
                updated = true;
            }

            if (data->buttons[4] && data->buttons[9])
            {
                EEPROM.get(0, data->aimi_id);
                data->scan = true;
            }
            else
            {
                memset(&data->aimi_id, 0, 10);
                data->scan = false;
            }
            return updated;
        }

        void set_led(raw_hid::led_t &data)
        {
            FastLED.setBrightness(data.ledBrightness);

            //for(int i = 0; i < 3; i++) {
            //    memcpy(&lightColors[2-i], &data.ledColors[i], 3);
            //memcpy(&lightColors[i + 3], &data.ledColors[i + 5], 3);
            //}
            for (int i = 0; i < 3; i++)
            {
                // game 0 -> 2

                //memcpy(&lightColors[i], &data.ledColors[i], 3);
                //memcpy(&lightColors[i+3], &data.ledColors[i + 5], 3);
                memcpy(&lightColors[5 - i], &data.ledColors[i], 3);
                memcpy(&lightColors[2 - i], &data.ledColors[i + 5], 3);
                LeftColors[0] = CRGB::Purple;
                RightColors[0] = CRGB::Purple;
            }
            /*for(int i=5;i<8;i++){
                memcpy(&lightColors[10-i], &data.ledColors[i], 3);
            }*/
            FastLED.show();
        }
    }
}
