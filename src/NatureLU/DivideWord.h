#include<set>

#ifndef _DIVIDEWORD_H
#define _DIVIDEWORD_H
#define MAX_SIZE 10000000
#define MAX_LEN 100
/*
	����Ϊ����ִʵĻ���
	���������е�����������ʽasci������gb2312��
*/
struct DictNode
{
	char *word;
	int freq;
};
struct Dict
{
	struct DictNode dict_array[MAX_SIZE];
	int size;
};
struct DictIndex
{
	struct
	{
		int start;
		int end;
	}len_shift[MAX_LEN];
	int len_max;
};
class DivideWord
{
	protected:
		static inline int _comp_1(const void *a,const void *b)
		{
			DictNode *p1=(DictNode*)a;
			DictNode *p2=(DictNode*)b;
			int len1=strlen(p1->word);
			int len2=strlen(p2->word);
			if(len1!=len2)
				return len2-len1;
			//else if(p2->freq !=p1->freq)
			//	return p2->freq - p1->freq;
			else return strcmp(p2->word,p1->word);
		}
		static int addWord(const std::string &word);
		static struct Dict dict_chs;
		static struct DictIndex dict_index;
	
		static std::set<std::string> dict_set;
	public:
		static bool useDictSet;//�����Ƿ����ֵ伯�Ͻṹ

		//�ֵ��������
		static void iniDict();
		static int getWordCount();
		static int saveDict(const char *path);
		static int loadDict(const char *path,bool useDictSet=true);
		static void createDictIndex(bool isSorted=true);
		static int createDictFromCorpus(const char *path,bool useDictSet=true);//�����Ͽⴴ���ʵ�
		static void sortDict();
		static int destroyDict();

		//�ֵ伯�ϲ�������
		static void createDictSet(bool useDictSet=true);
		static void distoryDictSet();
		
		//������������غ���
		int load_passage(const char *path,std::string &passage);
		int save_passage(const char *path,std::string &passage);
		int searchWord(const std::string &word);//����������شʵ��е�����ֵ
		
		//�������麯��������Ӱ�첻��
		//ת������
		virtual int word2num(const std::string &word);//��asci��ת��Ϊ����������->��λ�룩
		virtual int word2num(char c1,char c2);
		//�ִʺ���
		virtual int divide_sentence(std::string &sentence,std::string seperator)=0;//��û�б�����
		virtual int divide_passage(const std::string &str_in,std::string &str_out,std::string seperator="/");//��һ���ַ��������б�㣩
		virtual int divide_file(const char *inpath,const char *outpath,std::string seperator="/");
		//���ۺ���
		virtual void eval_line(std::string &test,std::string &answer,
								int &num_test,int &num_answer,int &num_correct,std::string seperator);
		virtual void eval_line(std::string &test,std::string &answer,
								double &correct_ratio,double &recall_ratio,std::string seperator);
		virtual int eval_file(const char *path_test,const char *path_answerint,
								int &num_test,int &num_answer,int &num_correct,std::string seperator);
		virtual int eval_file(const char *path_test,const char *path_answer,
								double &correct_ratio,double &recall_ratio,std::string seperator);
		
};
#endif