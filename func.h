#pragma once
#ifndef FUNC_H
#define FUNC_H
#include <iostream>
#include <vector>
using namespace std;


struct info_ins_del {// for ins and delete
	string s;
	int start;
	int end;
};

struct info_inv_dup {// for inv and dup
	int start;
	int end;
};

struct info_tra {//for tra results
	int id_del;
	int start_del;
	int end_del;

	int id_ins;
	int start_ins;
	int end_ins;
};

struct info_c_tra {//store tra and check later
	int id;
	int start;
	int len;
	string s1;
	string s2;
};

void select_sv(vector<string>& a, vector<string>& b, vector<string> c);
void read_file(vector<string>& a, vector<string>& b, vector<string>& c);
void write_file(vector<vector<info_ins_del>>& a, vector<vector<info_inv_dup>>& b, vector<info_tra>& c,vector<string>d);

int check_ins_dup(string t, string p, int& ind1, int& ind2,int& dup,int&pre);
int check_del(string t, string p, int& ind1, int& ind2);
int check_inv(string t, string p, int& ind1, int& ind2);
void check_tra(vector<info_tra>& t,vector<info_c_tra>ln,vector<info_c_tra>ak);
int check_c_tra(int id,string t, string p, int& ind1, int& ind2,vector<info_c_tra>&r);
bool cmp(char a, char b);
int* getNext(string t);
int kmp(string t, string p);


#endif
