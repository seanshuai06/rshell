#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

#include "Processes.h"
#include "Base.h"
#include "Command.h"
#include "Andand.h"
#include "Oror.h"

//Simple constructor
Processes::Processes()
{
    currCmds.resize(0);
    string andand = "&&";
    string oror = "||";
}
//Simple destructor
Processes::~Processes()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
}
//This executes all objects within the object's currCmds vector
int Processes::execute()
{
    int temp = 0;
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        temp = currCmds.at(i)->execute();
    }
    return temp;
}
//Processes parses the code to split it into separate command lines
//should there be any semicolons present.
void Processes::parse(string input)
{
    vector<string> currCs;
    istringstream inSS(input);
    string currString;
    //Main loop for parsing input that contains semicolons
    while(input.find(";") != string::npos)
    {
        bool hashtag = false;
        bool semicolon = false;
        currCs.resize(0);
        while(!semicolon)
        {
            inSS >> currString;
            input.erase(0, currString.size() + 1);
            //Tests for hashtag/semicolon presence
            if(currString.find("#") != string::npos)
            {
                hashtag = true;
            }
            if(currString.find(";") != string::npos)
            {
                semicolon = true;
            }
            if(!hashtag)
            {
                if(!semicolon)
                {
                    currCs.push_back(currString);
                }
                else
                {
                    currString.erase(currString.size() - 1);
                    currCs.push_back(currString);
                }
            }
        }
        bool detected = false;
        for(unsigned j = 0; j < currCs.size(); ++j)
        {
            if(currCs.at(j) == "||" || currCs.at(j) == "&&")
            {
                detected = true;
                break;
            }
        }
        if(detected)
        {
            string prevConnector;
            string nextConnector;
            vector<string> firstCommand;
            unsigned i = 0;
            while(currCs.at(i) != "&&" && currCs.at(i) != "||")
            {
                firstCommand.push_back(currCs.at(i));
                ++i;
            }
            prevConnector = currCs.at(i);
            Base *temp3 = new Command(firstCommand);
            currCmds.push_back(temp3);
            ++i;
            vector<string> currCommand;
            for(;i < currCs.size(); ++i)
            {
                currCommand.push_back(currCs.at(i));
                if(currCs.at(i) == "&&" || currCs.at(i) == "||")
                {
                    currCommand.pop_back();
                    Base *temp = new Command(currCommand);
                    nextConnector = currCs.at(i);
                    if(prevConnector == "&&")
                    {
                        Base *temp2 = new Andand(currCmds.at(currCmds.size() - 1), temp);
                        currCmds.pop_back();
                        currCmds.push_back(temp2);
                        currCommand.resize(0);
                    }
                    else
                    {
                        Base * temp2 = new Oror(currCmds.at(currCmds.size() - 1), temp);
                        currCmds.pop_back();
                        currCmds.push_back(temp2);
                        currCommand.resize(0);
                    }
                }
            }
        }
        else {
            vector<string> currCommand;
            for(unsigned k = 0; k < currCs.size(); ++k)
            {
                currCommand.push_back(currCs.at(k));
            }
            Base *temp = new Command(currCommand);
            currCmds.push_back(temp);
        }
    }
    //Second loop that parses the remaining part of the input
    //Also, if the input never contained any semicolons,
    //the parse will go straight to this part of the code
    currCs.resize(0);
    while(inSS >> currString)
    {
        if(currString.find("#") != string::npos)
        {
            break;
        }
        currCs.push_back(currString);
    }
    bool detected = false;
    for(unsigned j = 0; j < currCs.size(); ++j)
    {
        if(currCs.at(j) == "&&" || currCs.at(j) == "||")
        {
            detected = true;
            break;
        }
    }
    if(detected)
    {
        string prevConnector;
        string nextConnector;
        vector<string> firstCommand;
        unsigned i = 0;
        while(currCs.at(i) != "&&" && currCs.at(i) != "||")
        {
            firstCommand.push_back(currCs.at(i));
            ++i;
        }
        prevConnector = currCs.at(i);
        Base *temp3 = new Command(firstCommand);
        currCmds.push_back(temp3);
        ++i;
        vector<string> currCommand;
        for(; i < currCs.size(); ++i)
        {
            currCommand.push_back(currCs.at(i));
            if(currCs.at(i) == "&&" || currCs.at(i) == "||")
            {
                currCommand.pop_back();
                Base *temp = new Command(currCommand);
                nextConnector = currCs.at(i);
                if(prevConnector == "&&")
                {
                    Base *temp2 = new Andand(currCmds.at(currCmds.size() - 1), temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    currCommand.resize(0);
                }
                else
                {
                    Base * temp2 = new Oror(currCmds.at(currCmds.size() - 1), temp);
                    currCmds.pop_back();
                    currCmds.push_back(temp2);
                    currCommand.resize(0);
                }
            }
        }
        Base * temp = new Command(currCommand);
        if(prevConnector == "&&")
            {
                Base *temp2 = new Andand(currCmds.at(currCmds.size() - 1), temp);
                currCmds.pop_back();
                currCmds.push_back(temp2);
                currCommand.resize(0);
            }
        else
        {
            Base * temp2 = new Oror(currCmds.at(currCmds.size() - 1), temp);
            currCmds.pop_back();
            currCmds.push_back(temp2);
        }
    }
    else {
        vector<string> currCommand;
        for(unsigned k = 0; k < currCs.size(); ++k)
        {
            currCommand.push_back(currCs.at(k));
        }
        Base *temp = new Command(currCommand);
        currCmds.push_back(temp);
    }
}

void Processes::reset()
{
    for(unsigned i = 0; i < currCmds.size(); ++i)
    {
        delete currCmds.at(i);
    }
    this->currCmds.resize(0);
}