/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#pragma once

#include <bits/stdc++.h>

using namespace std;

#include "Huffman_struct.h"

class Compression {
public:
	// ������������
	int CreateHuffmanTree(int tong[], int leafNumber, Node HuffmanTree[]);

	// �Գ��ֵ��ַ����б���
	int GetCodeNode(Node HuffmanTree[], int leafNumber);

	// ���ļ����б���
	string Encode(string path1, Node HuffmanTree[], int leafNumber);

	// ͳ���ı����ַ����ֵĴ���
	int Count(string op, string path1, string path2, int tong[]);

	// ���ַ����е� 01 ������ת��
	string SwithcStringToBinary(string HuffmanPath, int& Sign);

	// ��ȡ��ǰ�ڵ��λ����Ϣ
	int GetSide(Node HuffmanTree[], int leafNumber);

	// д���ļ�
	int WriteToFile(string path2, Node HuffmanTree[], int leafNumber, string binaryPath, int Sign);

	// ���Է��������������������Ϣ
	int ShowTable(Node HuffmanTree[], int leafNumber);
};