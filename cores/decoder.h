#include <string>
#include <vector>

constexpr char kBase64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline bool isEncoded(char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string decode(const std::string& base64) {
    std::string decoded;
    int val = 0, valb = -8;
    for (char c : base64) {
        if (isEncoded(c)) {
            val = (val << 6) + (unsigned char)kBase64Chars[c];
            valb += 6;
            if (valb >= 0) {
                decoded.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
    }
    return decoded;
}

std::vector<uint8_t> decodeToBytes(const std::string& base64) {
    std::vector<uint8_t> bytes;
    int val = 0, valb = -8;
    for (char c : base64) {
        if (isEncoded(c)) {
            val = (val << 6) + (unsigned char)kBase64Chars[c];
            valb += 6;
            if (valb >= 0) {
                bytes.push_back((val >> valb) & 0xFF);
                valb -= 8;
            }
        }
    }
    return bytes;
}