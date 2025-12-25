#include <iostream>
using namespace std;
int getLength(const char* str) {
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}
void shiftCipher(const char* input, int key, char* output, bool encrypt) {
    key = key % 26;
    if (!encrypt) key = 26 - key;
    for (int i = 0; input[i] != '\0'; i++) {
        char ch = input[i];
        if (ch >= 'A' && ch <= 'Z')
            output[i] = ((ch - 'A' + key) % 26) + 'A';
        else if (ch >= 'a' && ch <= 'z')
            output[i] = ((ch - 'a' + key) % 26) + 'a';
        else
            output[i] = ch;
    }
}
void terminate(char* str, int len) {
    str[len] = '\0';
}
int main() {
    const int MAX = 100;
    char msg[MAX], cipher[MAX], decrypted[MAX];
    int key;
    cout << "Enter message: ";
    cin.getline(msg, MAX);
    cout << "Enter key (0-25): ";
    cin >> key;
    int len = getLength(msg);
    shiftCipher(msg, key, cipher, true);
    terminate(cipher, len);
    cout << "Encrypted: " << cipher << endl;
    shiftCipher(cipher, key, decrypted, false);
    terminate(decrypted, len);
    cout << "Decrypted: " << decrypted << endl;
    return 0;
}