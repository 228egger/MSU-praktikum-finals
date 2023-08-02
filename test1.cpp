#include <iostream>
#include <stdexcept>
#include <exception>
#include <utility>

template <typename T>
class MultiSet{
	public:
		MultiSet();
		MultiSet(const MultiSet &, int new_capacity = -1);
		~MultiSet();
		MultiSet & operator = (MultiSet);
		T & operator [] (int index);
		const T & operator [] (int index) const;
		void add(const T &t);
		void del(int index, T & arg);
		void clear(); //обнуление
		int size() const; //возврат размера
	private:
		T *arr;
		int cur_size;
    	int capacity;
};

template <typename T>
MultiSet<T>::MultiSet(): arr(0), cur_size(0), capacity(1){
	arr = new T[capacity];
}

template <typename T>
MultiSet<T>::~MultiSet(){
	if (capacity > 0){
		delete [] arr;
	}
}

template <typename T>
MultiSet<T>::MultiSet(const MultiSet & arg, int new_capacity){
	cur_size = arg.cur_size;
	if ((new_capacity >= arg.cur_size) && (new_capacity > 0)){
		capacity = new_capacity;
	}
	arr = new T[capacity];
	try{
		for (int i = 0; i < cur_size; ++i) {
            arr[i] = arg.arr[i];
        }
	}
	catch(...){
		delete [] arr;
		throw std::exception();
	}
}

template <typename T>
MultiSet<T> &
MultiSet<T>::operator = (MultiSet arg){
	std::swap(arr, arg.arr);
	std::swap(cur_size, arg.cur_size);
	std::swap(capacity, arg.capacity);
	return *this;
}

template <typename T>
T&
MultiSet<T>::operator [](int index){
	if ((index >= 0) && (index < cur_size)) {
        return arr[index];
    }
    else {
        throw std::exception();
    }
}

template <typename T>
const T&
MultiSet<T>::operator [] (int index) const{
	if ((index >= 0) && (index < cur_size)) {
        return arr[index];
    }
    else {
        throw std::exception();
    }
}

template <typename T>
int
MultiSet<T>::size() const{
	return cur_size;
}

template <typename T>
void
MultiSet<T>::clear(){
	cur_size = 0;
}

template <typename T>
void
MultiSet<T>::del(int index, T & arg);{
	T copy = (*this)[index];
	try{
		arr[index] = (*this)[cur_size - 1];
	}
	catch(...){
		throw std::exception();
	}
	arg = copy;
	cur_size -= 1;
}

template <typename T>
void
MultiSet<T>::add(const T &t){
	if (cur_size >= capacity){
		MultiSet copy(*this, capacity * 2);
		copy.arr[copy.cur_size] = t;
		copy.cur_size += 1;
		std::swap(arr, copy.arr);
		std::swap(cur_size, copy.cur_size);
		std::swap(capacity, copy.capacity);
	}
	else{
		arr[cur_size] = t;
		cur_size += 1;
	}
}





int
main() {
    try {
        MultiSet<int> * x;
        MultiSet<int> * y;
        x = new MultiSet<int>();
        y = new MultiSet<int>();
        x->add(1);
        x->add(2);
        x->add(3);
        y->add(4);
        y->add(5);
        y->add(6);
        std::cout << (*x)[0] << (*x)[1] << (*x)[2] << std::endl;
        std::cout << (*y)[0] << (*y)[1] << (*y)[2] << std::endl;
        (*y) = (*x);
        delete x;
        std::cout << (*y)[0] << (*y)[1] << (*y)[2] << std::endl;
        delete y;
    }
    catch(...) {
        std::cout << std::endl << "Exception!" << std::endl;
    }
}


