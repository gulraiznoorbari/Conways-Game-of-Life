#pragma once

struct Position {
	int x;
	int y;
	int liveNeighboursCount;

	Position()
	{
		x = 0;
		y = 0;
		liveNeighboursCount = 0;
	}

	Position(int x, int y)
	{
		this->x = x;
		this->y = y;
		liveNeighboursCount = 0;
	}

	// const correctness
	bool operator==(const Position& other) const
	{
		return ((x == other.x) && (y == other.y));
	}
};

template <class T>
class UnInitDynamicArray
{
private:
	T *_array;
	int _arraySize;
	int _maxSize;

public:
	UnInitDynamicArray();
	UnInitDynamicArray(int);
	UnInitDynamicArray(const UnInitDynamicArray<T>&);

	void Insert(const T&);
	void Delete(const T&);
	int GetSize();
	void Resize(int);

	T& operator[](int);

	~UnInitDynamicArray();
};

template <class T>
UnInitDynamicArray<T>::UnInitDynamicArray()
{
	_array = nullptr;
	_arraySize = 0;
	_maxSize = 0;
}

template <class T>
UnInitDynamicArray<T>::UnInitDynamicArray(int size)
{
	_arraySize = 0;
	_maxSize = 2 * size;
	// allocate space for double the required size as cellular growth is important.
	_array = new T[_maxSize];
}

template <class T>
UnInitDynamicArray<T>::UnInitDynamicArray(const UnInitDynamicArray<T>& otherArray)
{
	_arraySize = otherArray._arraySize;
	_maxSize = otherArray._maxSize;

	_array = new T[_maxSize];
	for (int i = 0; i < _arraySize; i++)
	{
		_array[i] = otherArray._array[i];
	}
}

template <class T>
void UnInitDynamicArray<T>::Insert(const T& value)
{
	if (_arraySize == _maxSize)
	{
		_maxSize *= 2;
		T *temp = new T[_maxSize];
		for (int i = 0; i < _arraySize; i++)
		{
			temp[i] = _array[i];
		}
		delete[] _array;
		_array = temp;
	}
	_array[_arraySize] = value;
	_arraySize++;
}

template <class T>
void UnInitDynamicArray<T>::Delete(const T& value)
{
	// basically a temp array will be created without the element we want to delete.
	T* temp = new T[_maxSize];
	for (int i = 0, j = 0; i < _arraySize; i++, j++)
	{
		if (_array[i] == value)
		{
			j--;
		}
		else 
		{
			temp[j] = _array[i];
		}
	}
	delete[] _array;
	_arraySize--;
	_array = temp;
}

template <class T>
int UnInitDynamicArray<T>::GetSize()
{
	return _arraySize;
}

template <class T>
void UnInitDynamicArray<T>::Resize(int size)
{
	_maxSize = size * 2;
	_arraySize = 0;
	_array = new T[_maxSize];
}

template <class T>
T& UnInitDynamicArray<T>::operator[](int index)
{
	return _array[index];
}

template <class T>
UnInitDynamicArray<T>::~UnInitDynamicArray()
{
	delete[] _array;
}