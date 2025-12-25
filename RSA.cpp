#include <iostream>
#include <ctime> // For timing
using namespace std;

// Function to calculate GCD (Greatest Common Divisor)
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to find modular inverse (using brute force method)
int modInverse(int e, int phi)
{
    for (int d = 1; d < phi; ++d)
    {
        if ((d * e) % phi == 1)
            return d;
    }
    return -1;
}

// Function to compute (base^exp) % mod using binary exponentiation
int modPow(int base, int exp, int mod)
{
    int result = 1;
    base %= mod;
    while (exp > 0)
    {
        if (exp % 2)
        {
            result = (result * base) % mod;
            cout << "modPow: result = " << result << endl; // Output of each round
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int main()
{
    // Timer Start
    clock_t startTime, endTime;
    startTime = clock();

    int p = 3, q = 11;           // Two small prime numbers for simplicity
    int n = p * q;               // Modulus for public and private keys
    int phi = (p - 1) * (q - 1); // Euler's totient function

    // Find 'e' such that 1 < e < phi and gcd(e, phi) = 1
    int e = 7;
    while (gcd(e, phi) != 1)
        ++e; // Ensure 'e' is coprime with phi

    // Find 'd' such that (e * d) % phi = 1 (modular inverse)
    int d = modInverse(e, phi);

    // Input message as number less than n
    int msg;
    cout << "Enter message (as number < " << n << "): ";
    cin >> msg;

    // Encryption: cipher = msg^e % n
    int cipher = modPow(msg, e, n);
    cout << "Encrypted message: " << cipher << endl;

    // Decryption: decrypted = cipher^d % n
    int decrypted = modPow(cipher, d, n);
    cout << "Decrypted message: " << decrypted << endl;

    // Output public and private keys
    cout << "\nPublic Key: (" << e << ", " << n << ")";
    cout << "\nPrivate Key: (" << d << ", " << n << ")";

    // Timer End
    endTime = clock();
    double elapsedTime = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "\nTotal Execution Time: " << elapsedTime << " seconds" << endl;

    // Memory Usage Estimation
    size_t memoryUsed = sizeof(p) + sizeof(q) + sizeof(n) + sizeof(phi) + sizeof(e) + sizeof(d) + sizeof(msg) + sizeof(cipher) + sizeof(decrypted);
    cout << "Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis Discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. RSA encryption is based on the difficulty of factoring large composite numbers.\n";
    cout << "2. With small values of p and q, RSA is very weak as it is vulnerable to **brute-force attacks**.\n";
    cout << "3. For RSA to be secure, large primes (hundreds of digits) are used to make factoring infeasible.\n";
    cout << "4. **Public key** (e, n) is shared, and **private key** (d, n) is kept secret.\n";
    cout << "5. RSA relies on the **factoring problem**, which is computationally expensive for large numbers.\n";
    cout << "6. The security of RSA can be improved by using larger keys (e.g., 2048-bit or 4096-bit keys).\n";

    return 0;
}
