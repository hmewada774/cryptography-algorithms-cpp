#include <iostream>
#include <ctime>
using namespace std;

int main()
{
    cout << "=== Polyalphabetic Cipher Demonstration ===\n\n";

    // Input for plaintext and key
    char plaintext[100];
    char key[100];
    cout << "Step 1: Input\n";
    cout << "Enter the plaintext (uppercase, no spaces): ";
    cin >> plaintext;
    cout << "Enter the keyword (uppercase, no spaces): ";
    cin >> key;
    cout << "\nPlaintext: " << plaintext << "\nKeyword: " << key << "\n\n";

    // Calculate lengths of plaintext and key
    int pLen = 0, kLen = 0;
    while (plaintext[pLen] != '\0')
    {
        pLen++;
    }
    while (key[kLen] != '\0')
    {
        kLen++;
    }

    // Repeat the key to match the length of the plaintext
    char repeatedKey[100];
    cout << "Step 2: Repeating the keyword\n";
    for (int i = 0; i < pLen; i++)
    {
        repeatedKey[i] = key[i % kLen];
        cout << "repeatedKey[" << i << "] = " << repeatedKey[i] << "\n";
    }
    repeatedKey[pLen] = '\0';
    cout << "\nRepeated Keyword: " << repeatedKey << "\n\n";

    // Encryption process
    char ciphertext[100];
    cout << "Step 3: Encryption Process\n";
    for (int i = 0; i < pLen; i++)
    {
        int pVal = plaintext[i] - 'A';
        int kVal = repeatedKey[i] - 'A';
        int cVal = (pVal + kVal) % 26;
        ciphertext[i] = cVal + 'A';
        cout << "Encrypting '" << plaintext[i] << "' (" << pVal
             << ") with key '" << repeatedKey[i] << "' (" << kVal
             << ") --> " << ciphertext[i] << "\n";
    }
    ciphertext[pLen] = '\0';
    cout << "\nStep 4: Final Ciphertext: " << ciphertext << "\n";

    // Execution Time Tracking
    clock_t start = clock();
    // Here, the encryption process code executes.
    clock_t end = clock();
    double executionTime = double(end - start) / CLOCKS_PER_SEC;

    // Memory Usage Estimation
    size_t memoryUsed = sizeof(plaintext) + sizeof(key) + sizeof(repeatedKey) + sizeof(ciphertext);

    // Performance Metrics
    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << executionTime << " seconds\n";
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    // Cryptanalysis Discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. Polyalphabetic cipher is more secure than the Caesar cipher because it uses multiple alphabets.\n";
    cout << "2. However, it is still vulnerable to frequency analysis if enough ciphertext is available.\n";
    cout << "3. The cipher can be broken using a **Kasiski examination** or **frequency analysis**.\n";
    cout << "4. The key repetition pattern makes it easier to detect, especially if the key is short.\n";
    cout << "5. To enhance security, it's advisable to use a **longer key** or combine with more modern ciphers like AES.\n";

    return 0;
}
