#include <iostream>
#include <ctime> // For measuring execution time
using namespace std;

int strLength(char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

void generateKey(char *message, char *keyword, char *key)
{
    int msgLen = strLength(message);
    int keyLen = strLength(keyword);
    for (int i = 0; i < msgLen; i++)
    {
        key[i] = keyword[i % keyLen];
    }
    key[msgLen] = '\0';
}

void encrypt(char *message, char *key, char *cipher)
{
    int len = strLength(message);
    for (int i = 0; i < len; i++)
    {
        if (message[i] >= 'A' && message[i] <= 'Z')
        {
            cipher[i] = ((message[i] - 'A') + (key[i] - 'A')) % 26 + 'A';
            cout << "Round " << i + 1 << " Encryption: " << cipher[i] << endl; // Output for each round
        }
        else
        {
            cipher[i] = message[i];
        }
    }
    cipher[len] = '\0';
}

void decrypt(char *cipher, char *key, char *message)
{
    int len = strLength(cipher);
    for (int i = 0; i < len; i++)
    {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z')
        {
            message[i] = ((cipher[i] - key[i] + 26) % 26) + 'A';
            cout << "Round " << i + 1 << " Decryption: " << message[i] << endl; // Output for each round
        }
        else
        {
            message[i] = cipher[i];
        }
    }
    message[len] = '\0';
}

int main()
{
    clock_t startTime, endTime;
    startTime = clock(); // Start measuring execution time

    const int MAX = 100;
    char message[MAX], keyword[MAX], key[MAX], cipher[MAX], decrypted[MAX];

    cout << "Enter UPPERCASE message: ";
    cin.getline(message, MAX);
    cout << "Enter UPPERCASE keyword: ";
    cin.getline(keyword, MAX);

    generateKey(message, keyword, key);

    encrypt(message, key, cipher);
    cout << "Encrypted message: " << cipher << endl;

    decrypt(cipher, key, decrypted);
    cout << "Decrypted message: " << decrypted << endl;

    endTime = clock(); // End measuring execution time
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "\nTotal Execution Time: " << elapsedTime << " seconds" << endl;

    // Memory usage estimation (size of variables in bytes)
    size_t memoryUsed = sizeof(message) + sizeof(keyword) + sizeof(key) + sizeof(cipher) + sizeof(decrypted);
    cout << "Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. The Vigenère cipher is a polyalphabetic cipher and is much more secure than simpler ciphers like Caesar cipher.\n";
    cout << "2. If the keyword is long and random enough, the Vigenère cipher is resistant to frequency analysis.\n";
    cout << "3. The main weakness of Vigenère is that if the keyword is short or repeated, frequency analysis may still be successful.\n";
    cout << "4. Known plaintext attacks are one way to break the cipher, especially if the attacker has enough ciphertext and knows part of the plaintext.\n";
    cout << "5. To improve security, the key should be long and randomly generated, making it difficult to guess or break through cryptanalysis.\n";

    return 0;
}
