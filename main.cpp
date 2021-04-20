// Author: Cameron Meyer (cdm180003)
// Project 3
// CS 4348.001


#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdexcept>
#include <exception>
#include <unistd.h>
#include <cmath>
#include <math.h>
#include <tuple>
using namespace std;

const string space = "\n\t\r\v\f";      // Whitespace characters for parsing the input file

vector<tuple<int, int, int>> jobs; 

vector<int> getIntFromString(string s)
{
    string number = s.substr(0, s.find(space)); // Only use the beginning of the line until whitespace is found
    try
    {
        // Convert string to int
        std::string::size_type sz;
        int result = stoi(number, &sz);
        return vector<int>{1, result}; // Index 0 in this vector indicates a success
    }
    catch(...)
    {
        return vector<int>{-1, 0}; // Index 0 in this vector indicates a failure for int conversion
    }
}

void populateJobs(string fileName)
{
    ifstream file;
    file.open(fileName);
    if(file)
    {
        string input = "";

        // Continuously read each line of input from the file
        while(getline(file, input))
        {
            int count = 0;
            string entry = "";
            vector<string> line;

            for(char c : input)
            {
                if(count < 3) // Each line should have 3 elements we need to keep track of
                {
                    if(space.find(c)) // Check if the current char is a whitespace
                    {
                        if(entry != "") // If entry contains something, this whitespace is the end of the entry
                        {
                            line.push_back(entry);
                            count++;
                            entry = "";
                        }
                    }
                    else // Add non-whitespace characters to the entry
                    {
                        entry = entry + c;
                    }
                }
            }

            if(line.count == 3)
            {
                jobs.push_back(make_tuple(line[0], getIntFromString(line[1]), getIntFromString(line[2])));
            }

            //if(input.substr(0, 1) == ".")
            //{
            //    string errorMessage = "Error: Invalid memory write at address: " + to_string(tempAddress);
            //    throw out_of_range(errorMessage);
            //    exit(-1);
            //}
        }
        file.close();
    }
    else
    {
        string errorMessage = "Error: File " + fileName + " not found.";
        throw invalid_argument(errorMessage);
        exit(-1);
    }
}

int main(int argc, char *argv[])
{
    if(argc > 0)
    {
        // Populate jobs vector with contents from input file
        populateJobs(argv[1]);
    }
}