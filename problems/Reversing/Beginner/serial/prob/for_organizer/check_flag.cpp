#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

const char* bb = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string e(unsigned int p) {
    std::string num_str = std::to_string(p);
    size_t len = num_str.length();
    std::string k;

    for (size_t i = 0; i < len; i += 3) {
        unsigned char b1 = num_str[i] - '0';
        unsigned char b2 = (i + 1 < len) ? num_str[i + 1] - '0' : 0;
        unsigned char b3 = (i + 2 < len) ? num_str[i + 2] - '0' : 0;

        unsigned int triple = (b1 << 8) | (b2 << 4) | b3;

        k += bb[(triple >> 12) & 0x3F];
        k += bb[(triple >> 6) & 0x3F];
        k += (i + 1 < len) ? bb[triple & 0x3F] : '=';
        k += (i + 2 < len) ? bb[(triple << 6) & 0x3F] : '=';
    }
    return k;
}

bool is_number(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    return true;
}

void input_check(unsigned int user_input, unsigned int p, const std::string& name) {
    if (name != "hello") {
        std::cout << "invalid input" << std::endl;
        return;
    }
    if (user_input != p) {
        std::cout << "invalid input" << std::endl;
        return;
    }

    std::string k_p = e(p);
    std::string cc;
    for (char c : name) {
        cc += std::to_string(static_cast<int>(c));
    }
    std::cout << "hspace{" << k_p << cc << "}" << std::endl;
}

int main() {
    std::string name;
    std::cout << "Enter name: ";
    std::getline(std::cin, name);

    if (name.empty()) {
        std::cout << "write down input" << std::endl;
        system("pause");
        return 1;
    }

    unsigned int p = 0;
    for (char c : name) {
        p += static_cast<unsigned int>(c);
    }

    std::string serial_input;
    std::cout << "Enter serial: ";
    std::getline(std::cin, serial_input);

    if (serial_input.empty()) {
        std::cout << "write down input" << std::endl;
        system("pause");
        return 1;
    }

    try {
        unsigned int user_input = std::stoul(serial_input);
        input_check(user_input, p, name);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "invalid input" << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "invalid input" << std::endl;
    }

    system("pause");
    return 0;
}
