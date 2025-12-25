#include <iostream>
#include <ctime>
using namespace std;

int strLength(const char *str)
{
    int len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

int padMessage(char *message, int key)
{
    int length = strLength(message);
    int paddedLength = ((length + key - 1) / key) * key;
    for (int i = length; i < paddedLength; i++)
    {
        message[i] = 'X';
    }
    message[paddedLength] = '\0';
    return length;
}

void encryptGerman(const char *message, int key, char *cipher)
{
    int len = strLength(message);
    int rows = len / key;
    int index = 0;

    cout << "\nEncryption (Column-wise):\n";
    for (int col = 0; col < key; col++)
    {
        cout << "Column " << col << ": ";
        for (int row = 0; row < rows; row++)
        {
            char ch = message[row * key + col];
            cipher[index++] = ch;
            cout << ch;
        }
        cout << endl;
    }
    cipher[index] = '\0';
}

void decryptGerman(const char *cipher, int key, int originalLength, char *message)
{
    int len = strLength(cipher);
    int rows = len / key;
    int index = 0;

    cout << "\nDecryption (Row-wise):\n";
    for (int col = 0; col < key; col++)
    {
        cout << "Column " << col << ": ";
        for (int row = 0; row < rows; row++)
        {
            char ch = cipher[index++];
            message[row * key + col] = ch;
            cout << ch;
        }
        cout << endl;
    }

    message[originalLength] = '\0';
}

size_t estimateMemory(const char *message, const char *cipher, const char *decrypted)
{
    return sizeof(message) + sizeof(cipher) + sizeof(decrypted);
}

int main()
{
    const int MAX = 100;
    char message[MAX], cipher[MAX], decrypted[MAX];
    int key;

    cout << "Enter UPPERCASE message (no spaces): ";
    cin.getline(message, MAX);

    cout << "Enter key (number of columns): ";
    cin >> key;

    int originalLength = padMessage(message, key);

    // Timing encryption
    clock_t startEnc = clock();
    encryptGerman(message, key, cipher);
    clock_t endEnc = clock();

    double encTime = double(endEnc - startEnc) / CLOCKS_PER_SEC;
    cout << "\nEncrypted message: " << cipher << endl;
    cout << "Encryption time: " << encTime << " seconds\n";

    // Timing decryption
    clock_t startDec = clock();
    decryptGerman(cipher, key, originalLength, decrypted);
    clock_t endDec = clock();

    double decTime = double(endDec - startDec) / CLOCKS_PER_SEC;
    cout << "\nDecrypted message: " << decrypted << endl;
    cout << "Decryption time: " << decTime << " seconds\n";

    // Memory usage
    cout << "\nApproximate Memory Used: " << estimateMemory(message, cipher, decrypted) << " bytes\n";

    // Cryptanalysis
    cout << "\n--- Cryptanalysis Insight ---\n";
    cout << "1. Weak against brute-force: Only test all possible column numbers (keys).\n";
    cout << "2. Frequency analysis helps if message is long.\n";
    cout << "3. Known plaintext attack effective if part of original is known.\n";
    cout << "4. Padding character 'X' can help identify message structure.\n";
    cout << "5. Column permutation version (with key order) is stronger than fixed-order.\n";

    return 0;
}
