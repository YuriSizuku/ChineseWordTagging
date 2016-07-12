#include<map>
#include "TagWord.h"
#include "Viterbi.h"
#ifndef _HMMWORD_H
#define _HMMWORD_H
#pragma comment(linker, "/STACK:5000000000 ")//�ø��������/STACK:��������Ϊ���� (Ĭ����1m)
const int NUM_WORD=55000;//��������
class HMMword:public TagWord
{
	protected:
		double start_p[NUM_TAG];
		double trans_p[NUM_TAG][NUM_TAG];
		double emit_p[NUM_TAG][NUM_WORD];
		std::map<std::string,int> word_map;//����ӳ��

		void count2prob(int start[],int trans[][NUM_TAG],int emit[][NUM_WORD]);//��������ת��Ϊ���ʾ���
	public:
		HMMword(DivideWord *spliter):TagWord(spliter){};
		HMMword(DivideWord *spliter,const char *path_hmm,const char *path_wordmap);
		int train_hmm(const char *path);//ѵ����ע������ɷ�ģ�ͣ������Ͽ⣩
		int tagword_sentence(std::string &sentence,std::string seperator="/");//��ע
		int divide_sentence(std::string &sentence,std::string seperator);//�ִ�
		
		int save_args(const char *path_hmm,const char *path_wordmap);//�洢����
		int load_args(const char *path_hmm,const char *path_wordmap);//��ȡ����
		int save_hmm(const char *path);//��ȡhmmģ�Ͳ���
		int load_hmm(const char *path);//�洢hmmģ�Ͳ���
		int save_wordmap(const char *path);//�洢ӳ���
		int load_wordmap(const char *path);//��ȡӳ���
};
#endif