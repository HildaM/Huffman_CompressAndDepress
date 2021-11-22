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
	// 读取文件
	Tree ReadFile(string path1, unsigned int& leafNumber, string& searchPath, int& Sign);

	// 还原哈夫曼树
	int BuiltHuffmanTree(Node HuffmanTree[], int leafNumber);

	// 还原文件
	int RestoreFile(string searchPath, Node HuffmanTree[], string path2, int leafNumber);

	// 测试方法：输出哈夫曼树的信息
	int ShowTable(Node HuffmanTree[], int LeafNumber);
};