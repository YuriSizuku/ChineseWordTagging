#include "Viterbi.h"
#include <iostream>
int* Viterbi(int num_obs,int num_states,int len_obs,
			 int *obs,int *states,
			 double *start_p,double **trans,double **emit)//Ҫ��trans��emits�����������Ķ�ά����
{
	/*��������*/
	double **v=new double*[len_obs];			//ÿ��״̬�ĸ���
	int **path;									//������״̬��������·����ָ��
	int **tpath;								//dp�õ���ʱ·����ָ��
	int ***path_data=new int**[2];				//����·��������
	double **trans_p=new double*[num_states];	//���ݾ���
	double **emit_p=new double*[num_states];	//�������
	int y,y0;									//����̬
	int i,j,t;
	

	path_data[0]=new int*[num_states];path_data[1]=new int*[num_states];
	path=path_data[0];tpath=path_data[1];

	for(i=0;i<len_obs;i++)
		v[i]=new double[num_states];
	for(i=0;i<num_states;i++)
	{
		path[i]=new int[len_obs];
		tpath[i]=new int[len_obs];
		trans_p[i]=(double*)trans+i*num_states;
		emit_p[i]=(double*)emit+i*num_obs;
	}
	/*END*/
	
	//��ʼ������
	for(i=0;i<num_states;i++)
	{
		y=states[i];
		v[0][y]=start_p[y] * emit_p[y][obs[0]];
		path[y][0]=y;//����ʼ��״̬·����Ϊ����
	}
	//dp
	for(t=1;t<len_obs;t++)
	{
		for(i=0;i<num_states;i++)//t��״̬
		{
			y=states[i];
			double prob=-1.0;//������
			for(j=0;j<num_states;j++)//t-1��״̬
			{
				y0=states[j];
				//double tprob=v[t-1][y0] * trans_p[y0][y] * emit_p[y][obs[t]];//�������ˣ����ܻ����
				double tprob=v[t-1][y0] * trans_p[y0][y];
				tprob*=emit_p[y][obs[t]];
				if(tprob > prob)
				{
					prob=tprob;
					v[t][y]=prob;
					//memcpy(tpath[y],path[y0],t*sizeof(double));//����ʹ��ջ��
					for(int k=0;k<t;k++)//����·��,�����е�����·��
						tpath[y][k]=path[y0][k];
					tpath[y][t]=y;//����t��y״̬��·��Ϊ����һ����������һ��·��  �� y��
				}
			}
		}
		int **temp;
		temp=tpath;tpath=path;path=temp;//����·��������
	}

	//ȡ�ø���������
	double prob=-1.0;
	int *result;
	for(i=0;i<num_states;i++)
	{
		y=states[i];
		if(v[len_obs-1][y]>prob)
		{
			prob=v[len_obs-1][y];
			result=path[y];
		}
	}

	/*�ͷű���*/
	for(i=0;i<len_obs;i++)
		delete[] v[i];
	for(i=0;i<num_states;i++)
	{
		delete[] tpath[i];
		if(result!=path[i]) delete[] path[i];
	}
	delete[] v;
	delete[] path_data[0];
	delete[] path_data[1];
	delete[] path_data;
	delete[] trans_p;
	delete[] emit_p;
	/*END*/
	return result;//�������ո������״̬��·����
}