#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

static bool parse_ipv4_to_u32(const std::string& s, uint32_t& out) {
    uint32_t oct[4] = {0,0,0,0};
    int idx = 0;
    uint32_t cur = 0;
    bool have_digit = false;

    for (char c : s) {
        if (c >= '0' && c <= '9') {
            have_digit = true;
            cur = cur * 10 + (c - '0');
            if (cur > 255) return false;
        } else if (c == '.') {
            if (!have_digit || idx >= 3) return false;
            oct[idx++] = cur;
            cur = 0;
            have_digit = false;
        } else {
            return false;
        }
    }
    if (!have_digit || idx != 3) return false;
    oct[3] = cur;

    out = (oct[0] << 24) | (oct[1] << 16) | (oct[2] << 8) | oct[3];
    return true;
}

static inline uint8_t b1(uint32_t ip){ return (ip >> 24) & 0xFF; }
static inline uint8_t b2(uint32_t ip){ return (ip >> 16) & 0xFF; }
static inline uint8_t b3(uint32_t ip){ return (ip >> 8)  & 0xFF; }
static inline uint8_t b4(uint32_t ip){ return  ip        & 0xFF; }

static inline void print_ip(uint32_t ip) {
    std::cout << unsigned(b1(ip)) << '.'
              << unsigned(b2(ip)) << '.'
              << unsigned(b3(ip)) << '.'
              << unsigned(b4(ip)) << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<uint32_t> ips;
    ips.reserve(100000);

    std::string line;
    while (std::getline(std::cin, line)) {
        // берем до \t
        size_t tab = line.find('\t');
        std::string first = (tab == std::string::npos) ? line : line.substr(0, tab);

        uint32_t ip;
        if (parse_ipv4_to_u32(first, ip)) {
            ips.push_back(ip);
        }
        // невалидные строки игнорируем
    }

    // Обратная лексикографическая по байтам
    std::sort(ips.begin(), ips.end(), std::greater<uint32_t>());

    // Полный список
    for (uint32_t ip : ips) print_ip(ip);

    // 1
    for (uint32_t ip : ips) if (b1(ip) == 1) print_ip(ip);

    // 46 70
    for (uint32_t ip : ips) if (b1(ip) == 46 && b2(ip) == 70) print_ip(ip);

    //любой 46
    for (uint32_t ip : ips) {
        if (b1(ip) == 46 || b2(ip) == 46 || b3(ip) == 46 || b4(ip) == 46)
            print_ip(ip);
    }
    return 0;
}
