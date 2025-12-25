#include <iostream>
#include <ctime>
#include <unordered_map>
#include <bitset>
using namespace std;

const int p = 97;
const int a = 2;
const int b = 3;

struct Point
{
    int x;
    int y;
    bool atInfinity;

    Point() : x(0), y(0), atInfinity(true) {}
    Point(int x_, int y_) : x(x_), y(y_), atInfinity(false) {}
};

int modInverse(int k, int mod)
{
    k = k % mod;
    for (int x = 1; x < mod; ++x)
    {
        if ((k * x) % mod == 1)
            return x;
    }
    return -1;
}

Point pointAdd(Point P, Point Q)
{
    if (P.atInfinity)
        return Q;
    if (Q.atInfinity)
        return P;

    if (P.x == Q.x && (P.y + Q.y) % p == 0)
        return Point();

    int lambda;
    if (P.x == Q.x && P.y == Q.y)
    {
        int num = (3 * P.x * P.x + a) % p;
        int den = modInverse(2 * P.y, p);
        if (den == -1)
            return Point();
        lambda = (num * den) % p;
    }
    else
    {
        int num = (Q.y - P.y + p) % p;
        int den = modInverse((Q.x - P.x + p) % p, p);
        if (den == -1)
            return Point();
        lambda = (num * den) % p;
    }

    // Output each round of lambda calculation (acting like S-box in some sense)
    cout << "Lambda (in binary): " << bitset<8>(lambda) << endl;

    int x3 = (lambda * lambda - P.x - Q.x + p) % p;
    int y3 = (lambda * (P.x - x3) - P.y + p) % p;

    cout << "Point Addition result: (" << x3 << ", " << y3 << ")" << endl;

    return Point((x3 + p) % p, (y3 + p) % p);
}

Point scalarMultiply(Point P, int k)
{
    Point R;
    int round = 0;
    while (k > 0)
    {
        round++;
        if (k % 2 == 1)
        {
            cout << "Round " << round << " - Point Addition: ";
            R = pointAdd(R, P);
        }
        P = pointAdd(P, P);
        k /= 2;
    }
    return R;
}

void analyzeFrequency(const Point &p)
{
    unordered_map<int, int> freqMap;
    freqMap[p.x]++;
    freqMap[p.y]++;

    cout << "\n[Point Frequency Analysis of Shared Secret]\n";
    for (auto &pair : freqMap)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main()
{
    clock_t start = clock(); // Start measuring time

    Point G(3, 6);
    int privKey = 7;
    Point pubKey = scalarMultiply(G, privKey);

    cout << "Private Key: " << privKey << endl;
    cout << "Public Key: (" << pubKey.x << ", " << pubKey.y << ")" << endl;

    int k = 5;
    Point shared = scalarMultiply(pubKey, k);
    cout << "Shared Secret (x): " << shared.x << endl;

    clock_t end = clock(); // End measuring time
    double execTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation (basic)
    int memoryUsed = sizeof(G) + sizeof(pubKey) + sizeof(shared) + sizeof(privKey) + sizeof(k);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Cryptanalysis (Frequency analysis of shared secret)
    analyzeFrequency(shared);

    return 0;
}
