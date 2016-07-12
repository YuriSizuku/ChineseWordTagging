#include <iostream>
#include<fstream>
#include<string>
#include<set>
#include "DivideWord.h"
using namespace std;

/*��̬��������ֵ(����)*/
struct Dict DivideWord::dict_chs={{0},0};
struct DictIndex DivideWord::dict_index={{0},0};
bool DivideWord::useDictSet=false;
set<string> DivideWord::dict_set;//����Ҫ�ڴ�����������������Ӵ���

/*��̬����*/
void DivideWord::iniDict()
{
	dict_chs.size=0;
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(dict_chs.dict_array[i].word!=NULL)
			delete[]dict_chs.dict_array[i].word;
		dict_chs.dict_array[i].word=NULL;
		dict_chs.dict_array[i].freq=0;
	}
}
int DivideWord::getWordCount()
{
	return dict_chs.size;
}
int DivideWord::saveDict(const char *path)
{
	ofstream fout(path,ios::out);
	if(fout.fail()) return -1;
	for(int i=0;i<dict_chs.size;i++)
	{
		fout<<i+1<<" "<<dict_chs.dict_array[i].word<<" "<<dict_chs.dict_array[i].freq<<endl;
	}
	fout.close();
	return 0;
}
int DivideWord::loadDict(const char *path,bool useDictSet)
{
	ifstream fin(path,ios::in);
	if(fin.fail()) return -1;
	int i,freq;
	string word;
	char *str;
	iniDict();
	while(!fin.eof())
	{
		fin>>i>>word>>freq;
		str=new char[word.length()+1];//�ⲽ����word.length()��ʱrelease�����
		str[word.length()]=0;
		strcpy(str,word.c_str());
		dict_chs.dict_array[dict_chs.size].freq=freq;
		dict_chs.dict_array[dict_chs.size].word=str;
		dict_chs.size++;
		//printf("debug2...%d_%d_%d_%s\n",dict_chs.size,word.length(),strlen(str),str);
	}
	fin.close();
	if(useDictSet==true)
		createDictSet();
	return 0;
}
int DivideWord::addWord(const string &word)
{
	for(int i=0;i<dict_chs.size;i++)
	{
		if(strcmp(word.c_str(),dict_chs.dict_array[i].word)==0)
		{
			dict_chs.dict_array[i].freq++;
			return i;
		}
	}
	char *str=new char[word.length()+1];
	str[word.length()]=0;
	strcpy(str,word.c_str());
	dict_chs.dict_array[dict_chs.size].word=str;
	dict_chs.dict_array[dict_chs.size].freq++;
	return ++dict_chs.size;
}
int DivideWord::createDictFromCorpus(const char *path,bool useDictSet)
{
	ifstream fin(path,ios::in);
	if(fin.fail()) return -1;
	long count=0;
	int pos,pos2,tmp;
	string line;
	string word;
	string tip;
	iniDict();
	while(getline(fin,line))
	{
		pos=line.find("/",0);
		while(pos!=string::npos)
		{
			tip="";
			word="";
			pos2=pos;
			while(1)
			{
				if(pos2 >= line.length()) break;
				if(line.at(pos2)==' ') break;
				pos2++;
			}
			if(pos2-pos>0)
				tip=line.substr(pos,pos2-pos);
			
			if(tip!="/m" && tip!="/w" && tip!="/s" && tip!="")//����
			{
				pos2=pos;
				while(1)
				{
					if(pos<=0) break;
					if(line.at(pos2)==' ') break;
					pos2--;
				}
				if(pos2+1>=0)
					word=line.substr(pos2+1,pos-pos2-1);
				if(word!="")
					addWord(word);
			}
			pos=line.find("/",pos+1);
		}
		count++;
	}
	fin.close();
	if(useDictSet==true)
		createDictSet();
	return count;
}
void DivideWord::createDictIndex(bool isSorted)
{
	if(!isSorted) sortDict();
	int len=strlen(dict_chs.dict_array[0].word);
	memset(dict_index.len_shift,0,sizeof(dict_index.len_shift));
	dict_index.len_max=len;
	dict_index.len_shift[len].start=0;
	for(int i=0;i<dict_chs.size;i++)
	{
		if(len!=strlen(dict_chs.dict_array[i].word))
		{
			dict_index.len_shift[len].end=i;
			len=strlen(dict_chs.dict_array[i].word);
			dict_index.len_shift[len].start=i;
		}
	}
	dict_index.len_shift[len].end=dict_chs.size;
}
void DivideWord::createDictSet(bool useDictSet)
{
	DivideWord::useDictSet=useDictSet;
	string word;
	for(int i=0;i<dict_chs.size;i++)
	{
		word=dict_chs.dict_array[i].word;
		dict_set.insert(word);
	}
}
void DivideWord::distoryDictSet()
{
	dict_set.clear();
	useDictSet=false;
}
void DivideWord::sortDict()
{
	qsort(dict_chs.dict_array,dict_chs.size,sizeof(DictNode),_comp_1);
}
int DivideWord::destroyDict()
{
	int i;
	for(i=0;i<dict_chs.size;i++)
	{
		dict_chs.dict_array[i].freq=0;
		delete[] dict_chs.dict_array[i].word;
	}
	dict_chs.size=0;
	return i;
}

