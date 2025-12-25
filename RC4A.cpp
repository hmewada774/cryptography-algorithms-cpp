#include <iostream>
#include <ctime> // For timing
using namespace std;

const int N = 256;

void KSA(unsigned char *S, unsigned char *key, int keyLen)
{
    for (int i = 0; i < N; ++i)
        S[i] = i;

    int j = 0;
    for (int i = 0; i < N; ++i)
    {
        j = (j + S[i] + key[i % keyLen]) % N;
        swap(S[i], S[j]);
    }
}

void RC4A(unsigned char *key, int keyLen, unsigned char *data, int dataLen, unsigned char *output)
{
    unsigned char S1[N], S2[N];
    KSA(S1, key, keyLen);
    KSA(S2, key, keyLen);

    int i1 = 0, j1 = 0;
    int i2 = 0, j2 = 0;

    for (int n = 0; n < dataLen; ++n)
    {
        // Encrypt/Decrypt with RC4A
        i1 = (i1 + 1) % N;
        j1 = (j1 + S1[i1]) % N;
        swap(S1[i1], S1[j1]);
        unsigned char k1 = S2[(S1[i1] + S1[j1]) % N];

        i2 = (i2 + 1) % N;
        j2 = (j2 + S2[i2]) % N;
        swap(S2[i2], S2[j2]);
        unsigned char k2 = S1[(S2[i2] + S2[j2]) % N];

        output[n] = data[n] ^ ((n % 2 == 0) ? k1 : k2);

        // Print state after each round for analysis
        cout << "Round " << n + 1 << ": ";
        cout << "S1[" << i1 << "] = " << (int)S1[i1] << ", S2[" << i2 << "] = " << (int)S2[i2]
             << ", Output: " << (int)output[n] << ", Data: " << (int)data[n] << endl;
    }
}

int main()
{
    unsigned char key[] = {'K', 'E', 'Y'};
    int keyLen = 3;

    unsigned char data[] = "HELLO_RC4A";
    int dataLen = 10;
    unsigned char encrypted[10];
    unsigned char decrypted[10];

    // Measure execution time for encryption
    clock_t start = clock();
    RC4A(key, keyLen, data, dataLen, encrypted);
    clock_t end = clock();
    double encryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nEncryption Time: " << encryptionTime << " seconds" << endl;

    // Measure execution time for decryption
    start = clock();
    RC4A(key, keyLen, encrypted, dataLen, decrypted);
    end = clock();
    double decryptionTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "Decryption Time: " << decryptionTime << " seconds" << endl;

    // Print results
    cout << "\nOriginal : ";
    for (int i = 0; i < dataLen; ++i)
        cout << data[i];
    cout << "\nEncrypted: ";
    for (int i = 0; i < dataLen; ++i)
        cout << hex << (int)encrypted[i] << " ";
    cout << "\nDecrypted: ";
    for (int i = 0; i < dataLen; ++i)
        cout << decrypted[i];
    cout << endl;

    // Memory usage estimation (in bytes)
    size_t memoryUsed = sizeof(key) + sizeof(data) + sizeof(encrypted) + sizeof(decrypted) + sizeof(keyLen) + sizeof(dataLen);
    cout << "\nMemory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. RC4A is a **stream cipher** based on the **RC4 algorithm**, which is known for being fast and simple.\n";
    cout << "2. The RC4 algorithm is vulnerable to several types of cryptanalytic attacks, such as **key recovery** attacks and **biases in keystream generation**.\n";
    cout << "3. RC4A improves upon RC4 by introducing two key streams (S1 and S2) and alternating between them.\n";
    cout << "4. While this modification may improve security against some attacks, it still suffers from biases when not properly initialized or when the key size is too small.\n";
    cout << "5. Modern cryptographic systems such as **AES** are much more secure than RC4A and should be preferred for sensitive data.\n";

    return 0;
}
