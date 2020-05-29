#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

void ReadMapCSV(const std::string& filename, std::vector<int>& output)
{
    std::ifstream fin(filename);
    std::string line;

    if (!fin.is_open())
    {
        std::cout << "MAP FILE NOT FOUND" << std::endl;
    }

    while (!fin.eof())
    {
        getline(fin, line);
        std::stringstream ssLine(line);
        char ch;
        int num;
        for (int i = 0; i < 100; i++)
        {
            while (ssLine >> num) // read a number
            {
                output.emplace_back(num); // store the number 
                ssLine >> ch; // read and discard the comma
            }

        }
    }
    fin.close();
}