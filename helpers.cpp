#include "helpers.h"
#include "constants.h"
#include <hidapi/hidapi.h>
#include <chrono>
#include <iostream>
#include <set>
#include <algorithm>

// Helper: Always returns a unique identifier (serial or fallback)
std::wstring get_identifier(const wchar_t* serial) {
    if (serial && wcslen(serial) > 0) {
        return std::wstring(serial);
    } else {
        // Use current time as fallback
        return std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count());
    }
}

std::map<std::wstring, std::shared_ptr<JoyCon>>& discover_all_joycons(
    std::map<std::wstring, std::shared_ptr<JoyCon>>& joycons,
    std::vector<std::wstring>& unbound,
    bool pairing
) {
    struct hid_device_info* devices = hid_enumerate(JOYCON_VENDOR_ID, 0);
    struct hid_device_info* current = devices;

    std::set<std::wstring> known_ids;
    for (const auto& pair : joycons) {
        known_ids.insert(pair.first);
    }

    std::vector<std::thread> threads;
    std::mutex mtx; // For protecting joycons and unbound
    int count = 0;

    for (; current; current = current->next) {
        uint16_t vid = current->vendor_id;
        uint16_t pid = current->product_id;
        std::wstring identifier = get_identifier(current->serial_number);

        if (!identifier.empty() && known_ids.count(identifier)) {
            continue;
        }

        // Launch a thread for each Joy-Con initialization
        threads.emplace_back([&, vid, pid, identifier]() {
            try {
                std::shared_ptr<JoyCon> jc;
                std::string jc_type;
                if (pid == JOYCON_L_PRODUCT_ID) {
                    jc = std::make_shared<LJoycon>(vid, pid, identifier);
                    jc_type = "L";
                } else if (pid == JOYCON_R_PRODUCT_ID) {
                    jc = std::make_shared<RJoycon>(vid, pid, identifier);
                    jc_type = "R";
                } else {
                    return;
                }

                try {
                    set_player_lamps_once();
                } catch (const std::exception& ex) {
                    std::wcerr << L"Warning: Issue with Joy-Con (serial: " << identifier << L"): " << ex.what() << std::endl;
                }

                // Lock before updating shared data
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    joycons[identifier] = jc;
                    if (std::find(unbound.begin(), unbound.end(), identifier) == unbound.end()) {
                        unbound.push_back(identifier);
                    }
                    std::wcout << L"Discovered " << jc_type.c_str() << L" Joy-Con with ID: " << identifier << std::endl;
                    ++count;
                }
            } catch (const std::exception& e) {
                std::lock_guard<std::mutex> lock(mtx);
                std::wcerr << L"Error initializing Joy-Con (serial: " << identifier << L"): " << e.what() << std::endl;
            }
        });
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    if (!pairing) {
        std::wcout << L"Discovered " << count << L" devices for vendor " << std::hex << JOYCON_VENDOR_ID << std::endl;
    }

    hid_free_enumeration(devices);
    return joycons;
}

void set_player_lamps_once() {
    //empty
}
