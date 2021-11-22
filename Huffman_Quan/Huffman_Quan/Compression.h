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
	// 创建哈夫曼树
	int CreateHuffmanTree(int tong[], int leafNumber, Node HuffmanTree[]);

	// 对出现的字符进行编码
	int GetCodeNode(Node HuffmanTree[], int leafNumber);

	// 对文件进行编码
	string Encode(string path1, Node HuffmanTree[], int leafNumber);

	// 统计文本中字符出现的次数
	int Count(string op, string path1, string path2, int tong[]);

	// 把字符串中的 01 串进行转换
	string SwithcStringToBinary(string HuffmanPath, int& Sign);

	// 获取当前节点的位置信息
	int GetSide(Node HuffmanTree[], int leafNumber);

	// 写入文件
	int WriteToFile(string path2, Node HuffmanTree[], int leafNumber, string binaryPath, int Sign);

	// 测试方法：输出哈夫曼树的信息
	int ShowTable(Node HuffmanTree[], int leafNumber);
};