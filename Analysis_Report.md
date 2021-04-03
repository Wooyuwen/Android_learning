###  **分析报告**

 																																`17307130115 吴毓文`

> `IDE: Visual Studio 2019`
>
> 操作系统：`windows`
>
> 编程语言:`C/C++`
>
> 实验结果：成功找到所有`SV`片段，但多个`SV`片段起始值和返回值与参考答案数据存在整数1-5的偏差。
>
> 但是`len`大小相同，也即`POS`上存在偏差。

#### 项目结构：

> `main.cpp`：主函数入口，调用`select_sv()，read_file()`函数；
>
> `read_file.cpp`：读取序列数据以及编号并赋值给string对象`ref，sv,ID`；
>
> `write_file.cpp`：将输出写入结果文件`result.txt`；
>
> `select_sv.cpp`：检测序列错误，并调用`write_file()`将结果写入`result.txt`文件；功能主体；
>
> `func.h`：保存函数和结构体的声明；



#### 算法设计思路(`select_sv`)：

首先给出使用范围贯穿整个文件的字符串匹配算法`kmp():`

```
int* getNext(string p){...}
int kmp(string T, string p){...}
```

该算法时间复杂度和空间复杂度均为`O(n)`；具体实现见`select_sv.cpp`；（326行）

记原始(ID相同)DNA字符串序列为`ref`，包含错误信息的单链为`sv`，分别对`ref`和`sv`从下标`index1`和`index2`(初始化为0)开始每一个字符进行比较，相同则比较下一对字符，若不相同，则开始错误类型检测：

##### 四种类型检测函数：

> `check_ins_dup()`:检测错误类型是否为INS或者`DUP`；
>
> `check_del()`：检测错误类型是否为`DEL`;
>
> `check_inv()`：检测错误类型是否为`INV`；
>
> `check_tra()`：检测错误类型是否为`TRA`； 
>
> 以上四个函数返回值均为对应查找到的`SV`片段长度，同时根据类型相应地改变`index1`和`index2`。

> **检测`INS/DUP：`**
>
> 实现函数：`select_sv.cpp/check_ins_dup()`
>
> ```c++
> int check_ins_dup(string t, string p, int& ind1, int& ind2,int&is_dup,int&pre)
> {
> 	if (ind1 >= t.length() - 1000 || ind2 >= p.length() - 2000)
> 		return -1;
> 	string t_ref = t.substr(ind1, 20);
> 	string t_sv = p.substr(ind2, 1100);
> 	int del = kmp(t_sv, t_ref);
> 	if (del >= 0)
> 	{
> 		//ins? :yes
> 		//dup? : check
> 		for (int i = 0; i < 5; i++) {
> 			string dup = p.substr(ind2-i, del);
> 			string pre = p.substr(ind2-i-del, del);
> 			
> 			if (strcmp(dup.c_str(), pre.c_str()) == 0){
> 				pre = i;//偏差位数，记录起始位置需要往前移动i个单位
> 				is_dup = 1;
> 				ind2 += del;
> 				return del;
> 			}
> 			else if(i==4){
> 				ind2 += del;
> 				return del;
> 			}
> 		}
> 	}
> 	return -1;
> }
> ```
>
> ​    思路：这里将`INS和DUP`类型一起检测是由于`DUP`可以看做INS的变种，即插入一段与前面内容相同的片段。检测的原理是将`ref`错误片段开始的长度为20的子序列`t_ref`和`sv`相同位置 开始长度为1100的子序列`t_sv`作为参数传递给`kmp`()，若匹配成功，返回值即为插入片段长度`len`。
>
> ​    接下来检测是否为`DUP`：取`sv`上`index2`开始长度为`len`的字符串`dup`和`index2-len`开始长度为`len`的字符串`pre`。
>
> ​    调用`strcmp()`比较这两个字符串，相等则错误类型`is_dup`设置为1。特别的，这里做了''偏差位数'‘的处理：因为重复子串`dup`也可能和ref中对应`dup`之后的序列在前几位相同，我们假设最多可能4个相同，因为此时概率为(1/4)^4=0.39%，再长的位数近乎不可能不以考虑。使用一个for循环检查是否存在偏差位数i，命中则返回对应的`POS`和`LEN`，以及偏差位数`pre(i)`并将`index2`加上LEN（`del`）以调整至下一轮检测位置。

