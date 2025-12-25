#include <iostream>
#include <ctime>
using namespace std;

char toHexChar(int nibble)
{
    if (nibble < 10)
        return '0' + nibble;
    return 'A' + (nibble - 10);
}

void printBlock(const unsigned char block[4], const char *msg)
{
    cout << msg << ": ";
    for (int i = 0; i < 4; i++)
        cout << toHexChar(block[i] >> 4) << toHexChar(block[i] & 0x0F) << " ";
    cout << "\n";
}

unsigned char sbox[16] = {0xE, 0x4, 0xD, 0x1,
                          0x2, 0xF, 0xB, 0x8,
                          0x3, 0xA, 0x6, 0xC,
                          0x5, 0x9, 0x0, 0x7};

void substitute(unsigned char block[4])
{
    cout << "\nSubstitution Step Details:\n";
    for (int i = 0; i < 4; i++)
    {
        unsigned char original = block[i];
        unsigned char upper = (original >> 4) & 0x0F;
        unsigned char lower = original & 0x0F;
        unsigned char newUpper = sbox[upper];
        unsigned char newLower = sbox[lower];
        cout << "Byte " << i << " (" << toHexChar(upper) << toHexChar(lower) << "): ";
        cout << "Upper: " << toHexChar(upper) << " -> " << toHexChar(newUpper) << ", ";
        cout << "Lower: " << toHexChar(lower) << " -> " << toHexChar(newLower) << "\n";
        block[i] = (newUpper << 4) | newLower;
    }
}

void permuteNibbles(unsigned char block[4])
{
    unsigned char nibbles[8];
    for (int i = 0; i < 4; i++)
    {
        nibbles[2 * i] = (block[i] >> 4) & 0x0F;
        nibbles[2 * i + 1] = block[i] & 0x0F;
    }
    cout << "\nPermutation Step Details:\nOriginal nibbles: ";
    for (int i = 0; i < 8; i++)
        cout << toHexChar(nibbles[i]) << " ";
    cout << "\n";
    int perm[8] = {2, 0, 1, 3, 5, 7, 4, 6};
    unsigned char newNibbles[8];
    for (int i = 0; i < 8; i++)
    {
        newNibbles[i] = nibbles[perm[i]];
        cout << "New pos " << i << " <- " << perm[i] << " (" << toHexChar(nibbles[perm[i]]) << ")\n";
    }
    for (int i = 0; i < 4; i++)
        block[i] = (newNibbles[2 * i] << 4) | newNibbles[2 * i + 1];
}

void keyMixing(unsigned char block[4], const unsigned char key[4])
{
    cout << "\nKey Mixing Details:\n";
    for (int i = 0; i < 4; i++)
    {
        unsigned char original = block[i];
        block[i] ^= key[i];
        cout << "Byte " << i << ": "
             << toHexChar(original >> 4) << toHexChar(original & 0x0F)
             << " XOR "
             << toHexChar(key[i] >> 4) << toHexChar(key[i] & 0x0F)
             << " = "
             << toHexChar(block[i] >> 4) << toHexChar(block[i] & 0x0F) << "\n";
    }
}

void rotateKey(unsigned char key[4])
{
    unsigned char nibbles[8];
    for (int i = 0; i < 4; i++)
    {
        nibbles[2 * i] = (key[i] >> 4) & 0x0F;
        nibbles[2 * i + 1] = key[i] & 0x0F;
    }
    cout << "\nKey Schedule (Rotation):\nOriginal key nibbles: ";
    for (int i = 0; i < 8; i++)
        cout << toHexChar(nibbles[i]) << " ";
    cout << "\n";
    unsigned char temp = nibbles[0];
    for (int i = 0; i < 7; i++)
        nibbles[i] = nibbles[i + 1];
    nibbles[7] = temp;
    cout << "Rotated key nibbles:  ";
    for (int i = 0; i < 8; i++)
        cout << toHexChar(nibbles[i]) << " ";
    cout << "\n";
    for (int i = 0; i < 4; i++)
        key[i] = (nibbles[2 * i] << 4) | nibbles[2 * i + 1];
}

unsigned char hexToByte(char a, char b)
{
    unsigned char val = 0;
    val |= (a >= 'A' ? a - 'A' + 10 : a - '0') << 4;
    val |= (b >= 'A' ? b - 'A' + 10 : b - '0');
    return val;
}

bool isValidHexChar(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'));
}

