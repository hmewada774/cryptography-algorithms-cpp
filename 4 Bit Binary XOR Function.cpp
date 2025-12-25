#include <iostream>
#include <chrono>
#include <fstream>
#include <string>

void printMemoryUsage()
{
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line))
    {
        if (line.substr(0, 6) == "VmRSS:")
        {
            std::cout << "Memory Usage (RSS): " << line.substr(6) << std::endl;
            break;
        }
    }
}

int main()
{
    char input1[5], input2[5], result[5];

    auto start = std::chrono::high_resolution_clock::now(); // Start time

    std::cout << "Enter first 4-bit binary string: ";
    std::cin >> input1;
    std::cout << "Enter second 4-bit binary string: ";
    std::cin >> input2;

    for (int i = 0; i < 4; ++i)
    {
        if (input1[i] == input2[i])
            result[i] = '0';
        else
            result[i] = '1';
    }
    result[4] = '\0';

    std::cout << "XOR Result: " << result << std::endl;

    auto end = std::chrono::high_resolution_clock::now(); // End time
    std::chrono::duration<double, std::micro> exec_time = end - start;

    std::cout << "Execution Time: " << exec_time.count() << " microseconds" << std::endl;

    printMemoryUsage(); // Memory usage

    return 0;
}
