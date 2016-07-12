#include <string>
#include "bfmm.h"
using namespace std;
int BMM::divide_sentence(string &sentence,string seperator)//�����в����б�㡢�ո�
{
	if(dict_index.len_max==0)
		createDictIndex(false);
	int count=0;
	int pi=0;//��ǰλ��
	int m=dict_index.len_max;//�ʵ�������ַ�����
	int n=sentence.length()-pi;//ʣ�����ֵĳ���
	string tword;//��ǰ����
	
	//���ֽ�ƥ�䣬���ֵ��бȶԣ����������
	while(n >= 1 )
	{
		m=dict_index.len_max;
		if(n<m) m=n;
		tword=sentence.substr(pi,m);
		if(searchWord(tword)!=-1)//�ҵ�ƥ��
		{
			sentence.insert(pi+m,seperator);
			pi+=seperator.length();
			count++;
			pi+=m;
			n=sentence.length()-pi;
		}
		else
		{
			if(n==2 && (unsigned char)sentence[pi]>=128) //ȫ���ַ�
			{
				sentence.insert(pi+2,seperator);
				pi+=seperator.length();
				count++;
				pi+=2;
				n=sentence.length()-pi;
			}
			else if(n==1 && (unsigned char)sentence[pi]<128) //����ַ�
			{
				sentence.insert(pi+1,seperator);
				pi+=seperator.length();
				count++;
				pi++;
				n=sentence.length()-pi;
			}
			else
				n--;
		}
	}
	return count;
}
int FMM::divide_sentence(string &sentence,string seperator)
{
	if(dict_index.len_max==0)
		createDictIndex(false);
	int count=0;

	int pi=sentence.length();//ָ���Ӵ�β
	int m=dict_index.len_max;//�ʵ�������ַ�����
	int n=pi;//ʣ�����ֵĳ���
	string tword;//��ǰ����
	while(n >= 1)
	{
		m=dict_index.len_max;
		if(n<m) m=n;
		tword=sentence.substr(pi-m,m);
		if(searchWord(tword)!=-1)//�ҵ�ƥ��
		{
			sentence.insert(pi,seperator);
			count++;
			pi-=m;
			n=pi;
		}
		else
		{
			if(n==2 && (unsigned char)sentence[pi-1]>=128) //ȫ���ַ�
			{
				sentence.insert(pi,seperator);
				count++;
				pi-=2;
				n=pi;
			}
			else if(n==1 && (unsigned char)sentence[pi-1]<128) //����ַ�
			{
				sentence.insert(pi,seperator);
				count++;
				pi--;
				n=pi;
			}
			else
				n--;
		}
	}
	return count;	 
}