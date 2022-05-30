#pragma once

namespace component {
    namespace ongeki_hardware {
        void start();
        bool read_io(raw_hid::output_data_t *data);
        void set_led(raw_hid::led_t &data);
        void update();
    }
}
