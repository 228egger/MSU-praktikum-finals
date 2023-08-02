#include <iostream>
#include <stdexcept>
#include <exception>
#include <map>
#include <stack>
#include <set>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
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

struct Pair {
    std::string name;
    Component * component;
};

Component * 
my_find(const MultiSet<Pair> & components, std::string & arg){
	for (size_t i = 0; i < components.size(); i ++){
		if (components[i].name == arg){
			return components[i].component;
		}
	}
	return nullptr;
}

int
my_find_(const MultiSet<std::string> & previous_lexemas, const std::string & arg) {
    for (int i = 0; i < previous_lexemas.size(); ++i) {
        if (previous_lexemas[i] == arg) {
            return 1;
        }
    }
    return 0;
}

enum LexType{
    IDENT,
    COMMA,
    LEFT_BR,
    RIGTH_BR, 
    END
};

class Parser{
	public:
		Parser() = default;
		void parse(std::string const & stream, const MultiSet<Pair> & components);
		MultiSet<Component *> & get();
	private:
		void S();
		const MultiSet<Pair> * components;
		void K();
		void gl();
		MultiSet<Component *> poliZ;
		LexType cur_lex{};
		MultiSet<std::string> previous_lexemas;
		int pos;
		std::string s;
		const std::string * stream;
};


void
Parser::gl(){
	cur_lex = END;
	s.clear();
	if (pos == int(stream->size())) {
		cur_lex = END;
        return;
    }
    int test = (*stream)[pos];
    if ((!isspace(test)) && (!isalpha(test)) && (!isdigit(test)) && (test != '(') && (test != ')') && (test != ',')) {
        throw std::exception();
    }
    while (isspace((*stream)[pos])) {
        pos++;
        if (pos == int(stream->size())) {
            return;
        }
    }
    if ((*stream)[pos] == '(') {
    	s = '(';
        cur_lex = LEFT_BR;
        pos++;
        return;
    }
    if ((*stream)[pos] == ')') {
    	s = ')';
        cur_lex = RIGTH_BR;
        pos++;
        return;
    }
    if ((*stream)[pos] == ',') {
    	s = ',';
        cur_lex = COMMA;
        pos++;
        return;
    }
    if (isdigit((*stream)[pos])){
    	throw std::exception();
    }
    while ((isalpha((*stream)[pos])) || (isdigit((*stream)[pos]))) {
        cur_lex = IDENT;
        s += (*stream)[pos];
        pos++;
        if (pos == int(stream->size())) {
            return;
        }
    }
}


void 
Parser::S(){
	if (my_find_(previous_lexemas, s) != 0) {
        	throw std::exception();
    	}
		if ((my_find(*components, s)) != nullptr){
			previous_lexemas.add(s);
			poliZ.add(my_find(*components, s));
		}
		else{
			throw std::exception();
		}
	if (cur_lex == IDENT){
		gl();
		if (cur_lex == LEFT_BR){
			gl();
			K();
			if (cur_lex == RIGTH_BR){
				gl();
			}
			else{
				throw std::exception();
			}
		}
		else{
			throw std::exception();
		}
	}
	else{
		throw std::exception();
	}
}

void 
Parser::K(){
	if (cur_lex == IDENT){
		S();
		while(cur_lex == COMMA){
			poliZ.add(nullptr);
			gl();
			S();
		}
		poliZ.add(nullptr);
	}
	else{
	}
}

MultiSet<Component *> &
Parser::get(){
	return poliZ;
}

void
Parser::parse(std::string const & arg, const MultiSet<Pair> & arg_){
	components = &arg_;
	stream = &arg;
	poliZ.clear();
	pos = 0;
	gl();
	S();
	if (cur_lex != END){
		throw std::exception();
	}
}


Component *make_from_text(std::string const &text, const MultiSet<Pair> & components){
	Parser a;
	arg.str(text);
	try {
		a.parse(text, components);
	}
	catch(...){
		throw std::exception();
	}
	MultiSet<Component *> & poliZ_fin = a.get();
	MultiSet<Component *> stack;
	for (size_t i = 0; i < poliZ_fin.size(); i++){
		if (poliZ_fin[i] != nullptr){
			stack.add(poliZ_fin[i]);
		}
		else{
			Component * node_1;
			stack.del(stack.size() - 1, node_1);
			Component * node_2 = stack[stack.size() - 1];
			(*node_2).add(*node_1);
		}
	}
	return poliZ_fin[0];
}

int
main() {
    MultiSet<Pair> components;
    Box A1("a1"), B1("b1"), C1("c1"),
    D1("d1"), G1("g1"), H1("h1"), L1("l1"), M1("m1");
    components.add({"a1", &A1});
    components.add({"b1", &B1});
    components.add({"c1", &C1});
    components.add({"d1", &D1});
    components.add({"g1", &G1});
    components.add({"h1", &H1});
    components.add({"l1", &L1});
    components.add({"m1", &M1});
    Component * node = nullptr;
    try {
        node = make_from_text("c1(a1(b1()),d1(g1(),h1(l1(),m1())))",
        components);
    }
    catch (...) {
        std::cout << "NO" << std::endl;
        return 0;
    }
    std::cout << *node << std::endl;
    return 0;
}


