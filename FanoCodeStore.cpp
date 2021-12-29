#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "FanoCodeStore.h"




//              +-----------------------------------------------------------------------------------+
//              |                           >>>CONSTRUCTOR + DESTRUCTOR<<<                          |
//              +-----------------------------------------------------------------------------------+




FanoCodeStore::FanoCodeStore(std::ifstream& Input, char Action)
{
	if (Input.is_open())
	{
		try { CheckFileContents(Input); }
		catch (...) { throw; }
		switch (Action)
		{
		case 'c':
			ConstructFanoTable(Input);
			break;
		case 'd':
			try { ConstructFanoTree(Input); }
			catch (...) { throw; }
			break;
		default:
			throw std::exception("ERROR: invalid argument");
		}
	}
	else throw std::exception("ERROR: file can't be opened");
}

FanoCodeStore::~FanoCodeStore() {}




//              +-----------------------------------------------------------------------------------+
//              |                                >>> FANO TABLE <<<                                 |
//              +-----------------------------------------------------------------------------------+




void FanoCodeStore::ConstructFanoTable(std::ifstream& Input)
{
	std::cout << std::endl;
	std::vector<Column> TmpTable;
	NextAction = Action::Compress;
	TotalAmount = 0;
	std::cout << "Begin constructing Fano table to compress..." << std::endl;

	FillbyUniqBytesAndAmounts(Input, TmpTable);
	std::cout << "Fano table was filled by unique bytes and their amounts" << std::endl;

	std::sort(TmpTable.begin(), TmpTable.end(), DescendingSort);
	std::cout << "Table was sorted in decsending order" << std::endl;

	FanoTreeCipher = ToString(TotalAmount);
	FanoTreeCipher.push_back('\n');
	FillbyFanoCodes(TmpTable, TotalAmount, 0, TmpTable.size());
	std::cout << "All codes were successfully composed" << std::endl;

	SaveToFanoTable(TmpTable);
	std::cout << "Fano table was constructed successfully" << std::endl;
}

void FanoCodeStore::FillbyUniqBytesAndAmounts(std::ifstream& Input, std::vector<Column>& TmpTable)
{
	unsigned char UniqByte = Input.get();
	while (!Input.eof())
	{
		bool Unique = true;
		long endOfFT = TmpTable.end() - TmpTable.begin();
		for (long i = 0; i < endOfFT; i++)
		{
			if (TmpTable[i].First == UniqByte)
			{
				Unique = false;
				TmpTable[i].Second++;
				break;
			}
		}
		if (Unique == true)
		{
			CodeFano Empty;
			Column NewColumn(UniqByte, 1, Empty);
			TmpTable.push_back(NewColumn);
		}
		TotalAmount++;
		UniqByte = Input.get();
	}
}

void FanoCodeStore::FillbyFanoCodes(std::vector<Column>& TmpTable, long Amount, long LeftBoard, long RightBoard)
{
	if (LeftBoard == RightBoard - 1)
	{
		if (LeftBoard == 0 && RightBoard == TmpTable.size())
		{
			TmpTable[LeftBoard].Third.push_back(0);
		}
		FanoTreeCipher.push_back(' ');
		FanoTreeCipher.push_back(TmpTable[LeftBoard].First);
		return;
	}
	long PerfectMid = Amount / 2;
	long CurrentMid = 0;
	long CurrentLeftBoard = LeftBoard;

	while (PerfectMid - (CurrentMid + TmpTable[CurrentLeftBoard].Second) > 0)
	{
		CurrentMid += TmpTable[CurrentLeftBoard].Second;
		CurrentLeftBoard++;
	}

	if (PerfectMid - CurrentMid >= (CurrentMid + TmpTable[CurrentLeftBoard].Second) - PerfectMid)
	{
		CurrentMid += TmpTable[CurrentLeftBoard].Second;
		CurrentLeftBoard++;
	}
	long iLeftPart = LeftBoard;
	for (; iLeftPart < CurrentLeftBoard; iLeftPart++)
		TmpTable[iLeftPart].Third.push_back(0);
	long iRightPart = CurrentLeftBoard;
	for (; iRightPart < RightBoard; iRightPart++)
		TmpTable[iRightPart].Third.push_back(1);

	FanoTreeCipher.push_back('0');
	FillbyFanoCodes(TmpTable, CurrentMid, LeftBoard, CurrentLeftBoard);
	FanoTreeCipher.push_back('1');
	FillbyFanoCodes(TmpTable, Amount - CurrentMid, CurrentLeftBoard, RightBoard);
}

