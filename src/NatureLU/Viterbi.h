/*
  ά�ر��㷨�������������̬����
  Ҫ��trans��emits�����������Ķ�ά����
*/
#ifndef _VITERBI_H
#define _VITERBI_H
//������ʽ������ֵ�ǳ���Ϊnum_obs���ϵ�����
int* Viterbi(int num_obs,int num_states,int len_obs,
			 int *obs,int *states,
			 double *start_p,double **trans,double **emit);

//����ʽ��
//�������ͬģ�͵��ò����ظ������ڴ棬���Ч�ʣ�
//����ʵ��hmm�㷨Ч�ʻ����ԣ���ʱ��д�����
class Viterbi
{

};
#endif