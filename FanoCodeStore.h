#pragma once

#include <map>
#include <list>
#include <vector>
#include <string>
#include "Triad.h"
#include "BinaryTree.h"

using CodeFano = std::vector<bool>;
using Column = Triad<unsigned char, long, CodeFano>;
using Node = BinaryTree::Node;
const int NumberOfBits = 8;
const int ERROR_OF_THE_ENVIRONMENT_PROGRAM = 1;
const int PROGRAMMER_ERROR = -1;
const int ALL_FINE = 0;

class FanoCodeStore
{
private:

	std::map<unsigned char, CodeFano> FanoTable;
	BinaryTree FanoTree;
	std::string FanoTreeCipher;
	enum class Action { Compress, Decompress } NextAction;
	long TotalAmount;
	short StrtIdx;

//~~~~~~~~~~~~~~~~~~~~FANO~TABLE~~~~~~~~~~~~~~~~~~~~

	void ConstructFanoTable(std::ifstream& Input);
	void FillbyUniqBytesAndAmounts(std::ifstream& Input, std::vector<Column>& TmpTable);
	void FillbyFanoCodes(std::vector<Column>& TmpTable, long Amount, long LeftBoard, long RightBoard);
	void SaveToFanoTable(std::vector<Column>& TmpTable);

//~~~~~~~~~~~~~~~~~~~~FANO~TREE~~~~~~~~~~~~~~~~~~~~~

	void ConstructFanoTree(std::ifstream& Input);
	void GetFanoTreeCipher(std::ifstream& Input);
	void DefineTotalAmount();
	void BuildFanoTree(Node*& CurNode, long LeftBoard, long RightBoard);

//~~~~~~~~~~~~~~~~~~~~~~OTHER~~~~~~~~~~~~~~~~~~~~~~~~

	bool IsDigit(unsigned char Symbol);
	std::string ToString(long Value);
	long FromString(const std::string& String);
	void CheckFileContents(std::ifstream& Input);
	class Sort
	{
	public:
		bool operator() (Column& Amount1, Column& Amount2) { return (Amount1.Second > Amount2.Second); }
	} DescendingSort;

public:

	FanoCodeStore(std::ifstream& Input, char Action);
	void CompressData(std::ifstream& Input, std::ofstream& Output);
	void DEcompressData(std::ifstream& Input, std::ofstream& Output);
	~FanoCodeStore();
};