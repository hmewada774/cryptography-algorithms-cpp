#include <iostream>
#include <ctime> // For execution time
using namespace std;

unsigned int F(unsigned int x, unsigned int key)
{
    x ^= key;
    x = ((x << 1) | (x >> 31)) ^ ((x >> 3) | (x << 29));
    return x;
}

void twofishEncrypt(unsigned int &L, unsigned int &R, unsigned int keys[8])
{
    for (int i = 0; i < 8; ++i)
    {
        unsigned int f = F(R, keys[i]);
        L ^= f;
        cout << "Round " << i + 1 << " Encryption: L = " << hex << L << " R = " << hex << R << endl; // Round output
        swap(L, R);
    }
    swap(L, R);                                                                  // Final swap
    cout << "Final Encryption: L = " << hex << L << " R = " << hex << R << endl; // Final output
}

void twofishDecrypt(unsigned int &L, unsigned int &R, unsigned int keys[8])
{
    for (int i = 7; i >= 0; --i)
    {
        unsigned int f = F(R, keys[i]);
        L ^= f;
        cout << "Round " << 8 - i << " Decryption: L = " << hex << L << " R = " << hex << R << endl; // Round output
        swap(L, R);
    }
    swap(L, R);                                                                  // Final swap
    cout << "Final Decryption: L = " << hex << L << " R = " << hex << R << endl; // Final output
}

int main()
{
    clock_t startTime, endTime;
    startTime = clock(); // Start measuring execution time

    unsigned int L = 0x11223344;
    unsigned int R = 0xAABBCCDD;
    unsigned int keys[8] = {
        0x01020304, 0x11121314, 0x21222324, 0x31323334,
        0x41424344, 0x51525354, 0x61626364, 0x71727374};

    cout << "Plaintext:  " << hex << L << " " << R << endl;

    twofishEncrypt(L, R, keys);
    cout << "Encrypted:  " << hex << L << " " << R << endl;

    twofishDecrypt(L, R, keys);
    cout << "Decrypted:  " << hex << L << " " << R << endl;

    endTime = clock(); // End measuring execution time
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "\nTotal Execution Time: " << elapsedTime << " seconds" << endl;

    // Memory usage estimation (size of variables in bytes)
    size_t memoryUsed = sizeof(L) + sizeof(R) + sizeof(keys);
    cout << "Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. Twofish is a block cipher with a key size of 128, 192, or 256 bits and uses a 128-bit block size.\n";
    cout << "2. It is known for being secure and efficient. However, no cipher is immune to brute-force attacks if the key is small enough.\n";
    cout << "3. The round function in Twofish is designed to provide confusion and diffusion, making it more resistant to cryptanalysis than simpler ciphers like DES.\n";
    cout << "4. Twofish has been analyzed extensively and is considered secure against both linear and differential cryptanalysis.\n";
    cout << "5. A weakness in Twofish, like other block ciphers, is that if the key is reused or weakly chosen, the cipher can become vulnerable.\n";
    cout << "6. The key expansion process in Twofish is designed to improve key sensitivity, but it is still essential to use strong, randomly chosen keys.\n";

    return 0;
}