/*��������*/
int DivideWord::load_passage(const char *path,string &passage)
{
	ifstream fin(path);
	if(fin.fail()) return -1;
	string line;
	passage="";
	while(getline(fin,line))
	{
		passage+=line+"\n";
	}
	fin.close();
	return 0;
}
int DivideWord::save_passage(const char *path,string &passage)
{
	ofstream fout(path);
	if(fout.fail()) return -1;
	fout<<passage;
	fout.close();
	return 0;
}
int DivideWord::searchWord(const string &word)
{
	if(useDictSet)
	{
		set<string>::iterator iter;
		iter=dict_set.find(word);
		if(iter!=dict_set.end())
		{
			return 1;//����set���Һܿ죬���ٷ��ؾ���λ��;
		}
	}
	else
	{
		int len=word.length();
		int res;
		for(int i=dict_index.len_shift[len].start;i<dict_index.len_shift[len].end;i++)
		{
			//if(word==dict_chs.dict_array[i].word)
			//	return i;
			res=strcmp(word.c_str(),dict_chs.dict_array[i].word);
			if(res==0) return i;
			else if(res==1) break;
		}
	}
	return -1;
}

/*�麯��*/
int DivideWord::word2num(const string &word)
{
	if(word.length()!=2) return -1;
	return word2num(word[0],word[1]);
}
int DivideWord::word2num(char c1,char c2)
{
	int dchar;
	dchar=(unsigned char)c1-0xa0;
	dchar*=100;//�����λ��10����4λ���ʲ�������16������
	dchar+=(unsigned char)c2-0xa0;
	//if(dchar<0) dchar+=NUM_OBS;//���⸺���쳣
	return dchar;
}
int DivideWord::divide_passage(const std::string &str_in,std::string &str_out,std::string seperator)
{
	int count=0;
	int start=0,i,j;
	int flag=0;//0���֣�1ȫ�ǷǺ��֣�2����ַ�
	unsigned int dchar=0;//˫�ֽ��ַ�����
	string sentence;
	str_out="";
	
	for(i=0;i <= str_in.length();)
	{
		if((unsigned char)str_in[i]<128)//����ַ�
		{
			if(flag==0)
			{	
				if(i-start>0)
				{
					sentence=str_in.substr(start,i-start);
					//count+=(this->*func_divide)(sentence,seperator);
					//�����ຯ��ָ��,����������÷��з������ֻص�����������麯�����
					this->divide_sentence(sentence,seperator);
					str_out+=sentence;
				}
				flag=2;
			}
			if(str_in[i]=='\n')//����,�����ö����ƶ�ȡ����0x0a,0x0d
			{
				str_out+=(char)str_in[i];
			}
			else if(str_in[i]==' ')//�ո�
			{
				str_out+=(char)str_in[i];
			}
			else if(ispunct(str_in[i]))//���
			{
				str_out+=(char)str_in[i];
				str_out+=seperator;
			}
			else//�����ĸ������
			{
				str_out+=(char)str_in[i];
				if(i+1 > str_in.length())
				{
					str_out+=seperator;
				}
				else
				{
					if(!(str_in[i+1]>='a' && str_in[i+1]<='z' 
						|| str_in[i+1]>='A' && str_in[i+1]<='Z')
						|| str_in[i+1]>='0' && str_in[i+1]<='9')
						str_out+=seperator;//�ָ�����ַ����ʡ�����
				}
			}
			i++;
		}
		else//ȫ���ַ�
		{
			dchar^=dchar;
			dchar|=(unsigned char)str_in[i];
			dchar<<=8;
			dchar|=(unsigned char)str_in[i+1];
			if(dchar >= 0xa1a0 && dchar <= 0xa9f0)//���ı����ŵı��뷶Χ��:0xA13F~0xAA40
			{
				if(flag==0)
				{
					if(i-start>0)
					{
						sentence=str_in.substr(start,i-start);
						this->divide_sentence(sentence,seperator);
						str_out+=sentence;
					}
					flag=1;
				}
				str_out+=str_in.substr(i,2);
				if(dchar >= 0xa3b0 && dchar <=0xa3b9 /*0~9*/
					|| dchar >=0xa3c1 && dchar<=0xa3da /*a~z*/
					|| dchar >= 0xa3e1 && dchar <= 0xa3fa/*A~Z*/)
				{
					if(i+3 > str_in.length())
						str_out+=seperator;
					else
					{
						dchar^=dchar;
						dchar|=(unsigned char)str_in[i+2];
						dchar<<=8;
						dchar|=(unsigned char)str_in[i+3];
						if(!(dchar >= 0xa3b0 && dchar <=0xa3b9 /*0~9*/
						|| dchar >=0xa3c1 && dchar<=0xa3da /*a~z*/
						|| dchar >= 0xa3e1 && dchar <= 0xa3fa/*A~Z*/))
						str_out+=seperator;
					}
				}
				else
					str_out+=seperator;
			}
			else
			{
				if(flag==1 || flag==2)
				{
					start=i;
					flag=0;
				}
			}
			i+=2;
		}
	}
	return count;
}
int DivideWord::divide_file(const char *inpath,const char *outpath,std::string seperator)
{
	int count=0;
	string str_in,str_out;
	load_passage(inpath,str_in);
	count=divide_passage(str_in,str_out,seperator);
	save_passage(outpath,str_out);
	return count;
}
void DivideWord::eval_line(string &test,string &answer,
							int &num_test,int &num_answer,int &num_correct,string seperator)
{
	set<string> answer_word;
	string word;
	int pos,pos2;

	pos=0;
	pos2=answer.find(seperator);
	num_answer=0;
	while(pos2!=string::npos)
	{
		word=answer.substr(pos,pos2-pos);
		answer_word.insert(word);
		num_answer++;
		pos=pos2+seperator.length();
		pos2=answer.find(seperator,pos2+seperator.length());
	}
	//num_answer=answer_word.size();//�������û���ظ��ʵ�
	num_test=0;
	num_correct=0;
	pos=0;
	pos2=test.find(seperator);
	while(pos2!=string::npos)
	{
		word=test.substr(pos,pos2-pos);
		num_test++;//���Ҳ�㻮�ְɡ���
		if(answer_word.count(word)>0)//Ԫ�ش���
			num_correct++;
		pos=pos2+seperator.length();
		pos2=test.find(seperator,pos2+seperator.length());
	}

}
void DivideWord::eval_line(string &test,string &answer,
							double &correct_ratio,double &recall_ratio,string seperator)
{
	int num_test,num_answer,num_correct;
	eval_line(test,answer,num_test,num_answer,num_correct,seperator);
	correct_ratio=(double)num_correct/(double)num_test;
	recall_ratio=(double)num_correct/(double)num_answer;
}
int DivideWord::eval_file(const char *path_test,const char *path_answer,
							int &num_test,int &num_answer,int &num_correct,string seperator)
{
	int num_ttest,num_tanswer,num_tcorrect;
	int flag=0;
	
	num_test=0;num_answer=0;num_correct=0;
	string line_test,line_answer;
	
	ifstream fin_test(path_test),fin_answer(path_answer);
	if(fin_test.fail() || fin_answer.fail())
		return -1;//���ļ�ʧ��
	while(getline(fin_answer,line_answer) && getline(fin_test,line_test))
	{
		if(flag==1) break;
		eval_line(line_test,line_answer,num_ttest,num_tanswer,num_tcorrect,seperator);
		num_correct+=num_tcorrect;
		num_test+=num_ttest;
		num_answer+=num_tanswer;
	}
	fin_test.close();
	fin_test.close();
	return 0;
}
int DivideWord::eval_file(const char *path_test,const char *path_answer,
							double &correct_ratio,double &recall_ratio,string seperator)
{
	int num_test,num_answer,num_correct;
	int res;
	res=eval_file(path_test,path_answer,num_test,num_answer,num_correct,seperator);
	correct_ratio=(double)num_correct/(double)num_test;
	recall_ratio=(double)num_correct/(double)num_answer;
	return res;
}