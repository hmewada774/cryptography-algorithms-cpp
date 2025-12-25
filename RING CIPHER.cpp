#include <iostream>
#include <ctime> // For timing
using namespace std;

void ringEncrypt(char *message, int key, char *cipher)
{
    int i = 0;
    while (message[i] != '\0')
    {
        if (message[i] >= 'A' && message[i] <= 'Z')
        {
            cipher[i] = ((message[i] - 'A' + key) % 26) + 'A';
        }
        else
        {
            cipher[i] = message[i];
        }
        cout << "Encrypting '" << message[i] << "' -> '" << cipher[i] << "' (Key: " << key << ")\n"; // Round output
        i++;
    }
    cipher[i] = '\0';
}

void ringDecrypt(char *cipher, int key, char *message)
{
    int i = 0;
    while (cipher[i] != '\0')
    {
        if (cipher[i] >= 'A' && cipher[i] <= 'Z')
        {
            message[i] = ((cipher[i] - 'A' - key + 26) % 26) + 'A';
        }
        else
        {
            message[i] = cipher[i];
        }
        cout << "Decrypting '" << cipher[i] << "' -> '" << message[i] << "' (Key: " << key << ")\n"; // Round output
        i++;
    }
    message[i] = '\0';
}

int main()
{
    const int MAX = 100;
    char message[MAX], cipher[MAX], decrypted[MAX];
    int key;

    // Measure execution time for encryption
    cout << "Enter UPPERCASE message: ";
    cin.getline(message, MAX);

    cout << "Enter key (0-25): ";
    cin >> key;

    // Timing encryption
    clock_t start = clock();
    ringEncrypt(message, key, cipher);
    clock_t end = clock();
    double encryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nEncrypted message: " << cipher << endl;
    cout << "\nEncryption Time: " << encryptionTime << " seconds" << endl;

    // Timing decryption
    start = clock();
    ringDecrypt(cipher, key, decrypted);
    end = clock();
    double decryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nDecrypted message: " << decrypted << endl;
    cout << "Decryption Time: " << decryptionTime << " seconds" << endl;

    // Memory usage estimation (in bytes)
    size_t memoryUsed = sizeof(message) + sizeof(cipher) + sizeof(decrypted) + sizeof(key);
    cout << "\nMemory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. The **Ring Cipher** is a simple substitution cipher similar to the Caesar Cipher.\n";
    cout << "2. It is vulnerable to **brute force attacks**, as there are only 26 possible keys (one for each letter of the alphabet).\n";
    cout << "3. The ciphertext is directly related to the key and the plaintext, so if an attacker knows a portion of the plaintext and ciphertext, they can easily deduce the key.\n";
    cout << "4. The cipher does not use any form of key expansion or permutation, so it is easily broken with frequency analysis and brute force.\n";
    cout << "5. For secure encryption, modern algorithms like **AES** should be used, as they provide much higher security.\n";

    return 0;
}
