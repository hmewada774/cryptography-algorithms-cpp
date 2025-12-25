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
            std::cout << "Memory Usage (RSS):" << line.substr(6) << std::endl;
            break;
        }
    }
}

int main()
{
    char input1[7], input2[7], result[7];

    auto start = std::chrono::high_resolution_clock::now(); // Start timer

    std::cout << "Enter first 6-bit binary string: ";
    std::cin >> input1;
    std::cout << "Enter second 6-bit binary string: ";
    std::cin >> input2;

    for (int i = 0; i < 6; ++i)
    {
        result[i] = (input1[i] == input2[i]) ? '0' : '1';
    }
    result[6] = '\0';

    std::cout << "XOR Result: " << result << std::endl;

    auto end = std::chrono::high_resolution_clock::now(); // End timer
    std::chrono::duration<double, std::micro> exec_time = end - start;

    std::cout << "Execution Time: " << exec_time.count() << " microseconds" << std::endl;

    printMemoryUsage(); // Print memory usage

    return 0;
}
