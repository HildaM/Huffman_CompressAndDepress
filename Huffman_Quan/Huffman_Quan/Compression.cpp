/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#include "Compression.h"

using namespace std;

// ������������
int Compression::CreateHuffmanTree(int tong[], int leafNumber, Node HuffmanTree[]) {
	int k = 0;
	// ʹ�����ȶ��д洢�Խڵ���ж�̬���򡣰���������Node�еĹ����������
	// greater: ����Ĭ��Ϊ����
	priority_queue<Node, vector<Node>, greater<Node>> pq;

	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            ��Ҷ�ڵ�
	// leafNumber--->2*LeafNumber - 2 �Ǹ��ڵ�

	// �Թ����������г�ʼ��
	for (int i = 0; i <= 256; i++) {
		// ֻ�г��ֹ����ַ��Ž��м�¼
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
	// ͨ�����ȶ��д�����������
	while (pq.size() > 1) {
		// ��ʱ�����ı�Ȼ��weight��С�������ڵ�
		Node t1 = pq.top();	pq.pop();
		Node t2 = pq.top();	pq.pop();

		HuffmanTree[t1.index].parent = j;
		HuffmanTree[t2.index].parent = j;

		HuffmanTree[j].index = j;
		HuffmanTree[j].parent = -1;
		HuffmanTree[j].left = t1.index;		// left�ڹ�����������С��right�ģ�����leftҪΪt1.index
		HuffmanTree[j].right = t2.index;
		HuffmanTree[j].weight = t1.weight + t2.weight;
		HuffmanTree[j].info = -127;

		pq.push(HuffmanTree[j]);
		j++;
	}

	// �����һ���ڵ���е�������
	j--;
	Node HuffmanTreeHead = pq.top();
	pq.pop();
	HuffmanTreeHead.parent = -127;
	HuffmanTree[j] = HuffmanTreeHead;
	HuffmanTree[j].info = -127;

	cerr << "Create Huffman Tree Completed" << endl;
	return 0;
}

// �Գ��ֵ��ַ����б���
int Compression::GetCodeNode(Node HuffmanTree[], int leafNumber) {
	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            ��Ҷ�ڵ�
	// leafNumber--->2*LeafNumber - 2 �Ǹ��ڵ�
	for (int i = 0; i < 2 * leafNumber - 2; i++) {
		// ����Ҷ�ӽڵ�ļ�������
		if (HuffmanTree[i].info == -127) continue;

		int indexForSearching = i;
		HuffmanTree[i].BinaryCode = "";

		// ��Ҷ�ӽڵ���������ϱ�����ֱ�����ڵ�
		int j = 0;
		while (HuffmanTree[indexForSearching].parent != -127) {
			j = HuffmanTree[indexForSearching].parent;
			// ����·���жϣ�ʹ�Լ���parent�Ĳż�¼
			if (HuffmanTree[j].left == indexForSearching)
				HuffmanTree[i].BinaryCode += '0';
			if (HuffmanTree[j].right == indexForSearching)
				HuffmanTree[i].BinaryCode += '1';
			indexForSearching = j;
		}

		// ��������ĺ�BinaryCode�ǵ���ģ�������Ҫת��Ϊ����
		reverse(HuffmanTree[i].BinaryCode.begin(),
			HuffmanTree[i].BinaryCode.end());
	}

	cerr << "Get Node BinarryCode Completed" << endl;
	return 0;
}

// ���ļ����б���
string Compression::Encode(string path1, Node HuffmanTree[], int leafNumber) {
	// ��Ŀ���ļ����ļ���
	ifstream inStr(path1, ios::in | ios::binary);

	char ch;
	unsigned int byteBuff = 0;
	int bitMask = 0x80;
	string HuffmanPath = "";

	while (inStr.get(ch)) {
		byteBuff = (int)(unsigned char)ch;	// ת��ΪACSII�룬����ȶ�
		int value = byteBuff;
		for (int i = 0; i < leafNumber; i++) {
			if (HuffmanTree[i].info == value) {
				HuffmanPath += HuffmanTree[i].BinaryCode;
				break;
			}
		}
	}

	cerr << "Encode Completed" << endl;
	// ���������ļ��Ķ����Ʊ���
	return HuffmanPath;
}

// ͳ���ı����ַ����ֵĴ���
int Compression::Count(string op, string path1, string path2, int tong[]) {
	// path1: Դ�ļ�  path2��Ŀ���ļ�

	// �ļ����������� in ��ʽ�������ƴ�
	ifstream inStr(path1, ios::in | ios::binary);

	unsigned int byteBuff = 0;
	char ch;
	while (inStr.get(ch)) {
		byteBuff = (int)(unsigned char)ch;	// ����ȡ�������ַ�����תΪchar����תΪint����
		tong[byteBuff]++;
	}

	int leafNumber = 0;
	for (int i = 0; i <= 256; i++) {
		if (tong[i] != 0) leafNumber++;
	}

	inStr.close();

	/*
		cerr���ص㣺
		cerr�����������ֱ�������һ������Ѹ�����������Ϣ���Ǳ�׼����Ĭ������±���������׼�������
		�����������壬Ҳ��˵������Ϣ����ֱ�ӷ��͵���ʾ��
	*/
	cerr << "Count Completed" << endl;
	return leafNumber;
}

// ���ַ����е� 01 ������ת��
string Compression::SwithcStringToBinary(string HuffmanPath, int& Sign) {
	string binaryPath = "";
	int byteBuff = 0;
	int shiftCount = 0;

	for (int i = 0; i < HuffmanPath.size(); i++) {
		byteBuff += (HuffmanPath[i] == '1' ? 1 : 0);
		byteBuff <<= 1;		// ����
		shiftCount++;

		// ����8λ
		if (shiftCount == 8) {
			byteBuff >>= 1;					// ��������ƶ���λ�����ƻ���
			binaryPath += (char)byteBuff;	// ��ӵ����յ��ַ�����
			shiftCount = 0;

			// ���絽��ĩβ���Ϳ���ֹͣѭ����
			if (i == HuffmanPath.size() - 1) break;

			// �Բ���8λ��������д���
			// i + 8: ��ʾ i �Ѿ��ƶ�������8λȥ��
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

	// ��¼β�������0
	Sign = 8 - shiftCount;

	cerr << "Switch String To Binary Completed" << endl;
	return binaryPath;
}

// ��ȡ��ǰ�ڵ��λ����Ϣ
int Compression::GetSide(Node HuffmanTree[], int leafNumber) {
	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            ��Ҷ�ڵ�
	// leafNumber--->2*LeafNumber - 2 �Ǹ��ڵ�
	for (int i = 0; i < 2 * leafNumber - 2; i++) {
		// �жϵ�ǰ�ڵ��ǲ���ĳ�����ڵ�����ӽڵ�
		if (i == HuffmanTree[HuffmanTree[i].parent].left)
			HuffmanTree[i].side = 'l';

		// �жϵ�ǰ�ڵ��ǲ���ĳ�����ڵ�����ӽڵ�
		if (i == HuffmanTree[HuffmanTree[i].parent].right)
			HuffmanTree[i].side = 'r';
	}

	return 0;
}

// д���ļ�
int Compression::WriteToFile(string path2, Node HuffmanTree[], int leafNumber, string binaryPath, int Sign) {
	// path2 ��Ҫд���Ŀ���ļ�

	// �ȼ�һ����ʾ�������8λ�ĸ���
	leafNumber -= 1;

	ofstream outStr(path2, ios::binary);
	/*
		���µĴ��������˼�ǣ�
		���ļ���д�� (����8λ�洢) + (��8λ�洢) ������
		д������ݶ��ǿ����ݣ��������Ϊ��ռλ��
	*/
	outStr.write(reinterpret_cast<char*>(&Sign), sizeof(char));
	outStr.write(reinterpret_cast<char*>(&leafNumber), sizeof(char));

	// ����ӻ�������������
	leafNumber += 1;


	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            ��Ҷ�ڵ�
	// leafNumber--->2*LeafNumber - 2 �Ǹ��ڵ�

	// �洢Ҷ�ӽڵ����Ϣ
	for (int i = 0; i < leafNumber; i++) {
		// ��ȡҪд�����ݵĵ�ַ��ת��Ϊchar*
		HuffmanTree[i].parent -= leafNumber;
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].info),		// �ýڵ��Ӧ��ASCII��
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].parent),	// �ýڵ�ĸ��ڵ�
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].side),		// �ýڵ�����ԣ���/�ң���
			sizeof(char));
	}

	// �洢���ڵ�
	for (int i = leafNumber; i < 2 * leafNumber - 1; i++) {
		HuffmanTree[i].parent -= leafNumber;
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].parent),
			sizeof(char));
		outStr.write(reinterpret_cast<char*>(&HuffmanTree[i].side),
			sizeof(char));
	}

	// �洢�������Ķ�������
	for (int i = 0; i < binaryPath.size(); i++) {
		outStr.put(binaryPath[i]);
	}

	outStr.close();

	cerr << "Write To File Completed" << endl;
	return 0;
}

// ���Է��������������������Ϣ
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