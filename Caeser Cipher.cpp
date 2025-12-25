#include <iostream>
#include <ctime>
#include <unordered_map>
using namespace std;

// Caesar cipher encryption with round-wise output and cryptanalysis
string caesarEncrypt(string text, int key)
{
    string result = "";
    cout << "\n-- Encryption Process --\n";
    for (size_t i = 0; i < text.size(); ++i)
    {
        char c = text[i];
        cout << " Round " << (i + 1) << ": Processing '" << c << "' -> ";
        if (c >= 'A' && c <= 'Z')
        {
            result += char((c - 'A' + key) % 26 + 'A');
            cout << "'" << result.back() << "' (shifted with key " << key << ")";
        }
        else if (c >= 'a' && c <= 'z')
        {
            result += char((c - 'a' + key) % 26 + 'a');
            cout << "'" << result.back() << "' (shifted with key " << key << ")";
        }
        else
        {
            result += c;
            cout << "'" << c << "' (non-alphabetic, unchanged)";
        }
        cout << endl;
    }
    return result;
}

// Function to calculate the character frequencies (for simple cryptanalysis)
void analyzeFrequency(string text)
{
    unordered_map<char, int> freqMap;
    for (char c : text)
    {
        if (isalpha(c))
        {
            freqMap[tolower(c)]++;
        }
    }

    cout << "\n[Character Frequency Analysis]\n";
    for (auto &pair : freqMap)
    {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int main()
{
    string plaintext;
    int key;

    // User Input
    cout << "Enter plaintext: ";
    getline(cin, plaintext);
    cout << "Enter key (shift value): ";
    cin >> key;

    // Measure execution time
    clock_t start = clock();

    // Encryption
    string encrypted = caesarEncrypt(plaintext, key);
    cout << "\nEncrypted text: " << encrypted << endl;

    // Measure end time
    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;
    cout << "\nExecution Time: " << execTime << " seconds" << endl;

    // Memory Usage Estimation (basic approach)
    int memoryUsed = sizeof(plaintext) + sizeof(encrypted) + sizeof(key);
    cout << "Estimated Memory Usage: " << memoryUsed << " bytes" << endl;

    // Perform basic cryptanalysis (frequency analysis)
    analyzeFrequency(encrypted);

    return 0;
}
