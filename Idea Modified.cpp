#include <iostream>
#include <ctime>
#include <iomanip>
using namespace std;

int modMul(int a, int b)
{
    long long result = (long long)a * b;
    return (int)(result % 65537);
}

int modAdd(int a, int b)
{
    return (a + b) % 65536;
}

void ideaRound(int &A, int &B, int &C, int &D, int key[], int round)
{
    int a1 = modMul(A, key[0]);
    int b1 = modAdd(B, key[1]);
    int c1 = modAdd(C, key[2]);
    int d1 = modMul(D, key[3]);

    A = a1;
    B = b1;
    C = c1;
    D = d1;

    cout << "Round " << round + 1 << ": ";
    cout << "A=" << A << " B=" << B << " C=" << C << " D=" << D << endl;
}

string encryptBlock(string block, int key[])
{
    int A = (block[0] << 8) + block[1];
    int B = (block[2] << 8) + block[3];
    int C = (block[0] << 4) + block[2];
    int D = (block[1] << 4) + block[3];

    cout << "Initial: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n";

    for (int i = 0; i < 4; ++i)
        ideaRound(A, B, C, D, key, i);

    string cipher = "";
    cipher += (char)(A >> 8);
    cipher += (char)(A & 0xFF);
    cipher += (char)(B >> 8);
    cipher += (char)(B & 0xFF);
    return cipher;
}

size_t estimateMemory()
{
    return sizeof(int) * 4 + sizeof(int) * 4 + sizeof(char) * 4;
}

int main()
{
    string plaintext = "TEST"; // 4 chars = 32 bits
    int key[4] = {12345, 23456, 34567, 45678};

    clock_t start = clock();
    string encrypted = encryptBlock(plaintext, key);
    clock_t end = clock();

    double execTime = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nEncrypted Bytes: ";
    for (char c : encrypted)
        cout << (int)(unsigned char)c << " ";
    cout << endl;

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << fixed << execTime << " seconds\n";
    cout << "Estimated Memory Used: " << estimateMemory() << " bytes\n";

    cout << "\n--- Cryptanalysis Summary ---\n";
    cout << "1. This is a simplified IDEA-like cipher with weak diffusion and no XOR or mixing layers.\n";
    cout << "2. The use of mod 65537 multiplication is cryptographically interesting, but insufficient alone.\n";
    cout << "3. Key schedule is static; real IDEA uses 52 subkeys derived via shifting.\n";
    cout << "4. No inversion/decryption supported — cannot assess reversibility.\n";
    cout << "5. This cipher can be broken with differential analysis due to weak substitution/permutation.\n";

    return 0;
}
