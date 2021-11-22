
#pragma once

#include<string>
using namespace std;

struct HuffmanNode {
	int info;		// ��Ϣ
	int index;		// ���� ?
	int weight;		// Ȩ��
	int parent;		// ���ڵ�
	int left;		// ��ڵ�
	int right;		// �ҽڵ�
	char side;
	string BinaryCode;	// ת�������Ķ�������

	// �������ؼƽϷ�
	friend bool operator> (HuffmanNode f1, HuffmanNode f2) {
		return f1.weight > f2.weight;
	}
};

typedef HuffmanNode Node;
typedef HuffmanNode* Tree;	// Ϊ�ṹ��ָ�뻻����