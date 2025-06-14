// constants.h
#pragma once
#include <cstdint>
#include <set>

constexpr uint16_t JOYCON_VENDOR_ID    = 0x057E;
constexpr uint16_t JOYCON_L_PRODUCT_ID = 0x2006;
constexpr uint16_t JOYCON_R_PRODUCT_ID = 0x2007;


const std::set<uint16_t> JOYCON_PRODUCT_IDS = {
    JOYCON_L_PRODUCT_ID,
    JOYCON_R_PRODUCT_ID
};
