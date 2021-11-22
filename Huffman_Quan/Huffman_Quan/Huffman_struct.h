
#pragma once

#include<string>
using namespace std;

struct HuffmanNode {
	int info;		// 信息
	int index;		// 索引 ?
	int weight;		// 权重
	int parent;		// 父节点
	int left;		// 左节点
	int right;		// 右节点
	char side;
	string BinaryCode;	// 转换而来的二进制码

	// 内置重载计较符
	friend bool operator> (HuffmanNode f1, HuffmanNode f2) {
		return f1.weight > f2.weight;
	}
};

typedef HuffmanNode Node;
typedef HuffmanNode* Tree;	// 为结构体指针换名字