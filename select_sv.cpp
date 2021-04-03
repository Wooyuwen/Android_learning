#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>
#include "func.h"
using namespace std;

void select_sv(vector<string>& a, vector<string>& b, vector<string> c)
{
	vector<info_ins_del> insert_LN;	   //ins string info
	vector<info_ins_del> delete_LN;	   //del string info
	vector<info_inv_dup> inverse_LN;   //inv string info
	vector<info_inv_dup> duplicate_LN; //dup string info

	vector<info_ins_del> insert_AK;
	vector<info_ins_del> delete_AK;
	vector<info_inv_dup> inverse_AK;
	vector<info_inv_dup> duplicate_AK;

	vector<vector<info_ins_del>>Insert_Del;
	vector<vector<info_inv_dup>>Dup_Inv;

	vector<info_tra> tra_all;//storing both types of tra
	vector<info_c_tra>tra_cop_LN;//selecting complicated tra
	vector<info_c_tra>tra_cop_AK;

	//>NZ_LN879429.1
	int index1 = 0, index2 = 0;
	int len1 = a[0].length();
	int len2 = b[1].length();
	string t = a[0], p = b[1];
	while (index1 < len1 && index2 < len2)
	{
		
		if (t[index1] == p[index2])
		{
			index1++;
			index2++;
			continue;
		}

		int is_dup = 0;
		int pre = 0;
		int tmp1 = check_ins_dup(t, p, index1, index2, is_dup,pre);
		if (tmp1>=0)
		{
			if (is_dup == 0) {
				info_ins_del info = { p.substr(index2 - tmp1, tmp1), index1, index1 + tmp1 };
				insert_LN.push_back(info);
			}
			else
			duplicate_LN.push_back({ index1 - tmp1-pre, index1-pre });
			continue;
		}
		int tmp2 = check_del(t, p, index1, index2);
		if ( tmp2>= 0)
		{
			info_ins_del info = { p.substr(index1 - tmp2, tmp2), index1 - tmp2, index1 };
			delete_LN.push_back(info);
			continue;
		}

		int tmp3 = check_inv(t, p, index1, index2);
		if ( tmp3>= 0)
		{
			inverse_LN.push_back({ index1 - tmp3, index1 });
			continue;
		}

		if (check_c_tra(0,t, p, index1, index2,tra_cop_LN) >= 0)
		{
			continue;
		}
		index1++;
		index2++;
	}

	//> NZ_AKCA01000001.1
	index1 = 0, index2 = 0;
	len1 = a[1].length();
	len2 = b[0].length();
	t = a[1];
	p = b[0];
	while (index1 < len1 && index2 < len2)
	{
		if (t[index1] == p[index2])
		{
			index1++;
			index2++;
			continue;
		}

		int is_dup = 0;
		int pre = 0;
		int tmp1 = check_ins_dup(t, p, index1, index2, is_dup,pre);
		if ( tmp1>= 0)
		{
			if(is_dup==0)
			insert_AK.push_back({ p.substr(index2 - tmp1, tmp1), index1, index1 + tmp1 });
			else
			duplicate_AK.push_back({ index1 - tmp1-pre, index1-pre});
			continue;
		}

		int tmp2 = check_del(t, p, index1, index2);
		if ( tmp2>= 0)
		{
			delete_AK.push_back({ p.substr(index1 - tmp2, tmp2), index1 - tmp2, index1 });
			continue;
		}

		int tmp3 = check_inv(t, p, index1, index2);
		if (tmp3>= 0)
		{
			inverse_AK.push_back({ index1 - tmp3, index1 });
			continue;
		}

		if (check_c_tra(1, t, p, index1, index2, tra_cop_AK) >= 0)
		{
			continue;
		}
		index1++;
		index2++;
	}
	check_tra(tra_all, tra_cop_LN,tra_cop_AK);

	Insert_Del.push_back(insert_LN);
	Insert_Del.push_back(insert_AK);
	Insert_Del.push_back(delete_LN);
	Insert_Del.push_back(delete_AK);
	Dup_Inv.push_back(inverse_LN);
	Dup_Inv.push_back(inverse_AK);
	Dup_Inv.push_back(duplicate_LN);
	Dup_Inv.push_back(duplicate_AK);

	write_file(Insert_Del,Dup_Inv,tra_all,c);
}

int check_ins_dup(string t, string p, int& ind1, int& ind2,int&is_dup,int&pre)
{
	if (ind1 >= t.length() - 1000 || ind2 >= p.length() - 2000)
		return -1;
	string t_ref = t.substr(ind1, 20);
	string t_sv = p.substr(ind2, 1100);
	int del = kmp(t_sv, t_ref);
	if (del >= 0)
	{
		//ins? :yes
		//dup? : check
		for (int i = 0; i < 5; i++) {
			string dup = p.substr(ind2-i, del);
			string pre = p.substr(ind2-i-del, del);
			
			if (strcmp(dup.c_str(), pre.c_str()) == 0){
				pre = i;//偏差位数，记录起始位置需要往前移动i个单位
				is_dup = 1;
				ind2 += del;
				return del;
			}
			else if(i==4){
				ind2 += del;
				return del;
			}
		}
	}
	return -1;
}

