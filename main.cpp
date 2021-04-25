// Author: Cameron Meyer (cdm180003)
// Project 3
// CS 4348.001


#include <stack>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <exception>
#include <unistd.h>
#include <cmath>
#include <math.h>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;

const string whitespace = "\n\t\r\v\f"; // Whitespace characters for parsing the input file

vector<tuple<string, int, int>> jobs; 

bool sortDuration(const tuple<string, int, int>& a, const tuple<string, int, int>& b)
{
    return (get<2>(a) < get<2>(b));
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
                if(count < 3) // Each line should have 3 elements we need to keep track of
                {
                    if(whitespace.find(c) != string::npos) // Check if the current char is a whitespace
                    {
                        if(entry != "") // If entry contains something, this whitespace marks the end of the entry
                        {
                            line.push_back(entry);
                            count++;
                            entry = "";
                        }
                    }
                    else if(index == input.size() - 1 && input.find_last_of(c) == input.size() - 1) // If the string ends with a non-whitespace character
                    {
                        entry = entry + c;
                        line.push_back(entry);
                        count++;
                        entry = "";
                    }
                    else // Add non-whitespace characters to the entry
                    {
                        entry = entry + c;
                    }
                }
                index++;
            }

            if(line.size() == 3)
            {
                tuple <string, int, int> job;
                job = make_tuple(line[0], stoi(line[1]), stoi(line[2]));
                jobs.push_back(job);
            }
        }

        //cout << "jobs size: " << jobs.size() << endl;

        for(auto job : jobs)
        {
            //cout << "job num elements: " << tuple_size<decltype(job)>::value << endl;
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

void printSchedule(string algorithm, vector<string> schedule)
{
    cout << algorithm << endl << endl;
    
    int index = 0;
    if(jobs.size() == schedule.size())
    {
        for(auto job : jobs)
        {
            cout << get<0>(job) << " " << schedule[index] << endl;
            index++;
        }
    }
}

void fcfs()
{
    vector<string> schedule;
    string running = "";

    for(auto job : jobs) { schedule.push_back(""); }

    int completedJobs = 0;
    while(true)
    {
        int jobIndex = 0;

        int runDuration = get<2>(jobs[completedJobs]);
        running = get<0>(jobs[completedJobs]);

        for(auto job : jobs)
        {
            for(int i = 0; i < runDuration; i++)
            {
                if(running == get<0>(job)) { schedule[jobIndex] += "X"; }
                else { schedule[jobIndex] += " "; }
            }
            jobIndex++;
        }
        completedJobs++;

        if(completedJobs >= jobs.size()) { break; }
    }

    printSchedule("FCFS", schedule);
    cout << endl << endl;
}

void spn()
{
    vector<tuple<string, int, int>> spnJobs = jobs;
    vector<string> schedule;
    string running = "";
    int time = 0;

    for(auto job : jobs) { schedule.push_back(""); }

    sort(spnJobs.begin(), spnJobs.end(), sortDuration);

    while(spnJobs.size() > 0)
    {
        int index = 0;
        int jobIndex = 0;
        int runDuration = 0;

        for(int i = 0; i < spnJobs.size(); i++)
        {
            if(get<1>(spnJobs[i]) <= time)
            {
                running = get<0>(spnJobs[i]);
                runDuration = get<2>(spnJobs[i]);
                index = i;
                break;
            }
        }

        spnJobs.erase(spnJobs.begin() + index);
        time += runDuration;

        for(auto job : jobs)
        {
            for(int i = 0; i < runDuration; i++)
            {
                if(running == get<0>(job)) { schedule[jobIndex] += "X"; }
                else { schedule[jobIndex] += " "; }
            }
            jobIndex++;
        }
    }

    printSchedule("SPN", schedule);
    cout << endl << endl;
}

void hrrn()
{
    vector<tuple<string, int, int>> hrrnJobs = jobs;
    vector<string> schedule;
    string running = "";
    int time = 0;

    for(auto job : jobs) { schedule.push_back(""); }

    while(hrrnJobs.size() > 0)
    {
        int index = 0;
        int r = 0;
        int jobIndex = 0;
        int runDuration = 0;

        for(int i = 0; i < hrrnJobs.size(); i++)
        {
            if(get<1>(hrrnJobs[i]) > time) { break; }

            int tempR = ((time - get<1>(hrrnJobs[i])) + get<2>(hrrnJobs[i])) / get<2>(hrrnJobs[i]);
            if(r < tempR)
            {
                r = tempR;
                running = get<0>(hrrnJobs[i]);
                runDuration = get<2>(hrrnJobs[i]);
                index = i;
            }
        }

        hrrnJobs.erase(hrrnJobs.begin() + index);
        time += runDuration;

        for(auto job : jobs)
        {
            for(int i = 0; i < runDuration; i++)
            {
                if(running == get<0>(job)) { schedule[jobIndex] += "X"; }
                else { schedule[jobIndex] += " "; }
            }
            jobIndex++;
        }
    }

    printSchedule("HRRN", schedule);
    cout << endl << endl;
}

int main(int argc, char *argv[])
{
    if(argc > 0)
    {
        // Populate jobs vector with contents from input file
        populateJobs(argv[1]);

        fcfs();
        spn();
        hrrn();
    }
}