------

> **检测`DEL：`**
>
> 实现函数：`select_sv.cpp/check_del()` 
>
> ​    思路：与INS检测相同，但是这里调用`kmp()`时，匹配字符串从`sv`中获取，目标字符串该从`ref`中获取，若命中并返回`len`，则将`index1`加上`len`，进入下一轮检查。具体代码见`select_sv.cpp`文件。

------

> **检测`INV：`**
>
> 实现函数：`select_sv.cpp/check_inv()` 
>
> ​     思路:同样是从不匹配的地方开始`(index1,index2)`，但对`ref`和`sv`同时选取从查错位开始的长度为1000的子串，对i=0→497，这里i为倒转序列(若有的话)的中间位置下标，这里使用了字符匹配函数`bool` `cmp`(char,char)(`select_sv.cpp298行`)，功能为若a,b两个字符在碱基对意义上匹配成功`（AT or GC）`返回true，否则返回false。以i为中心向两边延展比较，同时更新翻转子串的最大长度。这里也检测了子串长度的奇偶性以便返回正确的`SV`片段长度。这里给出主要代码，具体代码见`select_sv.cpp`文件：
>
> ```c++
> int check_inv(string t, string p, int& ind1, int& ind2)
> {
> 	if (ind1 >= t.length() - 1000 || ind2 >= p.length() - 1000)
> 		return -1;
> 	int len = 0;
> 	int ans = 0;
> 	bool even = false;
> 	string re = t.substr(ind1, 1000);
> 	string s = p.substr(ind2, 1000);
> 	const char* ref = re.c_str();
> 	const char* sv = s.c_str();
> 	while (len <= 497)
> 	{
> 		if (cmp(ref[len], sv[len]) )
> 		{
> 			for (int i = 1; i <= len; i++)
> 			{
> 				if (cmp(ref[len + i], sv[len - i]) == false ||
> 					cmp(ref[len - i], sv[len + i]) == false)
> 				{
> 					break;
> 				}
> 				if (i == len)
> 					ans = len;
> 			}
> 		}
> 		len++;
> 	}
> 	len = 0;
>     //偶数长度检测，代码长了不贴，思路和奇数位检测类似，并标记偶数标记位even
>     //返回对应的长度
> }
> 
> ```

------

> **检测`TRA：`**
>
> 实现函数：`select_sv.cpp/check_c_tra()`
>
> ```c++
> int check_c_tra(int id, string t, string p, int& ind1, int& ind2, vector<info_c_tra>& tra) {
> 	int len = 0;
> 	int max = 0;
> 	string subt = t.substr(ind1, 1010);
> 	string subp = p.substr(ind2, 1010);
> 	while (len < 1000) {
> 		if (subt[len] != subp[len]) {
> 			len++;
> 		}
> 		else {
> 			while (subt[len] == subp[len]) {
> 				max++;
> 				len++;
> 				if (max == 10)
> 					break;
> 			}
> 			if (max < 10) {
> 				max = 0;
> 			}
> 			else {//find it! the complicated tra
> 				tra.push_back({ id,ind1,len - 10,subt.substr(0,len - 10),subp.substr(0,len - 10) });
> 				ind1 += len - 10;
> 				ind2 += len - 10;
> 				return len - 10;
> 			}
> 		}
> 	}
> 	return -1;
> }
> ```
>
> ​    思路：从查错位开始两边字符逐个比较直至`ref`和` sv`连续10个字符相同，即两边字符对上相同的片段，然后将`ref`上之前不相同的字符串存储在结构`info_c_tra`中（声明在`func.h`）,此时为待确认，需要在检测完所有`DNA`单链后对所有的可能的单链信息一一匹配，匹配成功说明该两个可能`tra`为互换的一对。
>
> ```c++
> struct info_c_tra {//store tra and check later
> 	int id;
> 	int start;
> 	int len;
> 	string s1;
> 	string s2;
> };
> ```
>
> 找完数据所有单链中可能为`tra`的片段后，两两比对，确认是否为互换的一对；实现该功能的函数为
>
> `check_tra()`：
>
> ```c++
> void check_tra(vector<info_tra>& t, vector<info_c_tra>ln,vector<info_c_tra>ak)
> {
> 	//combine the exchanged tra
> 	for (int i = 0; i < ln.size(); i++) {
> 		for (int j = 0; j < ak.size(); j++) {
> 			if (ln[i].len == ak[j].len)
> 				t.push_back({0,ln[i].start,ln[i].start+ln[i].len,1,ak[j].start,ak[j].start + ak[j].len });
> 		}
> 	}
> 	//select the simple tra
> 
> }
> ```
>
> ​    

