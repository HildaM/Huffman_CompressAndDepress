/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#include "Compression.h"

using namespace std;

// 创建哈夫曼树
int Compression::CreateHuffmanTree(int tong[], int leafNumber, Node HuffmanTree[]) {
	int k = 0;
	// 使用优先队列存储对节点进行动态排序。按照内置在Node中的规则进行排序
	// greater: 升序（默认为降序）
	priority_queue<Node, vector<Node>, greater<Node>> pq;

	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            是叶节点
	// leafNumber--->2*LeafNumber - 2 是根节点

	// 对哈夫曼树进行初始化
	for (int i = 0; i <= 256; i++) {
		// 只有出现过的字符才进行记录
		if (tong[i] != 0) {
			HuffmanTree[k].info = i;
			HuffmanTree[k].index = k;
			HuffmanTree[k].left = HuffmanTree[k].right =
				HuffmanTree[k].parent = -1;
			HuffmanTree[k].weight = tong[i];

			pq.push(HuffmanTree[k]);
			k++;
		}
	}

	int j = leafNumber;
	// 通过优先队列创建哈夫曼树
	while (pq.size() > 1) {
		// 此时弹出的必然是weight最小的两个节点
		Node t1 = pq.top();	pq.pop();
		Node t2 = pq.top();	pq.pop();

		HuffmanTree[t1.index].parent = j;
		HuffmanTree[t2.index].parent = j;

		HuffmanTree[j].index = j;
		HuffmanTree[j].parent = -1;
		HuffmanTree[j].left = t1.index;		// left在哈夫曼树中是小于right的，所以left要为t1.index
		HuffmanTree[j].right = t2.index;
		HuffmanTree[j].weight = t1.weight + t2.weight;
		HuffmanTree[j].info = -127;

		pq.push(HuffmanTree[j]);
		j++;
	}

	// 对最后一个节点进行单独处理
	j--;
	Node HuffmanTreeHead = pq.top();
	pq.pop();
	HuffmanTreeHead.parent = -127;
	HuffmanTree[j] = HuffmanTreeHead;
	HuffmanTree[j].info = -127;

	cerr << "Create Huffman Tree Completed" << endl;
	return 0;
}

// 对出现的字符进行编码
int Compression::GetCodeNode(Node HuffmanTree[], int leafNumber) {
	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            是叶节点
	// leafNumber--->2*LeafNumber - 2 是根节点
	for (int i = 0; i < 2 * leafNumber - 2; i++) {
		// 不是叶子节点的继续遍历
		if (HuffmanTree[i].info == -127) continue;

		int indexForSearching = i;
		HuffmanTree[i].BinaryCode = "";

		// 从叶子节点出发，向上遍历，直到根节点
		int j = 0;
		while (HuffmanTree[indexForSearching].parent != -127) {
			j = HuffmanTree[indexForSearching].parent;
			// 进行路径判断，使自己的parent的才记录
			if (HuffmanTree[j].left == indexForSearching)
				HuffmanTree[i].BinaryCode += '0';
			if (HuffmanTree[j].right == indexForSearching)
				HuffmanTree[i].BinaryCode += '1';
			indexForSearching = j;
		}

		// 到达根基的后，BinaryCode是倒叙的，所以需要转换为正序
		reverse(HuffmanTree[i].BinaryCode.begin(),
			HuffmanTree[i].BinaryCode.end());
	}

	cerr << "Get Node BinarryCode Completed" << endl;
	return 0;
}

// 对文件进行编码
string Compression::Encode(string path1, Node HuffmanTree[], int leafNumber) {
	// 打开目标文件的文件流
	ifstream inStr(path1, ios::in | ios::binary);

	char ch;
	unsigned int byteBuff = 0;
	int bitMask = 0x80;
	string HuffmanPath = "";

	while (inStr.get(ch)) {
		byteBuff = (int)(unsigned char)ch;	// 转换为ACSII码，方便比对
		int value = byteBuff;
		for (int i = 0; i < leafNumber; i++) {
			if (HuffmanTree[i].info == value) {
				HuffmanPath += HuffmanTree[i].BinaryCode;
				break;
			}
		}
	}

	cerr << "Encode Completed" << endl;
	// 返回整个文件的二进制编码
	return HuffmanPath;
}

// 统计文本中字符出现的次数
int Compression::Count(string op, string path1, string path2, int tong[]) {
	// path1: 源文件  path2：目标文件

	// 文件输入流：以 in 方式、二进制打开
	ifstream inStr(path1, ios::in | ios::binary);

	unsigned int byteBuff = 0;
	char ch;
	while (inStr.get(ch)) {
		byteBuff = (int)(unsigned char)ch;	// 将读取出来的字符，先转为char，再转为int保存
		tong[byteBuff]++;
	}

	int leafNumber = 0;
	for (int i = 0; i <= 256; i++) {
		if (tong[i] != 0) leafNumber++;
	}

	inStr.close();

	/*
		cerr的特点：
		cerr不经过缓冲而直接输出，一般用于迅速输出出错信息，是标准错误，默认情况下被关联到标准输出流，
		但它不被缓冲，也就说错误消息可以直接发送到显示器
	*/
	cerr << "Count Completed" << endl;
	return leafNumber;
}

