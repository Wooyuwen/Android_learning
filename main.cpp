#include <iostream>
#include <vector>
#include <string>
#include "func.h"

using namespace std;

int main()
{
    vector<string> ID;
    vector<string> ref;
    vector<string> sv;
    read_file(ref, sv, ID); //read file
    select_sv(ref, sv, ID); //select all the sv
    system("pause");
    return 0;
}

