#include <iostream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include "Oror.h"
#include "Command.h"
using namespace std;

int Oror::execute()
{
    int currStatus = 0;
    currStatus = first->execute();
    currStatus = second->execute();
    if(currStatus != 1)
    {
        return -1;
    }
    return currStatus;
}