// 把字符串中的 01 串进行转换
string Compression::SwithcStringToBinary(string HuffmanPath, int& Sign) {
	string binaryPath = "";
	int byteBuff = 0;
	int shiftCount = 0;

	for (int i = 0; i < HuffmanPath.size(); i++) {
		byteBuff += (HuffmanPath[i] == '1' ? 1 : 0);
		byteBuff <<= 1;		// 左移
		shiftCount++;

		// 凑满8位
		if (shiftCount == 8) {
			byteBuff >>= 1;					// 将上面的移动归位，右移回来
			binaryPath += (char)byteBuff;	// 添加到最终的字符串中
			shiftCount = 0;

			// 假如到达末尾，就可以停止循环了
			if (i == HuffmanPath.size() - 1) break;

			// 对不满8位的情况进行处理
			// i + 8: 表示 i 已经移动到倒数8位去了
			if (i + 8 > HuffmanPath.size()) {
				i++;
				while (i <= HuffmanPath.size() - 1) {
					byteBuff += (HuffmanPath[i] == '1' ? 1 : 0);
					byteBuff <<= 1;
					shiftCount++;
					i++;
				}
				byteBuff <<= 7 - shiftCount;
				binaryPath += (char)byteBuff;
			}
		}
	}

	// 记录尾部多余的0
	Sign = 8 - shiftCount;

	cerr << "Switch String To Binary Completed" << endl;
	return binaryPath;
}

// 获取当前节点的位置信息
int Compression::GetSide(Node HuffmanTree[], int leafNumber) {
	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            是叶节点
	// leafNumber--->2*LeafNumber - 2 是根节点
	for (int i = 0; i < 2 * leafNumber - 2; i++) {
		// 判断当前节点是不是某个父节点的左子节点
		if (i == HuffmanTree[HuffmanTree[i].parent].left)
			HuffmanTree[i].side = 'l';

		// 判断当前节点是不是某个父节点的右子节点
		if (i == HuffmanTree[HuffmanTree[i].parent].right)
			HuffmanTree[i].side = 'r';
	}

	return 0;
}

// 写入文件
int Compression::WriteToFile(string path2, Node HuffmanTree[], int leafNumber, string binaryPath, int Sign) {
	// path2 是要写入的目标文件

	// 先减一：表示满足存满8位的个数
	leafNumber -= 1;

	ofstream outStr(path2, ios::binary);
	/*
		以下的代码表达的意思是：
		向文件中写入 (不满8位存储) + (满8位存储) 的数据
		写入的数据都是空数据，可以理解为“占位”
	*/
	outStr.write(reinterpret_cast<char*>(&Sign), sizeof(char));
	outStr.write(reinterpret_cast<char*>(&leafNumber), sizeof(char));

	// 将其加回来，继续处理
	leafNumber += 1;


	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            是叶节点
	// leafNumber--->2*LeafNumber - 2 是根节点

	// 存储叶子节点的信息
	for (int i = 0; i < leafNumber; i++) {
		// 获取要写的内容的地址，转换为char*
		HuffmanTree[i].parent -= leafNumber;
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].info),		// 该节点对应的ASCII码
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].parent),	// 该节点的父节点
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].side),		// 该节点的属性（左？/右？）
			sizeof(char));
	}

	// 存储根节点
	for (int i = leafNumber; i < 2 * leafNumber - 1; i++) {
		HuffmanTree[i].parent -= leafNumber;
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].parent),
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].side),
			sizeof(char));
	}

	// 存储编码过后的二进制码
	for (int i = 0; i < binaryPath.size(); i++) {
		outStr.put(binaryPath[i]);
	}

	outStr.close();

	cerr << "Write To File Completed" << endl;
	return 0;
}

// 测试方法：输出哈夫曼树的信息
int Compression::ShowTable(Node HuffmanTree[], int leafNumber) {
	for (int i = 0; i < 2 * leafNumber - 1; i++) {
		cout << "i:" << i << endl;
		cout << HuffmanTree[i].index << "<-index" << endl;
		cout << HuffmanTree[i].info << "<-info" << endl;
		cout << HuffmanTree[i].side << "<-side" << endl;
		cout << HuffmanTree[i].left << "<-left" << endl;
		cout << HuffmanTree[i].right << "<-right" << endl;
		cout << HuffmanTree[i].parent << "<-parent" << endl;
		cout << HuffmanTree[i].weight << "<-weight" << endl;
		cout << HuffmanTree[i].BinaryCode << "<-code" << endl;
	}
	return 0;
}