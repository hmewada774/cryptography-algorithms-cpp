#include <iostream>
#include <string>
#include <ctime>
using namespace std;

string encrypt(string data, string key)
{
    string result = data;
    for (int i = 0; i < data.length(); ++i)
    {
        result[i] ^= key[i % key.length()];
        cout << "Encrypt Round " << i + 1 << ": '" << data[i] << "' XOR '" << key[i % key.length()] << "' = '" << result[i] << "'\n";
    }
    return result;
}

string decrypt(string data, string key)
{
    cout << "--- Decryption Start ---\n";
    string result = encrypt(data, key); // Same as encrypt in XOR
    cout << "--- Decryption End ---\n";
    return result;
}

string getTGT(string clientID, string clientKey, string tgsKey)
{
    cout << "\n[Generating TGT]\n";
    string sessionKey = "CSKEY";
    string ticket = encrypt(clientID + sessionKey, tgsKey);
    string message = encrypt(sessionKey + ticket, clientKey);
    return message;
}

string getServiceTicket(string tgt, string clientID, string tgsSessionKey, string serverKey)
{
    cout << "\n[Requesting Service Ticket]\n";
    string decryptedTGT = decrypt(tgt, "TGSKEY");
    string sessionKey = "CSSKEY";
    string ticket = encrypt(clientID + sessionKey, serverKey);
    string message = encrypt(sessionKey + ticket, tgsSessionKey);
    return message;
}

string accessService(string ticket, string auth, string serverKey)
{
    cout << "\n[Accessing Service]\n";
    string decryptedTicket = decrypt(ticket, serverKey);
    string decryptedAuth = decrypt(auth, "CSSKEY");
    cout << "Decrypted Ticket: " << decryptedTicket << "\n";
    cout << "Decrypted Authenticator: " << decryptedAuth << "\n";

    if (decryptedTicket.substr(0, 6) == decryptedAuth.substr(0, 6))
        return "Access Granted!";
    return "Access Denied!";
}

size_t estimateMemoryUsage()
{
    return sizeof(string) * 10 + sizeof(char) * 100;
}

int main()
{
    string clientID = "Alice";
    string clientKey = "ALKEY";
    string tgsKey = "TGSKEY";
    string serverKey = "SRVKEY";

    clock_t start = clock();

    string encryptedTGT = getTGT(clientID, clientKey, tgsKey);
    string decrypted = decrypt(encryptedTGT, clientKey);
    string tgsSessionKey = decrypted.substr(0, 5);
    string ticketTGS = decrypted.substr(5);

    string encryptedServiceTicket = getServiceTicket(ticketTGS, clientID, tgsSessionKey, serverKey);
    string decrypted2 = decrypt(encryptedServiceTicket, tgsSessionKey);
    string clientServerSessionKey = decrypted2.substr(0, 6);
    string ticketSRV = decrypted2.substr(6);

    string authenticator = encrypt(clientID + clientServerSessionKey, clientServerSessionKey);
    string result = accessService(ticketSRV, authenticator, serverKey);

    clock_t end = clock();
    double execTime = double(end - start) / CLOCKS_PER_SEC;

    cout << "\n--- Final Result ---\n";
    cout << result << endl;

    cout << "\n--- Performance Metrics ---\n";
    cout << "Execution Time: " << fixed << execTime << " seconds\n";
    cout << "Estimated Memory Used: " << estimateMemoryUsage() << " bytes\n";

    cout << "\n--- Cryptanalysis Overview ---\n";
    cout << "1. XOR encryption is symmetric and very weak — vulnerable to known plaintext attacks.\n";
    cout << "2. No message integrity checks like MACs or timestamps — replay attacks possible.\n";
    cout << "3. Session keys are static strings — predictable and insecure.\n";
    cout << "4. Real Kerberos uses timestamps, nonces, and encryption with strong algorithms like AES.\n";

    return 0;
}
