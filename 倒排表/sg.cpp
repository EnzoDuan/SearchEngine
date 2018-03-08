#include<iostream>  
#include<fstream>  
//#include<stdio.h>  
#include<string.h>  
#include<map>  
//#include<vector>  
#include "vector"
#include<string>  
#include<algorithm>  
using namespace std;  
map<string,vector<int> > indextable;//倒排索引表  
void init() //初始化表  
{  
	indextable.clear();
}  

int main()  
{  
	//重定向从index.txt中读入，输出到result.txt，index.txt,result.txt都是在当前目录下  
	freopen("index.txt","r",stdin);  
	freopen("result.txt","w",stdout);  
	init();  
	int id;    //文件名  
	string filepath; //文件路径名  
	while(cin >> id >> filepath)//从index中读入文件名和文件路径名  
	{   
		setlocale(LC_ALL,"Chinese-simplified");//设置中文环境
		ifstream fin(filepath.c_str());//打开文件路径下的文件，参数应是c风格的字符串 
		fprintf(stderr, "打开文件：%s\n", filepath.c_str());
		setlocale(LC_ALL,"C");//还原
		string s;  
		while(fin>>s)//一个单词一个单词地读入  
		{  
			indextable[s].push_back(id);//把当前单词对应的文件名加入到单词对应的ID数组中  
		}  
	}  
	fprintf(stderr, "创建倒排表中。。。\n");
	map<string,vector<int> >::iterator map_it;//索引表迭代器  
	map_it=indextable.begin();  
	while(map_it!=indextable.end())//遍历整个索引表输出，因为MAP的键值是严格弱排序，因此输出是字典序  
	{  
		string tmp=map_it->first;  
		cout<<tmp<<" ";  
		int count = 1;
		for(int i=0;i < indextable[tmp].size()-1;i++){ 
			if(indextable[tmp][i] != indextable[tmp][i+1]){
				cout << indextable[tmp][i] << "|" << count << " ";
				count = 1;
			}
			else
				count++;
		}
		cout << indextable[tmp][indextable[tmp].size()-1] << "|" << count << endl;
		map_it++;  
	}  
	fprintf(stderr, "完成！\n");
	return 0;  
}  