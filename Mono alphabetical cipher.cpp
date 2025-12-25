#include <iostream>
#include <ctime> // For timing
using namespace std;

size_t estimateMemoryUsage(int textLength)
{
    return sizeof(char) * (textLength * 2 + 52); // plaintext + ciphertext + key maps
}

int main()
{
    string original = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string key = "QWERTYUIOPASDFGHJKLZXCVBNM";
    cout << "Substitution Key:\n";
    for (int i = 0; i < 26; ++i)
        cout << original[i] << " -> " << key[i] << "\n";

    string plaintext;
    cout << "\nEnter plaintext: ";
    getline(cin, plaintext);

    clock_t start = clock(); // Start time

    string ciphertext;
    cout << "\nProcessing Steps:\n";
    for (int i = 0; i < plaintext.size(); ++i)
    {
        char c = plaintext[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
        {
            char upper_c = (c >= 'a' && c <= 'z') ? c - 32 : c;
            int index = upper_c - 'A';
            char cipher_char = key[index];
            cout << c << " -> " << cipher_char << "\n";
            ciphertext += cipher_char;
        }
        else
        {
            cout << c << " (non-alphabetic, unchanged)\n";
            ciphertext += c;
        }
    }

    clock_t end = clock(); // End time
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nCiphertext: " << ciphertext << "\n";

    // Memory estimation
    size_t memoryUsed = estimateMemoryUsage(plaintext.length());

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << execTime << " seconds\n";
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    cout << "\n--- Cryptanalysis Overview ---\n";
    cout << "1. Monoalphabetic substitution is vulnerable to frequency analysis.\n";
    cout << "2. Common letters (E, T, A, O) and digrams (TH, HE, IN) can be used to break it.\n";
    cout << "3. Known plaintext or crib attacks make it trivially breakable.\n";
    cout << "4. Security improves with polyalphabetic or randomized keys and key rotation.\n";

    return 0;
}
