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
MultiSet<T>::del(int index, T & arg){
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


class Component{
	public:
		Component();
		Component(const Component &) = delete;
		Component & operator = (const Component &) = delete;
		virtual ~Component();
		int count() const;
		Component & subcomponent(int);
		const Component & subcomponent(int) const;
		void add(Component &);
		friend std::ostream & operator << (std::ostream &, const Component &);
	protected:
		MultiSet<Component *> multiset;
	private:
		virtual const std::string & name() const = 0;
};

Component::Component(): multiset(){}

Component::~Component(){}

int
Component::count() const{
	return multiset.size();
}

const Component &
Component::subcomponent(int index) const
{
	return *(multiset[index]);
}

void
Component::add(Component &new_component)
{
	multiset.add(&new_component);
}

std::ostream &
operator << (std::ostream &stream, const Component &comp)
{
	stream << comp.name() << '(';
	for (int i = 0; i < comp.multiset.size(); ++i) {
		stream << *(comp.multiset[i]);
		if (i < comp.multiset.size() - 1) {
			stream << ", ";
		}
	}
	stream << ')';
	return stream;
}

class Box:public Component{
	public:
		Box(const std::string &);
		~Box();
		Box(const Box &);
		Box & operator = (const Box &);
	private:
		const std::string & name() const;
		std::string str;
		const std::string q = "qwerty";
};

Box::Box(const std::string & arg): Component(), str(arg) {}

Box::Box(const Box & arg): Component(), str(arg.str) {}

Box::~Box() {}

const std::string &
Box::name() const
{
    return q;
}

Box &
Box::operator =(const Box & arg){
	if (this != &arg){
		multiset.clear();
		str = arg.str;
	}
	return *this;
}



int 
main()
{
    // test 1
    Box box1("box1");
    Box box2("box2");
    Box box3("box3");
    Box box4("box4");

    box1.add(box2);
    box1.add(box3);
    box1.add(box4);

    std::cout << box1 << std::endl;
    return 0;
}



