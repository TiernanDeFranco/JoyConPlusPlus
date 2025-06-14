#pragma once
#include "joycon.h"
#include <string>
#include <optional>

// Data structs for status
struct LJoyconData {
    JoyCon::Status::Battery battery;
    JoyCon::Status::Buttons::Side buttons;
    JoyCon::Status::AnalogSticks::Stick stick;
    JoyCon::Status::Gyro gyro;
    JoyCon::Status::Accel accel;
    std::optional<std::wstring> id;
};

struct RJoyconData {
    JoyCon::Status::Battery battery;
    JoyCon::Status::Buttons::Side buttons;
    JoyCon::Status::AnalogSticks::Stick stick;
    JoyCon::Status::Gyro gyro;
    JoyCon::Status::Accel accel;
    std::optional<std::wstring> id;
};

struct CombinedStatus {
    LJoyconData left;
    RJoyconData right;
    std::optional<std::wstring> l_id;
    std::optional<std::wstring> r_id;
};

// LJoycon
class LJoycon : public JoyCon {
public:
    LJoycon(
        uint16_t vendor_id,
        uint16_t product_id,
        const std::wstring& serial = L"",
        bool simple_mode = false
    );

    bool calibrated;
    std::wstring serial;

    struct Status {
        JoyCon::Status::Battery battery;
        JoyCon::Status::Buttons::Side buttons;
        JoyCon::Status::AnalogSticks::Stick stick;
        JoyCon::Status::Accel accel;
        JoyCon::Status::Gyro gyro;
    };

    Status get_status() const;
};

// RJoycon
class RJoycon : public JoyCon {
public:
    RJoycon(
        uint16_t vendor_id,
        uint16_t product_id,
        const std::wstring& serial = L"",
        bool simple_mode = false
    );

    bool calibrated;
    std::wstring serial;

    struct Status {
        JoyCon::Status::Battery battery;
        JoyCon::Status::Buttons::Side buttons;
        JoyCon::Status::AnalogSticks::Stick stick;
        JoyCon::Status::Accel accel;
        JoyCon::Status::Gyro gyro;
    };

    Status get_status() const;
};
