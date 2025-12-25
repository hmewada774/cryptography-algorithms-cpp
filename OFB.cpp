#include <iostream>
#include <string>
#include <ctime> // for timing
using namespace std;

int feistel(int half, int key)
{
    return (half ^ key) & 0xF;
}

int encryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF;
    int R = block & 0xF;
    int keys[4] = {k1, k2, k3, k4};

    cout << "EncryptBlock: Initial L = " << L << ", R = " << R << endl;
    for (int i = 0; i < 4; ++i)
    {
        int temp = R;
        int f_out = feistel(R, keys[i]);
        R = L ^ f_out;
        L = temp;
        cout << " Round " << i + 1 << ": L = " << L << ", R = " << R << " (F = " << f_out << ", K = " << keys[i] << ")\n";
    }
    return (L << 4) | R;
}

int decryptBlock(int block, int k1, int k2, int k3, int k4)
{
    int L = (block >> 4) & 0xF;
    int R = block & 0xF;
    int keys[4] = {k4, k3, k2, k1};

    for (int i = 0; i < 4; ++i)
    {
        int temp = L;
        L = R ^ feistel(L, keys[i]);
        R = temp;
    }
    return (L << 4) | R;
}

string ofbEncrypt(string plaintext, int iv, int k1, int k2, int k3, int k4)
{
    string ciphertext = "";
    int feedback = iv;

    cout << "\n--- OFB Encryption Steps ---\n";
    for (int i = 0; i < plaintext.size(); ++i)
    {
        char c = plaintext[i];
        int encFeedback = encryptBlock(feedback, k1, k2, k3, k4);
        int enc = c ^ encFeedback;
        ciphertext += (char)enc;
        cout << "Char[" << i << "] '" << c << "' XOR " << encFeedback << " => " << (int)(unsigned char)enc << "\n";
        feedback = encFeedback;
    }
    return ciphertext;
}

string ofbDecrypt(string ciphertext, int iv, int k1, int k2, int k3, int k4)
{
    string plaintext = "";
    int feedback = iv;

    cout << "\n--- OFB Decryption Steps ---\n";
    for (int i = 0; i < ciphertext.size(); ++i)
    {
        char c = ciphertext[i];
        int encFeedback = encryptBlock(feedback, k1, k2, k3, k4);
        int dec = c ^ encFeedback;
        plaintext += (char)dec;
        cout << "Char[" << i << "] " << (int)(unsigned char)c << " XOR " << encFeedback << " => '" << dec << "'\n";
        feedback = encFeedback;
    }
    return plaintext;
}

size_t estimateMemoryUsage(int textLength)
{
    return sizeof(char) * (textLength * 2) + sizeof(int) * 5; // ciphertext + plaintext + keys
}

int main()
{
    string text;
    cout << "Enter plaintext: ";
    getline(cin, text);

    int iv = 99;
    int k1 = 5, k2 = 9, k3 = 3, k4 = 12;

    clock_t start = clock(); // Execution time starts

    string encrypted = ofbEncrypt(text, iv, k1, k2, k3, k4);
    string decrypted = ofbDecrypt(encrypted, iv, k1, k2, k3, k4);

    clock_t end = clock(); // Execution time ends
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    size_t memoryUsed = estimateMemoryUsage(text.size());

    cout << "\nEncrypted (raw bytes): ";
    for (char c : encrypted)
        cout << (int)(unsigned char)c << " ";

    cout << "\nDecrypted: " << decrypted << endl;

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << execTime << " seconds\n";
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    cout << "\n--- Cryptanalysis Discussion ---\n";
    cout << "1. OFB (Output Feedback) is a stream cipher mode, so it doesn't propagate errors.\n";
    cout << "2. As long as the IV is not reused, OFB is semantically secure.\n";
    cout << "3. Weaknesses in the Feistel round function (like simple XOR) can lead to predictability.\n";
    cout << "4. Ensure keys and IVs are never reused, and Feistel rounds use strong S-box-like functions.\n";
    cout << "5. Short block sizes (like 8-bit blocks) are vulnerable to brute-force and pattern attacks.\n";

    return 0;
}
