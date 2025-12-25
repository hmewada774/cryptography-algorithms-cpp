#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include <bitset>
using namespace std;

string simpleDES(string text, string key)
{
    string result = text;
    for (int i = 0; i < text.length(); ++i)
    {
        result[i] = text[i] ^ key[i % key.length()];
        cout << "  S-box Output (Binary) after XOR with Key: " << bitset<8>(result[i]) << endl; // XOR with key and output in binary
    }
    return result;
}

string tripleDESEncrypt(string plaintext, string key1, string key2, string key3)
{
    cout << "\n--- Starting Triple DES Encryption ---\n";

    string step1 = simpleDES(plaintext, key1);
    cout << "Step 1 (Encrypted with Key1): " << bitset<8>(step1[0]) << endl;

    string step2 = simpleDES(step1, key2);
    cout << "Step 2 (Encrypted with Key2): " << bitset<8>(step2[0]) << endl;

    string step3 = simpleDES(step2, key3);
    cout << "Step 3 (Encrypted with Key3): " << bitset<8>(step3[0]) << endl;

    return step3;
}

string tripleDESDecrypt(string ciphertext, string key1, string key2, string key3)
{
    cout << "\n--- Starting Triple DES Decryption ---\n";

    string step1 = simpleDES(ciphertext, key3);
    cout << "Step 1 (Decrypted with Key3): " << bitset<8>(step1[0]) << endl;

    string step2 = simpleDES(step1, key2);
    cout << "Step 2 (Decrypted with Key2): " << bitset<8>(step2[0]) << endl;

    string step3 = simpleDES(step2, key1);
    cout << "Step 3 (Decrypted with Key1): " << bitset<8>(step3[0]) << endl;

    return step3;
}

void analyzeFrequency(string text)
{
    unordered_map<char, int> freqMap;
    for (char c : text)
    {
        freqMap[c]++;
    }

    cout << "\n[Character Frequency Analysis of Ciphertext]\n";
    for (auto &pair : freqMap)
    {
        cout << "'" << pair.first << "': " << pair.second << endl;
    }
}

int main()
{
    string plaintext = "HELLO123"; // Example plaintext
    string key1 = "KEYONE__";      // Key 1 for 3DES
    string key2 = "KEYTWO__";      // Key 2 for 3DES
    string key3 = "KEYTHREE";      // Key 3 for 3DES

    // Measure execution time
    clock_t start = clock();

    // Encrypt the plaintext using Triple DES
    string encrypted = tripleDESEncrypt(plaintext, key1, key2, key3);

    // Decrypt the ciphertext using Triple DES
    string decrypted = tripleDESDecrypt(encrypted, key1, key2, key3);

    // Measure end time
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    // Output encrypted and decrypted results
    cout << "\nEncrypted: ";
    for (char c : encrypted)
        cout << (int)(unsigned char)c << " ";
    cout << "\nDecrypted: " << decrypted << endl;

    // Execution time
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation (basic)
    int memoryUsed = sizeof(plaintext) + sizeof(encrypted) + sizeof(decrypted) + sizeof(key1) + sizeof(key2) + sizeof(key3);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis (Frequency analysis of ciphertext)
    analyzeFrequency(encrypted);

    return 0;
}
