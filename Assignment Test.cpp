#include <iostream>
#include <cstdint>
#define NUM_ROUNDS 8
uint8_t round_sboxes[NUM_ROUNDS][256];
void generate_modinv_sbox(uint8_t sbox[256]) {
    sbox[0] = 0;
    for (int x = 1; x < 256; x++) {
        for (int y = 1; y < 256; y++) {
            if ((x * y) % 257 == 1) {
                sbox[x] = y % 256;
                break;
            }
        }
    }
}
void init_round_sboxes() {
    for (int i = 0; i < NUM_ROUNDS; i++) {
        generate_modinv_sbox(round_sboxes[i]);
    }
}
uint32_t feistel_function(uint32_t half_block, uint32_t round_key, uint8_t sbox[256]) {
    uint32_t x = half_block ^ round_key;
    return (sbox[(x >> 0) & 0xFF] << 0) |
           (sbox[(x >> 8) & 0xFF] << 8) |
           (sbox[(x >> 16) & 0xFF] << 16) |
           (sbox[(x >> 24) & 0xFF] << 24);
}
uint64_t feistel_encrypt(uint64_t plaintext, uint32_t key) {
    uint32_t L = static_cast<uint32_t>(plaintext >> 32);
    uint32_t R = static_cast<uint32_t>(plaintext);
    for (int i = 0; i < NUM_ROUNDS; i++) {
        uint32_t round_key = key ^ (i * 0x9e3779b9);
        uint32_t F = feistel_function(R, round_key, round_sboxes[i]);
        uint32_t temp = R;
        R = L ^ F;
        L = temp;
    }
    return (static_cast<uint64_t>(L) << 32) | R;
}
uint64_t feistel_decrypt(uint64_t ciphertext, uint32_t key) {
    uint32_t L = static_cast<uint32_t>(ciphertext >> 32);
    uint32_t R = static_cast<uint32_t>(ciphertext);
    for (int i = NUM_ROUNDS - 1; i >= 0; i--) {
        uint32_t round_key = key ^ (i * 0x9e3779b9);
        uint32_t F = feistel_function(L, round_key, round_sboxes[i]);
        uint32_t temp = L;
        L = R ^ F;
        R = temp;
    }
    return (static_cast<uint64_t>(L) << 32) | R;
}
int hamming_distance(uint64_t a, uint64_t b) {
    uint64_t x = a ^ b;
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}
void differential_analysis(uint64_t plaintext, uint32_t key) {
    std::cout << "\n[Differential Cryptanalysis]\n";
    uint64_t base = feistel_encrypt(plaintext, key);
    for (int i = 0; i < 64; i++) {
        uint64_t modified = plaintext ^ (1ULL << i);
        uint64_t result = feistel_encrypt(modified, key);
        int diff = hamming_distance(base, result);
        std::cout << "  Flip bit " << i << ": " << diff << " bit(s) changed\n";
    }
}
void linear_analysis(uint32_t key) {
    std::cout << "\n[Linear Cryptanalysis]\n";
    int matches = 0, trials = 10000;
    for (int i = 0; i < trials; i++) {
        uint64_t plain = static_cast<uint64_t>(i * 982451653ULL);
        uint64_t cipher = feistel_encrypt(plain, key);
        int pbit = (plain >> 0) & 1;
        int cbit = (cipher >> 31) & 1;
        if (pbit == cbit) matches++;
    }
    double bias = static_cast<double>(matches - (trials / 2)) / (trials / 2);
    std::cout << "  Correlation (P[0] <-> C[31]): Bias = " << bias << "\n";
}
int main() {
    uint64_t plaintext;
    uint32_t key;
    std::cout << "Feistel Cipher with Dynamic S-Boxes (C++ version)\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "Enter 64-bit plaintext (hex): 0x";
    std::cin >> std::hex >> plaintext;
    std::cout << "Enter 32-bit key      (hex): 0x";
    std::cin >> std::hex >> key;
    init_round_sboxes();
    uint64_t ciphertext = feistel_encrypt(plaintext, key);
    std::cout << "\nEncrypted Ciphertext: 0x" << std::hex << ciphertext << "\n";
    uint64_t decrypted = feistel_decrypt(ciphertext, key);
    std::cout << "Decrypted Plaintext : 0x" << std::hex << decrypted << "\n";
    if (decrypted == plaintext) {
        std::cout << "Decryption successful!\n";
    } else {
        std::cout << "Decryption failed!\n";
    }
    differential_analysis(plaintext, key);
    linear_analysis(key);
    return 0;
}