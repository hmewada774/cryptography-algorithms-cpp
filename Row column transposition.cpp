#include <iostream>
#include <ctime> // For timing
using namespace std;

int getLength(char *msg)
{
    int len = 0;
    while (msg[len] != '\0')
        len++;
    return len;
}

int padMessage(char *msg, int key)
{
    int len = getLength(msg);
    int paddedLen = (len % key == 0) ? len : ((len / key) + 1) * key;

    for (int i = len; i < paddedLen; i++)
    {
        msg[i] = 'X';
    }
    msg[paddedLen] = '\0';
    return len;
}

void encrypt(char *msg, int key, char *cipher)
{
    int len = getLength(msg);
    int rows = len / key;
    int index = 0;

    for (int col = 0; col < key; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            cipher[index++] = msg[row * key + col];
            cout << "Encrypting: " << msg[row * key + col] << " -> " << cipher[index - 1] << endl; // Round output
        }
    }
    cipher[index] = '\0';
}

void decrypt(char *cipher, int key, int originalLen, char *decrypted)
{
    int len = getLength(cipher);
    int rows = len / key;
    int index = 0;

    for (int col = 0; col < key; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            decrypted[row * key + col] = cipher[index++];
            cout << "Decrypting: " << cipher[index - 1] << " -> " << decrypted[row * key + col] << endl; // Round output
        }
    }
    decrypted[originalLen] = '\0';
}

int main()
{
    const int MAX = 100;
    char message[MAX], cipher[MAX], decrypted[MAX];
    int key;

    // Measure execution time for encryption
    cout << "Enter UPPERCASE message (no spaces): ";
    cin.getline(message, MAX);

    cout << "Enter key (number of columns): ";
    cin >> key;

    // Timing padding
    clock_t start = clock();
    int originalLen = padMessage(message, key);
    clock_t end = clock();
    double paddingTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nPadding Time: " << paddingTime << " seconds" << endl;

    // Timing encryption
    start = clock();
    encrypt(message, key, cipher);
    end = clock();
    double encryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nEncrypted message: " << cipher << endl;
    cout << "\nEncryption Time: " << encryptionTime << " seconds" << endl;

    // Timing decryption
    start = clock();
    decrypt(cipher, key, originalLen, decrypted);
    end = clock();
    double decryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nDecrypted message: " << decrypted << endl;
    cout << "Decryption Time: " << decryptionTime << " seconds" << endl;

    // Memory usage estimation (in bytes)
    size_t memoryUsed = sizeof(message) + sizeof(cipher) + sizeof(decrypted) + sizeof(key);
    cout << "\nMemory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. The **Columnar Transposition Cipher** is a simple transposition cipher.\n";
    cout << "2. This cipher is more secure than a Caesar cipher because the order of characters is altered, but it still suffers from weaknesses.\n";
    cout << "3. If the key is known, the message can be easily reconstructed, making the cipher vulnerable to **brute-force attacks**.\n";
    cout << "4. The cipher can also be vulnerable to **frequency analysis**, especially if the message is long enough.\n";
    cout << "5. For higher security, **modern cryptographic algorithms** such as **AES** are recommended.\n";

    return 0;
}
