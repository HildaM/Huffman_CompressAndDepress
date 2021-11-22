/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#pragma once

#include <bits/stdc++.h>

using namespace std;

#include "Huffman_struct.h"

class Depression {
public:
	// ��ȡ�ļ�
	Tree ReadFile(string path1, unsigned int& leafNumber, string& searchPath, int& Sign);

	// ��ԭ��������
	int BuiltHuffmanTree(Node HuffmanTree[], int leafNumber);

	// ��ԭ�ļ�
	int RestoreFile(string searchPath, Node HuffmanTree[], string path2, int leafNumber);

	// ���Է��������������������Ϣ
	int ShowTable(Node HuffmanTree[], int LeafNumber);
};