#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "func.h"
using namespace std;

void write_file(vector<vector<info_ins_del>>& a, vector<vector<info_inv_dup>>& b, vector<info_tra>& c,vector<string>d)
{
    ofstream fout;
    fout.open("result.txt", ios::out); //read the two DNA sequences of ref.fasta into vector a
    if (!fout.is_open())
    {
        cout << "read file error" << endl;
    }
	
	//INS
	for (int i = 0; i < a[0].size(); i++)
	{
		if (a[0][i].s != " ")
			fout <<"INS" << " "
			<< "NZ_LN879429.1"
			<< " " << a[0][i].start << " " << a[0][i].end << endl;
	}
	for (int i = 0; i < a[1].size(); i++)
	{
		if (a[1][i].s != " ")
			fout <<"INS"<< " "
			<< "NZ_AKCA01000001.1"
			<< " " << a[1][i].start << " " << a[1][i].end << endl;
	}
	//DEL
	for (int i = 0; i < a[2].size(); i++)
	{
		if (a[2][i].s != " ")
			fout <<"DEL" << " "
			<< "NZ_LN879429.1"
			<< " " << a[2][i].start << " " << a[2][i].end << endl;
	}
	for (int i = 0; i < a[3].size(); i++)
	{
		if (a[3][i].s != " ")
			fout << "DEL" << " "
			<< "NZ_AKCA01000001.1"
			<< " " << a[3][i].start << " " << a[3][i].end << endl;
	}
	//DUP
	for (int i = 0; i < b[2].size(); i++)
	{
		fout << "DUP" << " "
			<< "NZ_LN879429.1"
			<< " " << b[2][i].start << " " << b[2][i].end << endl;
	}
	for (int i = 0; i < b[3].size(); i++)
	{
		fout << "DUP" << " "
			<< "NZ_AKCA01000001.1"
			<< " " << b[3][i].start << " " << b[3][i].end << endl;
	}
	//INV
	for (int i = 0; i < b[0].size(); i++)
	{
		fout << "INV" << " "
			<< "NZ_LN879429.1"
			<< " " << b[0][i].start << " " << b[0][i].end << endl;
	}
	for (int i = 0; i < b[1].size(); i++)
	{
		fout << "INV" << " "
			<< "NZ_AKCA01000001.1"
			<< " " << b[1][i].start << " " << b[1][i].end << endl;
	}

	//TRA
	for (int i = 0; i < c.size(); i++)
	{
		fout << "TRA"
			<< " " << d[c[i].id_del] << " " << c[i].start_del << " "
			<< c[i].end_del << " " << d[c[i].id_ins] << " " << c[i].start_ins << " "
			<< c[i].end_ins << endl;
	}
	fout.close();

}