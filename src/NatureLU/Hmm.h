#include "DivideWord.h"
#include"Viterbi.h"
#ifndef _HMM_H
#define _HMM_H

//����ͷ�ļ��ж���const��������ֹ�ض���(���У������Ͳ��������С��)
const int NUM_STATE=4;      //����̬��������B,M,E,S
const int NUM_OBS=10000;    //�۲�̬����������������

class HMM :public DivideWord
{
	/*�����˺���ֵ����⣬ѭ���������ʱ��asciֵ���ԣ�ʹ��unicode�ַ������Թ�ȥ
	*/
	protected:
		double start_p[NUM_STATE];					//��ʼ����̬����(BMES)
		double trans_p[NUM_STATE][NUM_STATE];		//״̬ת�ƾ���
		double emit_p[NUM_STATE][NUM_OBS];			//����������
		
		int word2num(const std::string &word);//��asci��ת��Ϊ����������->��λ�룩
		int word2num(char c1,char c2);
		int char2state(char c);//ͨ��B,M,E,S ���״̬����
		void count2prob(int start[],int trans[][NUM_STATE],int emit[][NUM_OBS]);//��������ת��Ϊ���ʾ���
	public:
		enum WordState{B,M,E,S};

		HMM();
		HMM(const char *hmm_path);

		int divide_sentence(std::string &sentence,std::string seperator="/");
		int createTrainFromCorpus(const char *inpath,const char *outpath);//�����Ͽⴴ��hmmѵ����train_hmm��ѵ����(��ʱ��д�κ���)
		int train_hmm(const char *path);
		//û�б�㣬û����������ַ���ÿ����֮������� /B,/M,/E,/S ��ǩ���ı���ѵ��hmmģ��
		//����/B��/E  ��/S  ˵/S  ��/B��/E  ��/S  ��/S  ��/B��/M��/E��������������ģ��ȥѵ��
		int save_hmm(const char *path);//��ȡhmmģ�Ͳ���
		int load_hmm(const char *path);//�洢hmmģ�Ͳ���
};
#endif