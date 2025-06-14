#include "helpers.h"
#include "shared_joycons.h"
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <chrono>
#include <thread>


int main() {
    std::map<std::wstring, std::shared_ptr<JoyCon>> joycons;
    std::vector<std::wstring> unbound;

    std::cout << "Running" << std::endl;

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Discover all Joy-Cons
    discover_all_joycons(joycons, unbound);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Running After Code" << std::endl;
    std::cout << "discover_all_joycons took " << elapsed.count() << " ms" << std::endl;

    // Print out the discovered Joy-Cons
    std::wcout << L"Discovered Joy-Cons:" << std::endl;
    for (const auto& pair : joycons) {
        std::wcout << L"  Serial: " << pair.first << std::endl;
        if (dynamic_cast<LJoycon*>(pair.second.get())) {
            std::wcout << L"    Type: Left" << std::endl;
        } else if (dynamic_cast<RJoycon*>(pair.second.get())) {
            std::wcout << L"    Type: Right" << std::endl;
        } else {
            std::wcout << L"    Type: Unknown" << std::endl;
        }
    }

    // Print unbound Joy-Cons
    std::wcout << L"Unbound Joy-Cons:" << std::endl;
    for (const auto& serial : unbound) {
        std::wcout << L"  " << serial << std::endl;
    }


    return 0;
}