void FanoCodeStore::SaveToFanoTable(std::vector<Column>& TmpTable)
{
	long endOfFT = TmpTable.end() - TmpTable.begin();
	for (long i = 0; i < endOfFT; i++)
		FanoTable[TmpTable[i].First] = TmpTable[i].Third;
}




//              +-----------------------------------------------------------------------------------+
//              |                                >>> FANO TREE <<<                                  |
//              +-----------------------------------------------------------------------------------+




void FanoCodeStore::ConstructFanoTree(std::ifstream& Input)
{
	std::cout << std::endl;
	NextAction = Action::Decompress;
	std::cout << "Begin extracting Fano tree from the compressed file to decompress..." << std::endl;
	try 
	{
		GetFanoTreeCipher(Input);
		std::cout << "Fano tree cipher was read from the file" << std::endl;

		DefineTotalAmount();
		std::cout << "Amount of all bytes was defined" << std::endl;

		BuildFanoTree(FanoTree.Root, StrtIdx, FanoTreeCipher.length());
		std::cout << "Fano tree was constructed successfully" << std::endl;
	}
	catch (...) { throw; }
}

void FanoCodeStore::GetFanoTreeCipher(std::ifstream& Input)
{
	unsigned char BufSymbFirst = Input.get();
	unsigned char BufSymbMid = Input.get();
	unsigned char BufSymbLast = Input.get();
	while (!Input.eof() && !(BufSymbFirst == ' ' && BufSymbLast == '\n'))
	{
		FanoTreeCipher.push_back(BufSymbFirst);
		BufSymbFirst = BufSymbMid;
		BufSymbMid = BufSymbLast;
		BufSymbLast = Input.get();
	}
	if (Input.eof())
		throw std::exception("ERROR: program can't continue to work. Specified file wasn't compressed at all/wasn't compressed by this program/was corrupted (step 1)");
	FanoTreeCipher.push_back(BufSymbFirst);
	FanoTreeCipher.push_back(BufSymbMid);
}

void FanoCodeStore::DefineTotalAmount()
{
	int i = 0;
	bool CarrRetIsFirstSymbol = true;
	bool NotAdigitBeforeCarrRet = false;
	while (FanoTreeCipher[i] != '\0' && FanoTreeCipher[i] != '\n')
	{
		CarrRetIsFirstSymbol = false;
		if (!IsDigit(FanoTreeCipher[i]))
		{
			NotAdigitBeforeCarrRet = true;
			break;
		}
		i++;
	}
	if (CarrRetIsFirstSymbol || NotAdigitBeforeCarrRet || FanoTreeCipher[i] == '\0')
		throw std::exception("ERROR: program can't continue to work. Specified file wasn't compressed at all/wasn't compressed by this program/was corrupted (step 2)");
	std::string StringTotalAmount;
	StringTotalAmount.assign(FanoTreeCipher, 0, i);
	TotalAmount = FromString(StringTotalAmount);
	StrtIdx = i + 1;
}

void FanoCodeStore::BuildFanoTree(Node*& CurNode, long LeftBoard, long RightBoard)
{
	CurNode = new Node();
	if (FanoTreeCipher[LeftBoard] == ' ')
	{
		CurNode->UniqByte = FanoTreeCipher[++LeftBoard];
		return;
	}
	else
	{
		int Stack = 1;
		long Center = LeftBoard;
		while (Stack != 0 && Center != RightBoard)
		{
			Center++;
			if (FanoTreeCipher[Center] == '0' && FanoTreeCipher[Center - 1] != ' ') Stack++;
			if (FanoTreeCipher[Center] == '1' && FanoTreeCipher[Center - 2] == ' ') Stack--;
		}
		if (Center == RightBoard)
			throw std::exception("ERROR: program can't continue to work. Specified file wasn't compressed at all/wasn't compressed by this program/was corrupted (step 3)");
		BuildFanoTree(CurNode->Left, ++LeftBoard, Center);
		BuildFanoTree(CurNode->Right, ++Center, RightBoard);
	}
}




