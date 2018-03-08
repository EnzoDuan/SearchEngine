#define EXPAND_LEN 1000
#include <assert.h>

template<typename E>
class vector{
public:
	vector();
	~vector();
	int size();
	E& operator[](int nSubscript);  
	void push_back(E val);
private:
	E * elems_ptr;
	int elem_max;
	int elem_capacity;
	void expand_elems();
};

template<typename E>
void vector<E>::expand_elems()
{
	E* new_ptr = new E[elem_capacity + EXPAND_LEN];
	for(int i = 0; i < elem_capacity; i++){
		new_ptr[i] = elems_ptr[i];
	}
	delete [] elems_ptr;
	elems_ptr = new_ptr;
	elem_capacity += EXPAND_LEN;
}

template<typename E>
void vector<E>::push_back(E val)
{
	if(elem_max == elem_capacity)
		expand_elems();
	elems_ptr[elem_max++] = val;
}

template<typename E>
vector<E>::vector()
{
	elem_capacity = EXPAND_LEN;
	elems_ptr = new E[elem_capacity];
	elem_max = 0;
}

template<typename E>
vector<E>::~vector()
{
	delete [] elems_ptr;
}

template<typename E>
int vector<E>::size()
{
	return elem_max;
}

template<typename E>
E& vector<E>::operator[](int nSubscript)
{
	if(nSubscript >= 0 && nSubscript < elem_max)
		return elems_ptr[nSubscript];
	else
		assert(0);
}

