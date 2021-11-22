// Huffman_Quan.cpp: 定义应用程序的入口点。
//

#include "Huffman_Quan.h"

// 请写上cpp文件所在文件夹的路径
const string projectPath = "E:\\Program Practics\\Huffman_Quan\\Huffman_Quan\\";

// 压缩文件
void CompressTheFile(string& op, string& path1, string& path2, Compression& compress) {
	cout << "压缩文件中............" << endl;

	int tong[257] = { 0 };

	// 获取叶子节点数量
	int leafNumber = compress.Count(op, path1, path2, tong);
	// 初始化树
	Tree HuffmanTree = new Node[2 * leafNumber - 1];
	// 创建哈夫曼树
	compress.CreateHuffmanTree(tong, leafNumber, HuffmanTree);
	compress.GetSide(HuffmanTree, leafNumber);
	compress.GetCodeNode(HuffmanTree, leafNumber);

	// 测试代码
	//compress.ShowTable(HuffmanTree, leafNumber);

	// 对文件进行编码
	string HuffmanPath = compress.Encode(path1, HuffmanTree, leafNumber);

	int Sign;
	// 获取二进制编码串
	string binaryPath = compress.SwithcStringToBinary(HuffmanPath, Sign);

	// 写入文件
	compress.WriteToFile(path2, HuffmanTree, leafNumber, binaryPath, Sign);

	cout << "压缩完毕！" << endl;
}

// 解压文件
void DepressTheFile(string& op, string& path1, string& path2, Depression& depress) {
	cout << "开始解压..........." << endl;

	unsigned int leafNumber;
	int Sign;
	string searchPath;

	// 读取压缩文件
	Tree HuffmanTree = depress.ReadFile(path1, leafNumber, searchPath, Sign);
	// 构建哈夫曼树
	depress.BuiltHuffmanTree(HuffmanTree, leafNumber);
	// 解压文件
	depress.RestoreFile(searchPath, HuffmanTree, path2, leafNumber);

	cout << "解压完毕！" << endl;
}

// 交互界面
void View(string& op, string& path1, string& path2) {
	bool flag = true;
	string fileName = "";
	string depressedFileName = "";

	while (flag) {
		cout << "请选择压缩或解压功能 （z：压缩，x：解压）" << endl;
		cin >> op;
		
		if (op == "z") {
			cout << "请输入待压缩的文件名：";		cin >> fileName;
			cout << "请输入压缩后的文件名：";		cin >> depressedFileName;
			path1 = projectPath + fileName;
			path2 = projectPath + depressedFileName;
			flag = false;
		}
		else if (op == "x") {
			cout << "请输入被压缩后的文件名：";	cin >> depressedFileName;
			cout << "请输入解压后的文件名：";		cin >> fileName;
			path1 = projectPath + depressedFileName;
			path2 = projectPath + fileName;
			flag = false;
		}
		else {
			cout << "输入有误，请认真检查过后在确认输入！" << endl;
			cout << "------------- 请重新输入 -------------" << endl;
		}
	}
}

// 主函数控制
int main(){

	string op = "";
	string path1 = "";
	string path2 = "";

	// 选择
	View(op, path1, path2);

	
	// 分情况处理
	if (op == "z") {			// 文件压缩
		Compression compress;
		CompressTheFile(op, path1, path2, compress);
	}
	else if (op == "x") {		// 文件解压
		Depression depress;
		DepressTheFile(op, path1, path2, depress);
	}

	return 0;
}