#include <iostream>
#include <cstring>
#include <ctime> // For measuring execution time
using namespace std;

int getLength(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

void substituteEncrypt(const char *msg, const char *key, char *cipher)
{
    int i = 0;
    while (msg[i] != '\0')
    {
        char ch = msg[i];
        if (ch >= 'A' && ch <= 'Z')
        {
            cipher[i] = key[ch - 'A'];
            cout << "Encrypting Round " << i + 1 << ": " << ch << " => " << cipher[i] << endl; // Output of each round
        }
        else
        {
            cipher[i] = ch;
        }
        i++;
    }
    cipher[i] = '\0';
}

void substituteDecrypt(const char *cipher, const char *key, char *msg)
{
    char reverseKey[26];
    for (int i = 0; i < 26; i++)
        reverseKey[key[i] - 'A'] = 'A' + i;

    int i = 0;
    while (cipher[i] != '\0')
    {
        char ch = cipher[i];
        if (ch >= 'A' && ch <= 'Z')
        {
            msg[i] = reverseKey[ch - 'A'];
            cout << "Decrypting Round " << i + 1 << ": " << ch << " => " << msg[i] << endl; // Output of each round
        }
        else
        {
            msg[i] = ch;
        }
        i++;
    }
    msg[i] = '\0';
}

int main()
{
    clock_t startTime, endTime;
    startTime = clock(); // Start measuring execution time

    const int MAX = 100;
    char message[MAX], cipher[MAX], decrypted[MAX];
    char key[27] = "QWERTYUIOPASDFGHJKLZXCVBNM"; // Substitution cipher key

    cout << "Enter UPPERCASE message: ";
    cin.getline(message, MAX);

    // Encrypt the message
    substituteEncrypt(message, key, cipher);
    cipher[getLength(message)] = '\0';
    cout << "Encrypted: " << cipher << endl;

    // Decrypt the message
    substituteDecrypt(cipher, key, decrypted);
    decrypted[getLength(cipher)] = '\0';
    cout << "Decrypted: " << decrypted << endl;

    endTime = clock(); // End measuring execution time
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "\nTotal Execution Time: " << elapsedTime << " seconds" << endl;

    // Memory usage estimation (size of variables in bytes)
    size_t memoryUsed = sizeof(message) + sizeof(key) + sizeof(cipher) + sizeof(decrypted);
    cout << "Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. Substitution ciphers are relatively easy to break if the key is known or if there is a pattern in the ciphertext.\n";
    cout << "2. The cipher is **vulnerable** to frequency analysis. For example, in English, 'E' is the most frequent letter, so by comparing the frequency of letters in the ciphertext with known language patterns, the key can be guessed.\n";
    cout << "3. If the key is randomly chosen and each letter maps to a different letter, the cipher becomes more difficult to break, but it is still not secure against modern cryptanalysis.\n";
    cout << "4. One-time pad is a **perfectly secure** substitution cipher, but it requires the key to be as long as the message and never reused.\n";
    cout << "5. A major weakness is that the same letter is always substituted by the same letter, meaning patterns in the ciphertext reveal clues to the plaintext.\n";
    cout << "6. Substitution ciphers were historically used but are now considered insecure for protecting sensitive information.\n";

    return 0;
}
