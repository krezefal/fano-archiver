#pragma once

class BinaryTree {
public:
	class Node {
	public:
		unsigned char UniqByte;
		Node* Left;
		Node* Right;
		Node() {}
		Node(unsigned char Byte) {
			UniqByte = Byte;
			Left = NULL;
			Right = NULL;
		}
		~Node() {}
	};
	Node* Root;
	BinaryTree() { Root = NULL; }
	void Erase(Node*& CurNode) {
		if (CurNode != NULL) {
			Erase(CurNode->Left);
			Erase(CurNode->Right);
			delete CurNode;
		}
	}
	~BinaryTree() { Erase(Root); }
};