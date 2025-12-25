#include <iostream>
#include <chrono>
#include <fstream>
using namespace std;

int gcd(int x, int y)
{
    while (y != 0)
    {
        int temp = x % y;
        x = y;
        y = temp;
    }
    return x;
}

void printMemoryUsage()
{
    ifstream status("/proc/self/status");
    string line;
    while (getline(status, line))
    {
        if (line.substr(0, 6) == "VmRSS:")
            cout << "Memory Usage (RSS):" << line.substr(6) << endl;
    }
}

int main()
{
    cout << "=== Affine Cipher Demonstration ===\n\n";
    char plaintext[100];
    int a, b;

    cout << "Step 1: Input Data\n";
    cout << "Enter the plaintext (uppercase, no spaces): ";
    cin >> plaintext;
    cout << "Enter key a (must be coprime with 26, e.g., 1, 3, 5, 7, etc.): ";
    cin >> a;
    cout << "Enter key b (any integer from 0 to 25): ";
    cin >> b;

    cout << "\nPlaintext: " << plaintext << "\n";
    cout << "Keys: a = " << a << ", b = " << b << "\n\n";

    if (gcd(a, 26) != 1)
    {
        cout << "Error: Key 'a' = " << a << " is not coprime with 26. Please use a valid key.\n";
        return 1;
    }

    int length = 0;
    while (plaintext[length] != '\0')
    {
        length++;
    }

    int plaintextNums[100];
    int cipherNums[100];
    char ciphertext[100];

    auto start = chrono::high_resolution_clock::now();

    cout << "Step 2: Processing Each Character using Affine Transformation:\n";
    for (int i = 0; i < length; i++)
    {
        char currentChar = plaintext[i];
        int pVal = currentChar - 'A';
        plaintextNums[i] = pVal;
        int cVal = (a * pVal + b) % 26;
        cipherNums[i] = cVal;
        char cipherChar = cVal + 'A';
        ciphertext[i] = cipherChar;
        cout << "For character '" << currentChar << "' (value " << pVal << "): ";
        cout << "Calculation -> (" << a << " * " << pVal << " + " << b << ") mod 26 = "
             << cVal << " => '" << cipherChar << "'\n";
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> exec_time = end - start;

    ciphertext[length] = '\0';

    cout << "\nStep 3: Final Ciphertext: " << ciphertext << "\n";
    cout << "\nExecution Time: " << exec_time.count() << " microseconds\n";
    printMemoryUsage();

    // Cryptanalysis
    cout << "\n=== Cryptanalysis Insight ===\n";
    cout << "- The Affine Cipher is vulnerable to frequency analysis.\n";
    cout << "- Since only 12 valid 'a' values exist (coprime with 26), brute force is trivial.\n";
    cout << "- For each of 12 values of 'a' and 26 values of 'b', attacker can try all 312 combos.\n";
    cout << "- Not secure for modern cryptographic use — only for educational or puzzle purposes.\n";

    return 0;
}
