#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "func.h"
using namespace std;

void read_file(vector<string>& a, vector<string>& b, vector<string>& c)
{
    string id;  //DNA ID
    string seq; //DNA sequence

    ifstream fin;
    fin.open("ref.fasta", ios::in); //read the two DNA sequences of ref.fasta into vector a
    //fin.open("task2_ref.fasta", ios::in);
    if (!fin.is_open())
    {
        cout << "read file error" << endl;
    }
    getline(fin, id); //read the first ID
    c.push_back(id);
    getline(fin, seq); //read the sequence
    a.push_back(seq);
    getline(fin, id);
    c.push_back(id); //read the next ID
    getline(fin, seq);
    a.push_back(seq);
    fin.close();

    fin.open("sv.fasta", ios::in); //read the two DNA sequences of sv.fasta into vector b
    //fin.open("task2_sv.fasta", ios::in);
    if (!fin.is_open())
    {
        cout << "read file error" << endl;
    }
    getline(fin, id); //read the first ID
    c.push_back(id);
    getline(fin, seq); //read the sequence
    b.push_back(seq);
    getline(fin, id);
    c.push_back(id); //read the next ID
    getline(fin, seq);
    b.push_back(seq);
    fin.close();
}
