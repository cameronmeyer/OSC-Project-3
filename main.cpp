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

const string whitespace = "\n\t\r\v\f";      // Whitespace characters for parsing the input file

vector<tuple<string, int, int>> jobs; 

vector<int> getIntFromString(string s)
{
    string number = s.substr(0, s.find(whitespace)); // Only use the beginning of the line until whitespace is found
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
            int index = 0;
            string entry = "";
            vector<string> line;

            for(char c : input)
            {
                //cout << "1 -- c: " << c << endl;
                if(count < 3) // Each line should have 3 elements we need to keep track of
                {
                    //cout << "2" << endl;
                    if(whitespace.find(c) != string::npos) // Check if the current char is a whitespace
                    {
                        //cout << "3" << endl;
                        if(entry != "") // If entry contains something, this whitespace is the end of the entry
                        {
                            //cout << "4" << endl;
                            line.push_back(entry);
                            count++;
                            entry = "";
                        }
                    }
                    //else if(input.substr(index, input.size()).find(c) == input.size() - 1)  // THIS IS ALWAYS GOING TO FIND IT AT THE BEGINNING OF THE SUBSTRING, WE JUST NEED THE LAST OCCURRENCE
                    else if(input.find_last_of(c) == input.size() - 1)
                    {
                        //cout << "7" << endl;
                        entry = entry + c;
                        line.push_back(entry);
                        count++;
                        entry = "";
                    }
                    else // Add non-whitespace characters to the entry
                    {
                        //cout << "5" << endl;
                        entry = entry + c;
                    }

                    //cout << "c at end of string: " << input.find(c) << endl;
                }
                //cout << "input.substr(index, input.size()).find(c) " << input.substr(index, input.size()).find(c) << endl;
                index++;
            }

            //cout << "line size: " << line.size() << endl;
            if(line.size() == 3)
            {
                //cout << "6" << endl;
                tuple <string, int, int> job;
                job = make_tuple(line[0], stoi(line[1]), stoi(line[2]));
                //job = make_tuple(line[0], getIntFromString(line[1]), getIntFromString(line[2]));
                jobs.push_back(job);
            }

            //if(input.substr(0, 1) == ".")
            //{
            //    string errorMessage = "Error: Invalid memory write at address: " + to_string(tempAddress);
            //    throw out_of_range(errorMessage);
            //    exit(-1);
            //}
            //cout << "count: " << count << endl;
        }

        cout << "jobs size: " << jobs.size() << endl;

        for(auto job : jobs)
        {
            cout << "job num elements: " << tuple_size<decltype(job)>::value << endl;
            //printf(get<0>(job), "\n");
            cout << get<0>(job) << " " << get<1>(job) << " " << get<2>(job) << endl;
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