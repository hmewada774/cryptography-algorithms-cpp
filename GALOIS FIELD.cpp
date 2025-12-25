#include <iostream>
#include <ctime>
#include <bitset>
using namespace std;

// Multiply two bytes in GF(2^8)
unsigned char gfMul(unsigned char a, unsigned char b)
{
    unsigned char result = 0;
    unsigned char irreducible = 0x1B;

    cout << "\n--- GF(2^8) Multiplication Rounds ---\n";

    for (int i = 0; i < 8; ++i)
    {
        cout << "Round " << i + 1 << ": ";
        cout << "a = " << bitset<8>(a) << ", b = " << bitset<8>(b) << ", result = " << bitset<8>(result) << endl;

        if (b & 1)
            result ^= a;

        bool carry = a & 0x80;
        a <<= 1;
        if (carry)
            a ^= irreducible;

        b >>= 1;
    }

    cout << "Final result: " << bitset<8>(result) << "\n";
    return result;
}

// Estimate memory used (rough approximation)
size_t estimateMemory()
{
    size_t mem = sizeof(unsigned char) * 4; // a, b, result, irreducible
    mem += sizeof(bool);                    // carry
    return mem;
}

int main()
{
    unsigned char a = 0x57; // Example input
    unsigned char b = 0x13;

    // Start time measurement
    clock_t start = clock();

    // Perform multiplication
    unsigned char product = gfMul(a, b);

    // End time measurement
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    // Output result
    cout << "\nGF(2^8) Multiply: ";
    cout << hex << (int)a << " × " << (int)b << " = " << (int)product << endl;

    // Show execution time and memory usage
    cout << "Execution Time: " << execTime << " seconds\n";
    cout << "Approximate Memory Used: " << estimateMemory() << " bytes\n";

    // Cryptanalysis Notes:
    cout << "\n--- Cryptanalysis Insight ---\n";
    cout << "1. GF(2^8) multiplication is non-linear and invertible.\n";
    cout << "2. Used in AES MixColumns step for diffusion.\n";
    cout << "3. Key observation: knowledge of input-output pairs does not reveal full mapping.\n";
    cout << "4. Try brute-force all 256 x 256 combinations to build a full multiplication table if needed.\n";

    return 0;
}