至此就找完了所有的`SV`片段，执行程序，读取数据文件，将结果写至`result.txt`文件，写文件的函数见`write_file.cpp`；

```c++
INS NZ_AKCA01000001.1 1953747 1954463
INS NZ_AKCA01000001.1 1967739 1968097
DEL NZ_LN879429.1 377911 378364
DEL NZ_LN879429.1 620355 621317
DEL NZ_LN879429.1 1253679 1254374
DEL NZ_LN879429.1 1356937 1357805
DEL NZ_AKCA01000001.1 1164626 1165001
DEL NZ_AKCA01000001.1 1337447 1338053
DEL NZ_AKCA01000001.1 1496848 1497671
DEL NZ_AKCA01000001.1 1501996 1502211
DEL NZ_AKCA01000001.1 1803678 1804106
DEL NZ_AKCA01000001.1 2049682 2049898
DUP NZ_LN879429.1 155138 155896
DUP NZ_LN879429.1 472186 473001
DUP NZ_LN879429.1 513302 513473
DUP NZ_LN879429.1 608769 608878
DUP NZ_LN879429.1 681202 681923
DUP NZ_LN879429.1 1476381 1476816
DUP NZ_AKCA01000001.1 253540 254343
DUP NZ_AKCA01000001.1 1394607 1395533
DUP NZ_AKCA01000001.1 1636577 1637492
DUP NZ_AKCA01000001.1 1924531 1925035
INV NZ_LN879429.1 758097 758865
INV NZ_LN879429.1 1037216 1038025
INV NZ_LN879429.1 1320631 1321222
INV NZ_LN879429.1 1717744 1717971
INV NZ_AKCA01000001.1 223919 224143
INV NZ_AKCA01000001.1 464259 464616
INV NZ_AKCA01000001.1 1346238 1346973
INV NZ_AKCA01000001.1 1707392 1707793
INV NZ_AKCA01000001.1 1870175 1871016
INV NZ_AKCA01000001.1 2093281 2093432
TRA NZ_LN879429.1 103434 103492 NZ_AKCA01000001.1 649739 649797
TRA NZ_LN879429.1 375813 376744 NZ_AKCA01000001.1 247851 248782
TRA NZ_LN879429.1 394881 395728 NZ_AKCA01000001.1 1131312 1132159
TRA NZ_LN879429.1 908045 908528 NZ_AKCA01000001.1 657370 657853
TRA NZ_LN879429.1 990013 990817 NZ_AKCA01000001.1 701287 702091
TRA NZ_LN879429.1 1335850 1336134 NZ_AKCA01000001.1 1386667 1386951
TRA NZ_LN879429.1 1376307 1376974 NZ_AKCA01000001.1 1361173 1361840
TRA NZ_LN879429.1 1530150 1531062 NZ_AKCA01000001.1 972580 973492
TRA NZ_LN879429.1 1827916 1828893 NZ_AKCA01000001.1 1604599 1605576
TRA NZ_LN879429.1 1883308 1884294 NZ_AKCA01000001.1 499915 500901
```

成功找到所有的`SV`片段，但多个`SV`片段起始值`POS`和返回值`POS+LEN`与参考答案数据存在整数1-5的偏差。

