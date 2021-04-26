// Author: Cameron Meyer (cdm180003)
// Project 3
// CS 4348.001


#include <string>
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

const string space = "\n\t\r\v\f "; // Whitespace characters for parsing the input file

vector<tuple<string, int, int>> jobs; 

// Custom sorting functionality to sort a vector of jobs based on duration of the job
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
                    if(space.find(c) != string::npos) // Check if the current char is a whitespace
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

            // When all elements are accounted for, create a tuple for this job
            if(line.size() == 3)
            {
                tuple <string, int, int> job;
                job = make_tuple(line[0], stoi(line[1]), stoi(line[2]));
                jobs.push_back(job);
            }
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
    int time = 0;
    while(true)
    {
        int jobIndex = 0;
        int runDuration = 0;
        running = "";

        // Ensure the job has arrived before running it
        if(get<1>(jobs[completedJobs]) <= time)
        {
            // Run the next job in the list
            runDuration = get<2>(jobs[completedJobs]);
            running = get<0>(jobs[completedJobs]);
        }
        else
        {
            // Don't run anything if the job hasn't arrived
            runDuration = 1;
            completedJobs--;

            if(completedJobs < 0) { completedJobs = 0; }
        }

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
        completedJobs++;

        // Exit once all jobs have been run
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

    // Sorts the copied list of jobs based on their run duration
    sort(spnJobs.begin(), spnJobs.end(), sortDuration);

    while(spnJobs.size() > 0)
    {
        int index = 0;
        int jobIndex = 0;
        int runDuration = 0;
        running = "";

        for(int i = 0; i < spnJobs.size(); i++)
        {
            // Ensure a job has arrived before running it
            if(get<1>(spnJobs[i]) <= time)
            {
                running = get<0>(spnJobs[i]);
                runDuration = get<2>(spnJobs[i]);
                index = i;
                break;
            }
        }

        // Erase the running job from the list if applicable
        if(running == "") { runDuration = 1; }
        else { spnJobs.erase(spnJobs.begin() + index); } 

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
        running = "";

        for(int i = 0; i < hrrnJobs.size(); i++)
        {
            // Ensure a job has arrived before running it
            if(get<1>(hrrnJobs[i]) > time) { break; }

            // Calculate the response ratio for this job and compare against the current highest value
            int tempR = ((time - get<1>(hrrnJobs[i])) + get<2>(hrrnJobs[i])) / get<2>(hrrnJobs[i]);
            if(r < tempR)
            {
                // Prepare to run the job with the highest response ratio next
                r = tempR;
                running = get<0>(hrrnJobs[i]);
                runDuration = get<2>(hrrnJobs[i]);
                index = i;
            }
        }

        // Erase the running job from the list if applicable
        if(running == "") { runDuration = 1; }
        else { hrrnJobs.erase(hrrnJobs.begin() + index); }

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
}

int main(int argc, char *argv[])
{
    if(argc > 0)
    {
        // Populate jobs vector with contents from input file
        populateJobs(argv[1]);

        // Run the necessary scheduling algorithms
        fcfs();
        spn();
        hrrn();
    }
}