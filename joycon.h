#pragma once

#include <hidapi/hidapi.h>
#include <cstdint>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <atomic>
#include <memory>
#include <stdexcept>

class JoyCon {
public:
    static constexpr size_t INPUT_REPORT_SIZE = 49;
    static constexpr double INPUT_REPORT_PERIOD = 0.015;
    static constexpr std::array<uint8_t, 8> DEFAULT_RUMBLE_DATA = {0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40};

    JoyCon(uint16_t vendor_id, uint16_t product_id, const std::wstring& serial = L"", bool simple_mode = false);
    virtual ~JoyCon();

    // Calibration
    void set_gyro_calibration(const std::array<int16_t, 3>& offset_xyz, const std::array<int16_t, 3>& coeff_xyz);
    void set_accel_calibration(const std::array<int16_t, 3>& offset_xyz, const std::array<int16_t, 3>& coeff_xyz);

    // Register input hook
    void register_update_hook(std::function<void(JoyCon&)> callback);

    // Status
    bool is_left() const;
    bool is_right() const;

    std::wstring serial;

    // Button getters
    int get_battery_charging() const;
    int get_battery_level() const;
    int get_button_y() const;
    int get_button_x() const;
    int get_button_b() const;
    int get_button_a() const;
    int get_button_right_sr() const;
    int get_button_right_sl() const;
    int get_button_r() const;
    int get_button_zr() const;
    int get_button_minus() const;
    int get_button_plus() const;
    int get_button_r_stick() const;
    int get_button_l_stick() const;
    int get_button_home() const;
    int get_button_capture() const;
    int get_button_charging_grip() const;
    int get_button_down() const;
    int get_button_up() const;
    int get_button_right() const;
    int get_button_left() const;
    int get_button_left_sr() const;
    int get_button_left_sl() const;
    int get_button_l() const;
    int get_button_zl() const;

    // Stick getters
    int get_stick_left_horizontal() const;
    int get_stick_left_vertical() const;
    int get_stick_right_horizontal() const;
    int get_stick_right_vertical() const;

    // Accel/Gyro getters
    float get_accel_x(int sample_idx = 0) const;
    float get_accel_y(int sample_idx = 0) const;
    float get_accel_z(int sample_idx = 0) const;
    float get_gyro_x(int sample_idx = 0) const;
    float get_gyro_y(int sample_idx = 0) const;
    float get_gyro_z(int sample_idx = 0) const;

    // Lamp and rumble
    void set_player_lamp_on(int on_pattern);
    void set_player_lamp_flashing(int player_number);
    void set_player_lamp(int player_number);
    void enable_vibration(bool enable = true);
    void rumble_simple();
    void rumble_bump();
    void rumble_stop();
    void disconnect_device();

    // Status dictionary (as a struct)
    struct Status {
        struct Battery {
            int charging;
            int level;
        } battery;
        struct Buttons {
            struct Side {
                int y = 0, x = 0, b = 0, a = 0, sr = 0, sl = 0, r = 0, zr = 0, plus = 0, home = 0;
                int down = 0, up = 0, right = 0, left = 0, l = 0, zl = 0, minus = 0, capture = 0;
            } right, left;
        } buttons;
        struct AnalogSticks {
            struct Stick {
                int horizontal = 0, vertical = 0, pressed = 0;
            } left, right;
        } analog_sticks;
        struct Accel {
            float x = 0, y = 0, z = 0;
        } accel;
        struct Gyro {
            float x = 0, y = 0, z = 0;
        } gyro;
    };
    Status get_status() const;

private:
    // Internal state
    uint16_t vendor_id_;
    uint16_t product_id_;
    std::wstring serial_;
    bool simple_mode_;
    std::array<uint8_t, 3> color_body_;
    std::array<uint8_t, 3> color_btn_;

    std::vector<std::function<void(JoyCon&)>> input_hooks_;
    mutable std::array<uint8_t, INPUT_REPORT_SIZE> input_report_;
    uint8_t packet_number_;
    std::array<uint8_t, 8> rumble_data_;

    // Calibration
    int16_t GYRO_OFFSET_X_, GYRO_OFFSET_Y_, GYRO_OFFSET_Z_;
    float GYRO_COEFF_X_, GYRO_COEFF_Y_, GYRO_COEFF_Z_;
    int16_t ACCEL_OFFSET_X_, ACCEL_OFFSET_Y_, ACCEL_OFFSET_Z_;
    float ACCEL_COEFF_X_, ACCEL_COEFF_Y_, ACCEL_COEFF_Z_;

    // HID device
    hid_device* joycon_device_;
    std::thread update_input_report_thread_;
    std::atomic<bool> running_;
    mutable std::mutex report_mutex_;

    // Internal helpers
    hid_device* open(uint16_t vendor_id, uint16_t product_id, const std::wstring& serial);
    void close();
    std::array<uint8_t, INPUT_REPORT_SIZE> read_input_report() const;
    void write_output_report(const std::vector<uint8_t>& command);
    std::pair<bool, std::vector<uint8_t>> send_subcmd_get_response(uint8_t subcommand, const std::vector<uint8_t>& argument);
    std::vector<uint8_t> spi_flash_read(uint32_t address, uint8_t size);
    void update_input_report();
    void read_joycon_data();
    void setup_sensors();
    static int16_t to_int16le_from_2bytes(uint8_t hbytebe, uint8_t lbytebe);
    int get_nbit_from_input_report(size_t offset_byte, int offset_bit, int nbit) const;
    void send_rumble(const std::array<uint8_t, 8>& data);
};