void readHexBlock(const char *prompt, unsigned char block[4])
{
    char input[9];
    while (true)
    {
        cout << prompt << " (8 hex digits): ";
        cin >> input;
        int len = 0;
        while (input[len])
            len++;
        if (len != 8)
        {
            cout << "Invalid length. Must be 8 hex digits.\n";
            continue;
        }
        bool valid = true;
        for (int i = 0; i < 8; i++)
            if (!isValidHexChar(input[i]))
            {
                cout << "Invalid character: " << input[i] << "\n";
                valid = false;
                break;
            }
        if (!valid)
            continue;
        for (int i = 0; i < 4; i++)
            block[i] = hexToByte(input[2 * i], input[2 * i + 1]);
        break;
    }
}

int hamming(unsigned char a[4], unsigned char b[4])
{
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
        unsigned char diff = a[i] ^ b[i];
        for (int j = 0; j < 8; j++)
            count += (diff >> j) & 1;
    }
    return count;
}

void copyBlock(unsigned char dest[4], const unsigned char src[4])
{
    for (int i = 0; i < 4; i++)
        dest[i] = src[i];
}

void differentialCryptanalysis(const unsigned char original[4], const unsigned char key1[4])
{
    cout << "\n[Differential Cryptanalysis]\n";
    unsigned char base[4], key2[4];
    copyBlock(base, original);
    for (int i = 0; i < 4; i++)
        key2[i] = key1[i];

    keyMixing(base, key1);
    substitute(base);
    permuteNibbles(base);
    rotateKey(key2);
    keyMixing(base, key2);
    substitute(base);
    permuteNibbles(base);

    for (int bit = 0; bit < 32; bit++)
    {
        unsigned char modified[4], result[4];
        copyBlock(modified, original);
        modified[bit / 8] ^= (1 << (bit % 8));
        copyBlock(result, modified);
        for (int i = 0; i < 4; i++)
            key2[i] = key1[i];

        keyMixing(result, key1);
        substitute(result);
        permuteNibbles(result);
        rotateKey(key2);
        keyMixing(result, key2);
        substitute(result);
        permuteNibbles(result);

        int diff = hamming(base, result);
        cout << "Bit flip " << bit << ": " << diff << " bit(s) changed.\n";
    }
}

void linearCryptanalysis(const unsigned char key1[4])
{
    cout << "\n[Linear Cryptanalysis]\n";
    int match = 0;
    for (int i = 0; i < 256; i++)
    {
        unsigned char plain[4] = {0, 0, 0, (unsigned char)i};
        unsigned char block[4], key2[4];
        copyBlock(block, plain);
        for (int j = 0; j < 4; j++)
            key2[j] = key1[j];

        keyMixing(block, key1);
        substitute(block);
        permuteNibbles(block);
        rotateKey(key2);
        keyMixing(block, key2);
        substitute(block);
        permuteNibbles(block);

        int pbit = (plain[3] >> 0) & 1;
        int cbit = (block[3] >> 7) & 1;
        if (pbit == cbit)
            match++;
    }
    double bias = (match - 128) / 128.0;
    cout << "Correlation (Plain[0] vs Cipher[7]): Bias = " << bias << "\n";
}

int main()
{
    unsigned char plaintext[4], key1[4], block[4];

    cout << "=== SIMPLE BLOCK CIPHER ===\n";
    readHexBlock("Enter plaintext", plaintext);
    readHexBlock("Enter key", key1);
    for (int i = 0; i < 4; i++)
        block[i] = plaintext[i];

    clock_t start = clock();

    cout << "\nPlaintext block:\n";
    printBlock(block, "Plaintext");

    cout << "\n--- Round 1 ---\n";
    keyMixing(block, key1);
    printBlock(block, "After Key Mixing (R1)");
    substitute(block);
    printBlock(block, "After Substitution (R1)");
    permuteNibbles(block);
    printBlock(block, "After Permutation (R1)");

    unsigned char key2[4];
    for (int i = 0; i < 4; i++)
        key2[i] = key1[i];
    rotateKey(key2);
    printBlock(key2, "Round 2 Key");

    cout << "\n--- Round 2 ---\n";
    keyMixing(block, key2);
    printBlock(block, "After Key Mixing (R2)");
    substitute(block);
    printBlock(block, "After Substitution (R2)");
    permuteNibbles(block);
    printBlock(block, "After Permutation (R2)");

    clock_t end = clock();
    double timeTaken = double(end - start) / CLOCKS_PER_SEC;

    cout << "\nCiphertext:\n";
    printBlock(block, "Ciphertext");
    cout << "\nExecution Time: " << timeTaken << " seconds\n";

    int memoryUsed = sizeof(plaintext) + sizeof(key1) + sizeof(block) + sizeof(key2);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    differentialCryptanalysis(plaintext, key1);
    linearCryptanalysis(key1);

    return 0;
}
