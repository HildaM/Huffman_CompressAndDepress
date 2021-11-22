/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#include "Depression.h"

// 读取文件
Tree Depression::ReadFile(string path1, unsigned int& leafNumber, string& searchPath, int& Sign) {
	// Sign的作用是记录末尾到底有多少个多余的0，方便等下删去

	ifstream inStr(path1, ios::in | ios::binary);
	searchPath = "";

	// 读取不满8位的个数
	char ch;
	inStr.get(ch);
	Sign = ch;

	// 读取有多少个叶子节点
	inStr.get(ch);
	leafNumber = (int)(unsigned char)ch;
	leafNumber += 1;

	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            是叶节点
	// leafNumber--->2*LeafNumber - 2 是根节点
	Tree HuffmanTree = new Node[2 * leafNumber - 1];
	unsigned int num;

	// 读取叶子节点信息（以节点数leafNumber进行划分）
	for (int i = 0; i < leafNumber; i++) {
		// 读取ASCII码
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].info = num;

		// 读取父节点数据
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].parent = num + leafNumber;  // 因为在写入信息中，减掉了leafNumber，所以此处要加上

		// 读取节点位置信息
		inStr.get(ch);
		HuffmanTree[i].side = ch;

		// 初始化其他信息
		HuffmanTree[i].BinaryCode = "";
		HuffmanTree[i].index = i;
		HuffmanTree[i].left = -1;
		HuffmanTree[i].right = -1;
		HuffmanTree[i].weight = -1;
	}

	// 读取根节点的信息
	for (int i = leafNumber; i < 2 * leafNumber - 1; i++) {
		// 读取父节点信息
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].parent = num + leafNumber;

		// 读取节点位置信息
		inStr.get(ch);
		HuffmanTree[i].side = ch;

		// 初始其他信息
		HuffmanTree[i].info = -10000;
		HuffmanTree[i].BinaryCode = "";
		HuffmanTree[i].index = i;
		HuffmanTree[i].left = -1;
		HuffmanTree[i].right = -1;
		HuffmanTree[i].weight = -1;
	}

	// 读取二进制信息
	int bitMask = 0x80;  // 0x80等于十进制的128 二进制：1000 0000 
	while (inStr.get(ch)) {
		num = (int)(unsigned char)ch;
		while (bitMask != 0) {
			// 进行二进制位运算
			if ((bitMask & num) != 0) {
				searchPath += '1';
			}
			if ((bitMask & num) == 0) {
				searchPath += '0';
			}
			bitMask >>= 1;
			/*
			* 上面一行是bitMask，下面一行是num，也就是转换出来的二进制码，最后一行是serachPath，作为存储num的字符串
			* 使用位运算将num的数据“导出来”，bitMask就是用来辅助位运算的！
				1000 0000	右移	   0100 0000   右移	  0010 0000
				0101 0010  ----->  0101 0010  ----->  0101 0010
				---------		   ---------		  ---------
				0				   01				  010
			*/
		}
		// 重新置为 1000 0000，为下一轮运算做准备
		bitMask = 0x80;
	}

	// 清除末尾多余的0
	searchPath.erase(searchPath.end() - Sign, searchPath.end());

	inStr.close();

	cerr << "Read File Completed" << endl;
	return HuffmanTree;
}

// 还原哈夫曼树
int Depression::BuiltHuffmanTree(Node HuffmanTree[], int leafNumber) {
	for (int i = 0; i < 2 * leafNumber - 1; i++) {
		// 找到该节点的父节点，并将父节点的left置为自己
		if (HuffmanTree[i].side == 'l')
			HuffmanTree[HuffmanTree[i].parent].left = i;
		// 找到该节点的父节点，并将父节点的right置为自己
		if (HuffmanTree[i].side == 'r')
			HuffmanTree[HuffmanTree[i].parent].right = i;
	}

	cerr << "Built Huffman Tree Completed" << endl;
	return 0;
}

// 还原文件
int Depression::RestoreFile(string searchPath, Node HuffmanTree[], string path2, int leafNumber) {
	ofstream outStr(path2, ios::out | ios::binary);

	// 哈夫曼树的根节点，就是哈夫曼数组的最后一个数据
	int head = 2 * leafNumber - 2, now = head;

	// 根据searchPath中的二进制码，遍历哈夫曼树，找到叶子节点
	for (int i = 0; i < searchPath.size(); i++) {
		// 根据二进制码进行遍历
		if (searchPath[i] == '0')
			now = HuffmanTree[now].left;  // 这里就相当于以前的 cur = cur->left
		if (searchPath[i] == '1')
			now = HuffmanTree[now].right; // 相当于 cur = cur->right
		// 找到叶子节点
		if (HuffmanTree[now].left == -1 && HuffmanTree[now].right == -1) {
			char res = HuffmanTree[now].info;
			outStr.put(res);
			now = head;		// 重新回到根节点
		}
	}

	outStr.close();

	cerr << "Restore File Completed" << endl;
	return 0;
}

// 测试方法：输出哈夫曼树的信息
int Depression::ShowTable(Node HuffmanTree[], int leafNumber) {
	for (int i = 0; i < 2 * leafNumber - 1; i++) {
		cout << "i:" << i << endl;
		cout << HuffmanTree[i].index << "<-index" << endl;
		cout << HuffmanTree[i].info << "<-info" << endl;
		cout << HuffmanTree[i].side << "<-side" << endl;
		cout << HuffmanTree[i].left << "<-left" << endl;
		cout << HuffmanTree[i].right << "<-right" << endl;
		cout << HuffmanTree[i].parent << "<-parent" << endl;
	}
	return 0;
}