#include "shared_joycons.h"
#include <chrono>

// LJoycon implementation
LJoycon::LJoycon(
    uint16_t vendor_id,
    uint16_t product_id,
    const std::wstring& serial,
    bool simple_mode
)
    : JoyCon(vendor_id, product_id, serial.empty() ? 
        std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()) : serial, simple_mode),
      calibrated(false),
      serial(serial.empty() ? std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()) : serial)
{}

LJoycon::Status LJoycon::get_status() const {
    auto base = JoyCon::get_status();
    return {
        base.battery,
        base.buttons.left,
        base.analog_sticks.left,
        base.accel,
        base.gyro
    };
}

// RJoycon implementation
RJoycon::RJoycon(
    uint16_t vendor_id,
    uint16_t product_id,
    const std::wstring& serial,
    bool simple_mode
)
    : JoyCon(vendor_id, product_id, serial.empty() ? 
        std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()) : serial, simple_mode),
      calibrated(false),
      serial(serial.empty() ? std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()) : serial)
{}

RJoycon::Status RJoycon::get_status() const {
    auto base = JoyCon::get_status();
    return {
        base.battery,
        base.buttons.right,
        base.analog_sticks.right,
        base.accel,
        base.gyro
    };
}

