#include <iostream>
#include <cstring>
#include <ctime> // For measuring execution time
using namespace std;

void encryptRailFence(const char *text, int key, char *result)
{
    int len = strlen(text);
    char rail[key][len];

    for (int i = 0; i < key; i++)
        for (int j = 0; j < len; j++)
            rail[i][j] = '\n';

    bool dir_down = false;
    int row = 0;
    for (int i = 0; i < len; i++)
    {
        rail[row][i] = text[i];
        if (row == 0 || row == key - 1)
            dir_down = !dir_down;
        row += dir_down ? 1 : -1;
    }

    // Output the state of the matrix for each round
    cout << "Step 1: Rail Matrix after encryption process:\n";
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < len; j++)
        {
            cout << rail[i][j] << " ";
        }
        cout << endl;
    }

    int index = 0;
    for (int i = 0; i < key; i++)
        for (int j = 0; j < len; j++)
            if (rail[i][j] != '\n')
                result[index++] = rail[i][j];

    result[index] = '\0';
}

void decryptRailFence(const char *cipher, int key, char *result)
{
    int len = strlen(cipher);
    char rail[key][len];

    for (int i = 0; i < key; i++)
        for (int j = 0; j < len; j++)
            rail[i][j] = '\n';

    bool dir_down;
    int row = 0;
    for (int i = 0; i < len; i++)
    {
        rail[row][i] = '*';
        if (row == 0)
            dir_down = true;
        else if (row == key - 1)
            dir_down = false;
        row += dir_down ? 1 : -1;
    }

    int index = 0;
    for (int i = 0; i < key; i++)
        for (int j = 0; j < len; j++)
            if (rail[i][j] == '*' && index < len)
                rail[i][j] = cipher[index++];

    row = 0;
    dir_down = true;
    for (int i = 0; i < len; i++)
    {
        result[i] = rail[row][i];
        if (row == 0)
            dir_down = true;
        else if (row == key - 1)
            dir_down = false;
        row += dir_down ? 1 : -1;
    }
    result[len] = '\0';

    // Output the state of the matrix after decryption
    cout << "Step 2: Rail Matrix after decryption process:\n";
    for (int i = 0; i < key; i++)
    {
        for (int j = 0; j < len; j++)
        {
            cout << rail[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    const int MAX = 100;
    char text[MAX], encrypted[MAX], decrypted[MAX];
    int key;

    // Input the text and key
    cout << "Enter message: ";
    cin.getline(text, MAX);
    cout << "Enter key (number of rails): ";
    cin >> key;

    // Start timing the encryption process
    clock_t start = clock();

    encryptRailFence(text, key, encrypted);
    cout << "Encrypted: " << encrypted << endl;

    // Time taken for encryption
    clock_t end = clock();
    double encryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Encryption Time: " << encryptionTime << " seconds\n";

    // Start timing the decryption process
    start = clock();

    decryptRailFence(encrypted, key, decrypted);
    cout << "Decrypted: " << decrypted << endl;

    // Time taken for decryption
    end = clock();
    double decryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Decryption Time: " << decryptionTime << " seconds\n";

    // Memory Usage Estimation
    size_t memoryUsed = sizeof(text) + sizeof(encrypted) + sizeof(decrypted) + sizeof(key);
    cout << "\nMemory Usage: " << memoryUsed << " bytes\n";

    // Cryptanalysis Discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. The Rail Fence cipher is a **transposition cipher**, meaning it only rearranges the positions of the letters.\n";
    cout << "2. It is **weak** in terms of cryptography, as the pattern of the rails can be easily deduced with frequency analysis.\n";
    cout << "3. If the number of rails is too small (e.g., 2 or 3), the cipher can be easily broken.\n";
    cout << "4. Increasing the number of rails improves security, but **modern cryptography** methods (like AES) are far stronger.\n";
    cout << "5. The cipher does not provide any confusion (mixing of characters), making it vulnerable to attacks such as **brute force** or **known plaintext** attacks.\n";

    return 0;
}
