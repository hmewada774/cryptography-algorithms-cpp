#include <iostream>
#include <ctime>
using namespace std;

void generateKey(char *key, int length)
{
    srand(time(0)); // Seed PRNG with current time
    for (int i = 0; i < length; i++)
    {
        key[i] = rand() % 256; // Random byte value
    }
}

void encryptDecrypt(char *text, char *key, int length, char *output, const char *label)
{
    cout << "\n--- " << label << " Round-wise Output ---\n";
    for (int i = 0; i < length; i++)
    {
        output[i] = text[i] ^ key[i];
        cout << "Char[" << i << "] '" << text[i] << "' ^ Key[" << i << "] = "
             << (int)(unsigned char)text[i] << " ^ " << (int)(unsigned char)key[i]
             << " = " << (int)(unsigned char)output[i] << " => '" << output[i] << "'\n";
    }
}

void printText(char *text, int length)
{
    for (int i = 0; i < length; i++)
    {
        cout << text[i];
    }
    cout << endl;
}

int main()
{
    const int maxLength = 100;
    char plaintext[maxLength], ciphertext[maxLength], decryptedText[maxLength];
    char key[maxLength];

    cout << "Enter plaintext: ";
    cin.getline(plaintext, maxLength);

    int length = 0;
    while (plaintext[length] != '\0')
    {
        length++;
    }

    clock_t start = clock(); // Start time

    generateKey(key, length);
    encryptDecrypt(plaintext, key, length, ciphertext, "Encryption");
    encryptDecrypt(ciphertext, key, length, decryptedText, "Decryption");

    clock_t end = clock(); // End time

    cout << "\nEncrypted Text: ";
    printText(ciphertext, length);

    cout << "Decrypted Text: ";
    printText(decryptedText, length);

    double executionTime = double(end - start) / CLOCKS_PER_SEC;
    size_t memoryUsed = sizeof(plaintext) + sizeof(ciphertext) + sizeof(decryptedText) + sizeof(key);

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << executionTime << " seconds\n";
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    cout << "\n--- Cryptanalysis Discussion ---\n";
    cout << "1. Random key generated each run using time-based PRNG (rand()).\n";
    cout << "2. Still vulnerable to known-plaintext attacks if key reused.\n";
    cout << "3. Key is now non-deterministic, so basic replay/frequency attacks are harder.\n";
    cout << "4. For stronger protection, store or securely share the random key (e.g., via secure channel).\n";

    return 0;
}
