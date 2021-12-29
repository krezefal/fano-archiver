#pragma once

template <typename F, typename S, typename T>
class Triad
{
public:
	F First;
	S Second;
	T Third;
	Triad() {}
	Triad(const F& First, const S& Second, const T& Third)
	{
		this->First = First;
		this->Second = Second;
		this->Third = Third;
	}
	Triad(const Triad& TheSource)
	{
		this->First = TheSource.First;
		this->Second = TheSource.Second;
		this->Third = TheSource.Third;
	}
	Triad& operator=(const Triad& TheSource)
	{
		if (&TheSource == this) return *this;
		this->First = TheSource.First;
		this->Second = TheSource.Second;
		this->Third = TheSource.Third;
		return *this;
	}
	bool operator==(const Triad& TheSource) const
	{
		if ((this->First == TheSource.First) 
		&& (this->Second == TheSource.Second) 
		&& (this->Third == TheSource.Third)) 
		return true;
		else return false;
	}
	~Triad()
	{
		First.~F();
		Second.~S();
		Third.~T();
	}
};
