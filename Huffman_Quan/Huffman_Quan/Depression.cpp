/*
	Date: 2021-11-22
	Email: Hilda_quan@163.com
*/

#include "Depression.h"

// ��ȡ�ļ�
Tree Depression::ReadFile(string path1, unsigned int& leafNumber, string& searchPath, int& Sign) {
	// Sign�������Ǽ�¼ĩβ�����ж��ٸ������0���������ɾȥ

	ifstream inStr(path1, ios::in | ios::binary);
	searchPath = "";

	// ��ȡ����8λ�ĸ���
	char ch;
	inStr.get(ch);
	Sign = ch;

	// ��ȡ�ж��ٸ�Ҷ�ӽڵ�
	inStr.get(ch);
	leafNumber = (int)(unsigned char)ch;
	leafNumber += 1;

	// Tree HuffmanTree = new Node[2 * LeafNumber - 1];
	// 0--->LeafNumber - 1            ��Ҷ�ڵ�
	// leafNumber--->2*LeafNumber - 2 �Ǹ��ڵ�
	Tree HuffmanTree = new Node[2 * leafNumber - 1];
	unsigned int num;

	// ��ȡҶ�ӽڵ���Ϣ���Խڵ���leafNumber���л��֣�
	for (int i = 0; i < leafNumber; i++) {
		// ��ȡASCII��
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].info = num;

		// ��ȡ���ڵ�����
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].parent = num + leafNumber;  // ��Ϊ��д����Ϣ�У�������leafNumber�����Դ˴�Ҫ����

		// ��ȡ�ڵ�λ����Ϣ
		inStr.get(ch);
		HuffmanTree[i].side = ch;

		// ��ʼ��������Ϣ
		HuffmanTree[i].BinaryCode = "";
		HuffmanTree[i].index = i;
		HuffmanTree[i].left = -1;
		HuffmanTree[i].right = -1;
		HuffmanTree[i].weight = -1;
	}

	// ��ȡ���ڵ����Ϣ
	for (int i = leafNumber; i < 2 * leafNumber - 1; i++) {
		// ��ȡ���ڵ���Ϣ
		inStr.get(ch);
		num = (int)(unsigned char)ch;
		HuffmanTree[i].parent = num + leafNumber;

		// ��ȡ�ڵ�λ����Ϣ
		inStr.get(ch);
		HuffmanTree[i].side = ch;

		// ��ʼ������Ϣ
		HuffmanTree[i].info = -10000;
		HuffmanTree[i].BinaryCode = "";
		HuffmanTree[i].index = i;
		HuffmanTree[i].left = -1;
		HuffmanTree[i].right = -1;
		HuffmanTree[i].weight = -1;
	}

	// ��ȡ��������Ϣ
	int bitMask = 0x80;  // 0x80����ʮ���Ƶ�128 �����ƣ�1000 0000 
	while (inStr.get(ch)) {
		num = (int)(unsigned char)ch;
		while (bitMask != 0) {
			// ���ж�����λ����
			if ((bitMask & num) != 0) {
				searchPath += '1';
			}
			if ((bitMask & num) == 0) {
				searchPath += '0';
			}
			bitMask >>= 1;
			/*
			* ����һ����bitMask������һ����num��Ҳ����ת�������Ķ������룬���һ����serachPath����Ϊ�洢num���ַ���
			* ʹ��λ���㽫num�����ݡ�����������bitMask������������λ����ģ�
				1000 0000	����	   0100 0000   ����	  0010 0000
				0101 0010  ----->  0101 0010  ----->  0101 0010
				---------		   ---------		  ---------
				0				   01				  010
			*/
		}
		// ������Ϊ 1000 0000��Ϊ��һ��������׼��
		bitMask = 0x80;
	}

	// ���ĩβ�����0
	searchPath.erase(searchPath.end() - Sign, searchPath.end());

	inStr.close();

	cerr << "Read File Completed" << endl;
	return HuffmanTree;
}

// ��ԭ��������
int Depression::BuiltHuffmanTree(Node HuffmanTree[], int leafNumber) {
	for (int i = 0; i < 2 * leafNumber - 1; i++) {
		// �ҵ��ýڵ�ĸ��ڵ㣬�������ڵ��left��Ϊ�Լ�
		if (HuffmanTree[i].side == 'l')
			HuffmanTree[HuffmanTree[i].parent].left = i;
		// �ҵ��ýڵ�ĸ��ڵ㣬�������ڵ��right��Ϊ�Լ�
		if (HuffmanTree[i].side == 'r')
			HuffmanTree[HuffmanTree[i].parent].right = i;
	}

	cerr << "Built Huffman Tree Completed" << endl;
	return 0;
}

// ��ԭ�ļ�
int Depression::RestoreFile(string searchPath, Node HuffmanTree[], string path2, int leafNumber) {
	ofstream outStr(path2, ios::out | ios::binary);

	// ���������ĸ��ڵ㣬���ǹ�������������һ������
	int head = 2 * leafNumber - 2, now = head;

	// ����searchPath�еĶ������룬���������������ҵ�Ҷ�ӽڵ�
	for (int i = 0; i < searchPath.size(); i++) {
		// ���ݶ���������б���
		if (searchPath[i] == '0')
			now = HuffmanTree[now].left;  // ������൱����ǰ�� cur = cur->left
		if (searchPath[i] == '1')
			now = HuffmanTree[now].right; // �൱�� cur = cur->right
		// �ҵ�Ҷ�ӽڵ�
		if (HuffmanTree[now].left == -1 && HuffmanTree[now].right == -1) {
			char res = HuffmanTree[now].info;
			outStr.put(res);
			now = head;		// ���»ص����ڵ�
		}
	}

	outStr.close();

	cerr << "Restore File Completed" << endl;
	return 0;
}

// ���Է��������������������Ϣ
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