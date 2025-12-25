#include <iostream>
#include <ctime>
using namespace std;

char matrix[5][5];

bool isInKey(char ch, char key[], int len)
{
    for (int i = 0; i < len; ++i)
    {
        if (key[i] == ch)
            return true;
    }
    return false;
}

void generateMatrix(char keyword[])
{
    char key[25];
    int klen = 0;
    for (int i = 0; keyword[i] != '\0'; ++i)
    {
        char ch = keyword[i];
        if (ch >= 'a' && ch <= 'z')
            ch -= 32; // Convert to uppercase
        if (ch == 'J')
            ch = 'I'; // Treat 'J' as 'I'
        if (ch >= 'A' && ch <= 'Z' && !isInKey(ch, key, klen))
        {
            key[klen++] = ch;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch)
    {
        if (ch == 'J')
            continue; // Skip 'J'
        if (!isInKey(ch, key, klen))
        {
            key[klen++] = ch;
        }
    }
    int idx = 0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            matrix[i][j] = key[idx++];
}

void findPos(char ch, int &row, int &col)
{
    if (ch == 'J')
        ch = 'I'; // Treat 'J' as 'I'
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (matrix[i][j] == ch)
            {
                row = i;
                col = j;
                return;
            }
}

void encrypt(char plaintext[], char ciphertext[])
{
    char clean[100];
    int len = 0;
    for (int i = 0; plaintext[i] != '\0'; ++i)
    {
        char ch = plaintext[i];
        if (ch >= 'a' && ch <= 'z')
            ch -= 32; // Convert to uppercase
        if (ch < 'A' || ch > 'Z')
            continue; // Skip non-alphabetic characters
        if (ch == 'J')
            ch = 'I'; // Treat 'J' as 'I'
        clean[len++] = ch;
    }
    char digraph[100];
    int dlen = 0;
    for (int i = 0; i < len; ++i)
    {
        digraph[dlen++] = clean[i];
        if (i + 1 < len)
        {
            if (clean[i] == clean[i + 1])
            {
                digraph[dlen++] = 'X'; // Insert 'X' if same character
            }
            else
            {
                digraph[dlen++] = clean[++i];
            }
        }
    }
    if (dlen % 2 != 0)
    {
        digraph[dlen++] = 'X'; // Append 'X' if odd number of characters
    }

    int c = 0;
    for (int i = 0; i < dlen; i += 2)
    {
        int r1, c1, r2, c2;
        findPos(digraph[i], r1, c1);
        findPos(digraph[i + 1], r2, c2);

        cout << "Processing digraph: " << digraph[i] << digraph[i + 1] << endl;

        if (r1 == r2)
        {
            ciphertext[c++] = matrix[r1][(c1 + 1) % 5]; // Same row
            ciphertext[c++] = matrix[r2][(c2 + 1) % 5];
        }
        else if (c1 == c2)
        {
            ciphertext[c++] = matrix[(r1 + 1) % 5][c1]; // Same column
            ciphertext[c++] = matrix[(r2 + 1) % 5][c2];
        }
        else
        {
            ciphertext[c++] = matrix[r1][c2]; // Rectangle rule
            ciphertext[c++] = matrix[r2][c1];
        }
    }
    ciphertext[c] = '\0';
}

int main()
{
    char keyword[50];
    char plaintext[100];
    char ciphertext[100];

    cout << "Enter keyword (no spaces): ";
    cin >> keyword;
    cout << "Enter plaintext: ";
    cin.ignore();
    cin.getline(plaintext, 100);

    // Start execution time tracking
    clock_t start = clock();

    generateMatrix(keyword);
    encrypt(plaintext, ciphertext);

    // End execution time tracking
    clock_t end = clock();

    cout << "Encrypted text: " << ciphertext << endl;

    // Performance Metrics
    double executionTime = double(end - start) / CLOCKS_PER_SEC;
    size_t memoryUsed = sizeof(plaintext) + sizeof(ciphertext) + sizeof(matrix) + sizeof(keyword);

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << executionTime << " seconds\n";
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes\n";

    // Cryptanalysis Discussion
    cout << "\n--- Cryptanalysis ---\n";
    cout << "1. Playfair Cipher is more secure than Caesar cipher but still vulnerable to frequency analysis.\n";
    cout << "2. The same digraphs (pairs of letters) may reveal patterns in the ciphertext.\n";
    cout << "3. Known-plaintext attacks and frequency analysis can be used to break the cipher.\n";
    cout << "4. To improve security, the Playfair cipher can be combined with other ciphers (e.g., AES).\n";

    return 0;
}