int check_del(string t, string p, int& ind1, int& ind2)
{
	if (ind1 >= t.length() - 2000 || ind2 >= p.length() - 1000)
		return -1;
	string t_ref = t.substr(ind1, 1100);
	string t_sv = p.substr(ind2, 50);
	int ins = kmp(t_ref, t_sv);
	if (ins >= 0)
	{
		ind1 += ins;
	}
	return ins;
}

int check_inv(string t, string p, int& ind1, int& ind2)
{
	if (ind1 >= t.length() - 1000 || ind2 >= p.length() - 1000)
		return -1;
	int len = 0;
	int ans = 0;
	bool even = false;
	string re = t.substr(ind1, 1000);
	string s = p.substr(ind2, 1000);
	const char* ref = re.c_str();
	const char* sv = s.c_str();
	while (len <= 497)
	{
		if (cmp(ref[len], sv[len]) )
		{
			for (int i = 1; i <= len; i++)
			{
				if (cmp(ref[len + i], sv[len - i]) == false ||
					cmp(ref[len - i], sv[len + i]) == false)
				{
					break;
				}
				if (i == len)
					ans = len;
			}
		}
		len++;
	}
	len = 0;
	while (len <= 497)
	{
		if (cmp(ref[len], sv[len + 1]) && cmp(ref[len + 1], sv[len]))
		{
			for (int i = 2; i <= len + 1; i++)
			{
				if (cmp(ref[len + i], sv[len - i + 1]) == false ||
					cmp(ref[len - i + 1], sv[len + i]) == false)
				{
					break;
				}
				if (i == len+1)
				{
					if (ans < len)
					{
						even = true;
						ans = len;
					}
				}
			}
		}
		len++;
	}
	if (ans >= 24) {
		if (even == true)
		{
			ind1 += 2 * (ans + 1);
			ind2 += 2 * (ans + 1);
			return 2 * (ans + 1);
		}
		else
		{
			ind1 += 2 * ans + 1;
			ind2 += 2 * ans + 1;
			return (2 * ans + 1);
		}
	}
	else
		return -1;
}

void check_tra(vector<info_tra>& t, vector<info_c_tra>ln,vector<info_c_tra>ak)
{
	//combine the exchanged tra
	for (int i = 0; i < ln.size(); i++) {
		for (int j = 0; j < ak.size(); j++) {
			if (ln[i].len == ak[j].len)
				t.push_back({0,ln[i].start,ln[i].start+ln[i].len,1,ak[j].start,ak[j].start + ak[j].len });
		}
	}
	//select the simple tra

}

//correct
int check_c_tra(int id, string t, string p, int& ind1, int& ind2, vector<info_c_tra>& tra) {
	int len = 0;
	int max = 0;
	string subt = t.substr(ind1, 1010);
	string subp = p.substr(ind2, 1010);
	while (len < 1000) {
		if (subt[len] != subp[len]) {
			len++;
		}
		else {
			while (subt[len] == subp[len]) {
				max++;
				len++;
				if (max == 10)
					break;
			}
			if (max < 10) {
				max = 0;
			}
			else {//find it! the complicated tra
				tra.push_back({ id,ind1,len - 10,subt.substr(0,len - 10),subp.substr(0,len - 10) });
				ind1 += len - 10;
				ind2 += len - 10;
				return len - 10;
			}
		}
	}
	return -1;
}

bool cmp(char a, char b){ 
	//return true for 'GC' or 'AT'
	if ((a == 'G' && b == 'C' )||( a == 'A' && b == 'T') ||( a == 'C' && b == 'G') ||( a == 'T' && b == 'A'))
		return true;
	else
		return false;
}

int* getNext(string p)
{
	int* next = new int[p.length()];
	next[0] = -1;           //while the first char not match, i++,j++
	int j = 0;
	int k = -1;
	while (j < (int)p.length() - 1)
	{
		if (k == -1 || p[j] == p[k])
		{
			j++;
			k++;
			next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}
	return next;
}

int kmp(string T, string p)
{
	int i = 0;
	int j = 0;
	int* next = getNext(T);
	while (i < (int)T.length() && j < (int)p.length())
	{
		if (j == -1 || T[i] == p[j])
		{
			i++;
			j++;
		}
		else
		{
			j = next[j];
		}
	}
	if (j == (int)p.length())
	{
		delete []next;
		return i - j;
	}
	delete []next;
	return -1;
}

