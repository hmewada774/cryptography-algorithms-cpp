#include <iostream>
#include <ctime> // For measuring execution time
using namespace std;

int getLength(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

void streamEncrypt(char *message, char *key, char *cipher)
{
    int i = 0;
    while (message[i] != '\0')
    {
        cipher[i] = message[i] ^ key[i]; // XOR encryption
        cout << "Round " << i + 1 << " (Encrypting): Message Char: " << message[i]
             << ", Key Char: " << key[i] << " => Cipher Char: " << (int)cipher[i] << endl; // Output of each round
        i++;
    }
    cipher[i] = '\0';
}

void streamDecrypt(char *cipher, char *key, char *message)
{
    int i = 0;
    while (cipher[i] != '\0')
    {
        message[i] = cipher[i] ^ key[i]; // XOR decryption (same as encryption)
        cout << "Round " << i + 1 << " (Decrypting): Cipher Char: " << (int)cipher[i]
             << ", Key Char: " << key[i] << " => Message Char: " << message[i] << endl; // Output of each round
        i++;
    }
    message[i] = '\0';
}

int main()
{
    // Timing start
    clock_t startTime, endTime;
    startTime = clock();

    const int MAX = 100;
    char message[MAX], key[MAX], cipher[MAX], decrypted[MAX];

    cout << "Enter message: ";
    cin.getline(message, MAX);

    cout << "Enter key (same length): ";
    cin.getline(key, MAX);

    int len = getLength(message);

    // Encrypt message
    streamEncrypt(message, key, cipher);
    cout << "Encrypted: ";
    for (int i = 0; cipher[i] != '\0'; i++)
        cout << (int)cipher[i] << " "; // Show the encrypted message in integers
    cout << endl;

    // Decrypt message
    streamDecrypt(cipher, key, decrypted);
    cout << "Decrypted: " << decrypted << endl;

    // Timer end
    endTime = clock();
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "\nTotal Execution Time: " << elapsedTime << " seconds" << endl;

    // Memory usage estimation (size of variables in bytes)
    size_t memoryUsed = sizeof(message) + sizeof(key) + sizeof(cipher) + sizeof(decrypted);
    cout << "Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. The stream cipher works by XORing the message with the key, which is repeated if necessary.\n";
    cout << "2. This cipher is **strong** as long as the key is truly random and not reused.\n";
    cout << "3. If the key is reused or predictable, the cipher becomes vulnerable to attacks such as the **known-plaintext attack**.\n";
    cout << "4. **Key management** is critical in stream ciphers. A weak or reused key can easily compromise the security.\n";
    cout << "5. The stream cipher is often used for real-time encryption and high-speed systems, but it requires careful key generation.\n";
    cout << "6. For maximum security, the key should be as long as the message and should never be reused (this is called the **one-time pad**).\n";

    return 0;
}