//              +-----------------------------------------------------------------------------------+
//              |                               COMPRESS / DECOMPRESS                               |
//              +-----------------------------------------------------------------------------------+




void FanoCodeStore::CompressData(std::ifstream& Input, std::ofstream& Output)
{
	if (NextAction == Action::Compress)
	{
		Input.clear();
		Input.seekg(0);
		Output << FanoTreeCipher << std::endl;
		int Count = 0;
		unsigned char Buffer = 0;
		for (long i = 0; i < TotalAmount; i++)
		{
			unsigned char CurrentByte = Input.get();
			CodeFano CurrentByteCode = FanoTable[CurrentByte];
			for (long i = 0; i < CurrentByteCode.size(); i++)
			{
				Buffer = Buffer | CurrentByteCode[i] << ((NumberOfBits - 1) - Count);
				Count++;
				if (Count == NumberOfBits)
				{
					Output << Buffer;
					Count = 0;
					Buffer = 0;
				}
			}
		}
		if (Count != 0) Output << Buffer;
		return;
	}
	std::cout << "ERROR: Calling wrong method" << std::endl;
	exit(PROGRAMMER_ERROR);
}

void FanoCodeStore::DEcompressData(std::ifstream& Input, std::ofstream& Output)
{
	if (NextAction == Action::Decompress)
	{
		if (FanoTree.Root->Left == NULL && FanoTree.Root->Right == NULL)
		{
			for (long i = 0; i < TotalAmount; i++) Output << FanoTree.Root->UniqByte;
			return;
		}
		int Count = 0;
		long TotAm = TotalAmount;
		Node* Bypass = FanoTree.Root;
		unsigned char CurrentByte = Input.get();
		while (CurrentByte != -1)
		{
			if (TotAm == 0) break;
			bool bit = CurrentByte & (1 << ((NumberOfBits - 1) - Count));
			if (bit == 0) Bypass = Bypass->Left;
			else Bypass = Bypass->Right;
			if ((Bypass->Left != NULL && Bypass->Right == NULL) || (Bypass->Left == NULL && Bypass->Right != NULL))
				throw std::exception("ERROR: program can't continue to work. Specified file wasn't compressed at all/wasn't compressed by this program/was corrupted (step 4 dcmpr)");
			if (Bypass->Left == NULL && Bypass->Right == NULL)
			{
				Output << Bypass->UniqByte;
				Bypass = FanoTree.Root;
				TotAm--;
			}
			Count++;
			if (Count == NumberOfBits)
			{
				Count = 0;
				CurrentByte = Input.get();
			}
		}
		return;
	}
	std::cout << "ERROR: Calling wrong method" << std::endl;
	exit(PROGRAMMER_ERROR);
}




//              +-----------------------------------------------------------------------------------+
//              |                                      OTHER                                        |
//              +-----------------------------------------------------------------------------------+




bool FanoCodeStore::IsDigit(unsigned char Symbol)
{
	if (Symbol >= '0' && Symbol <= '9') return true;
	return false;
}

std::string FanoCodeStore::ToString(long Value)
{
	std::ostringstream oss;
	oss << Value;
	return oss.str();
}

long FanoCodeStore::FromString(const std::string& String)
{
	std::istringstream iss(String);
	long Value;
	iss >> Value;
	return Value;
}

void FanoCodeStore::CheckFileContents(std::ifstream& Input)
{
	short FirstByte = Input.get();
	if (Input.eof() != 0) throw std::exception("ERROR: file is empty");
	short SecondByte = Input.get();
	if (Input.eof() != 0) throw std::exception("ERROR: compressed file can't consider only 1 byte. Single-byte file doesn't make sense to compress");
	Input.clear();
	Input.seekg(0);
}