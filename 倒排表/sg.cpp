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
map<string,vector<int> > indextable;//����������  
void init() //��ʼ����  
{  
	indextable.clear();
}  

int main()  
{  
	//�ض����index.txt�ж��룬�����result.txt��index.txt,result.txt�����ڵ�ǰĿ¼��  
	freopen("index.txt","r",stdin);  
	freopen("result.txt","w",stdout);  
	init();  
	int id;    //�ļ���  
	string filepath; //�ļ�·����  
	while(cin >> id >> filepath)//��index�ж����ļ������ļ�·����  
	{   
		setlocale(LC_ALL,"Chinese-simplified");//�������Ļ���
		ifstream fin(filepath.c_str());//���ļ�·���µ��ļ�������Ӧ��c�����ַ��� 
		fprintf(stderr, "���ļ���%s\n", filepath.c_str());
		setlocale(LC_ALL,"C");//��ԭ
		string s;  
		while(fin>>s)//һ������һ�����ʵض���  
		{  
			indextable[s].push_back(id);//�ѵ�ǰ���ʶ�Ӧ���ļ������뵽���ʶ�Ӧ��ID������  
		}  
	}  
	fprintf(stderr, "�������ű��С�����\n");
	map<string,vector<int> >::iterator map_it;//�����������  
	map_it=indextable.begin();  
	while(map_it!=indextable.end())//���������������������ΪMAP�ļ�ֵ���ϸ����������������ֵ���  
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
	fprintf(stderr, "��ɣ�\n");
	return 0;  
}  