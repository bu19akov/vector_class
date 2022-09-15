#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>

using namespace std;
template<typename T>
class Vector {
public:
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;
private:
	size_t sz; //Anzahl der Elemente im Vector
	size_t max_sz; //maximale Anzahl an Elementen
	static constexpr size_t min_sz{5}; //Mindestgroesse
	value_type* values; //Zeiger aud ein Feld
public:
	Vector() : Vector(min_sz){ //Defaultkonstruktor
	}
	
	Vector(size_t n){ //Konstruktor mit Perameterliste
		if (n <= min_sz)
			n = min_sz;
		sz = 0;
		values = nullptr;
		max_sz = 0;
		reserve(n);
	}
	
	Vector(std::initializer_list<value_type> list) : Vector(list.size()){
		for (auto l :list)
			values[sz++] = l;
	}
	
	Vector(const Vector& src) { //Kopierkonstruktor
		max_sz = src.max_sz;
		values = new value_type[max_sz];
		for (size_t i{0}; i < src.sz; ++i)
			values[i] = src.values[i];
		sz = src.sz;
	}
	
	Vector operator= (Vector src) { //Kopierzuweisungsoperator
		swap(sz, src.sz);
		swap(max_sz, src.max_sz);
		swap(values, src.values);
		return *this;
	}
	
	~Vector() {
		delete[] values;
	}
	
	void reserve(size_t n) { //Kapazitaet auf n vergroessern
		if (n > max_sz) {
			size_t new_sz = n;
			pointer new_values = new value_type[new_sz];
			for (size_t i{0}; i < sz; ++i)
				new_values[i] = values[i];
			max_sz = new_sz;
			delete[] values;
			values = new_values;
		}
	}
	
	void push_back(const_reference val) {
		if (sz >= max_sz) {
			reserve(max_sz*2);
		}
		values[sz++]=val;
	}
	
	void clear() {
		sz = 0;
	}
	
	ostream& print(ostream& o) const {
		o << '[';
		bool first{true};
		for (size_t i{0}; i < sz; ++i) {
			if (first)
				first = false;
			else
				o << ", ";
			o << values[i];
		}
		return o << ']';
	}
	
	size_t size() const {
		return sz;
	}
	
	bool empty() const {
		return (sz == 0);
	}
	
	void shrink_to_fit() {
		if (sz >= min_sz) {
			value_type* new_values = new value_type[sz];
			for (size_t i{0}; i < sz; ++i)
				new_values[i] = values[i];
			max_sz = sz;
			delete[] values;
			values = new_values;
		}
		else {
			value_type* new_values = new value_type[min_sz];
			for (size_t i{0}; i < sz; ++i)
				new_values[i] = values[i];
			max_sz = min_sz;
			delete[] values;
			values = new_values;
		}
	}
	
	void pop_back() {
		if (sz == 0)
			throw runtime_error{"Vector is empty"};
		else 
			sz--;
	}
	
	value_type& operator[](size_t index) {
		if (index > sz-1 || index < 0)
			throw runtime_error{"illegal index"};
		else
			return values[index];
	}
	
	const value_type& operator[](size_t index) const {
		if (index > sz-1 || index < 0)
			throw runtime_error{"illegal index"};
		else
			return values[index];
	}
	
	size_t capacity() const {
		return max_sz;
	}
	
	iterator begin(){
		return {values, values+sz}; 
	}
	
	iterator end(){
		return {values+sz, values+sz};
	}
	
	const_iterator begin() const{
		return {values, values+sz};
	}
	
	const_iterator end() const{
		return {values+sz, values+sz};
	}
	
	iterator insert(const_iterator pos, const_reference val){ 
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) > sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)}; 
		if (sz >= max_sz)
			reserve(max_sz*2);
		for (auto i{sz}; i-- > current;)
			values[i+1] = values[i]; 
		values[current] = val ;
		++sz;
		return iterator{values+current};
	}
	
	iterator erase(const_iterator pos){ 
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)}; 
		for (auto i{current}; i < sz - 1; ++i)
			values[i] = values[i+1]; 
		--sz;
		return iterator{values+current}; 
	}
	
	class Iterator{
		public:
			using value_type = Vector::value_type;
			using reference = Vector::reference;
			using pointer = Vector::pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = std::forward_iterator_tag;
		private:
			pointer ptr; 
			pointer end;
			bool is_end() const {
				return ptr == end || ptr == nullptr;
			};
		public:
			Iterator() : ptr(nullptr){
			}
			
			Iterator(pointer ptr, pointer end = nullptr) : ptr(ptr), end(end){
			}
			
			bool operator==(const const_iterator& p) const{
				return const_iterator(ptr) == p;
			}
			bool operator!=(const const_iterator& p) const{
				return const_iterator(ptr) != p;
			}
			
			reference operator*() const{
				if (is_end())
					throw runtime_error{" "};
				return *ptr;
			}
			
			pointer operator->() const{
				if (is_end())
					throw runtime_error{" "};
				return ptr;
			}
			
			iterator& operator++(){
				if (!is_end())
					++ptr;
				return *this;
			}
			
			iterator operator++(int){
				Iterator help;
				help.ptr = ptr;
				if (!is_end())
					++ptr;
				return help;
			}
			
			operator const_iterator() const{
				ConstIterator help{ptr};
				return help;
			}
	};
	
	class ConstIterator{
		public:
			using value_type = Vector::value_type;
			using reference = Vector::const_reference;
			using pointer = Vector::const_pointer;
			using difference_type = Vector::difference_type;
			using iterator_category = std::forward_iterator_tag;
		private:
			friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop){
				return lop.ptr - rop.ptr;
			}
			pointer ptr;
			pointer end;
			bool is_end() const {
				return ptr == end || ptr == nullptr;
			};
		public:
			ConstIterator() : ptr(nullptr){
			}
			
			ConstIterator(pointer ptr, pointer end = nullptr) : ptr(ptr), end(end){
			}
			
			reference operator*() const{
				if (is_end())
					throw runtime_error{" "};
				return *ptr;
			}
			
			pointer operator->() const{
				if (is_end())
					throw runtime_error{" "};
				return ptr;
			}
			
			bool operator==(const const_iterator& p) const{
				return ptr == p.ptr;
			}
			
			bool operator!=(const const_iterator& p) const{
				return ptr != p.ptr;
			}
			
			const_iterator& operator++(){
				if (!is_end())
					++ptr;
				return *this;
			}
			
			const_iterator operator++(int){
				ConstIterator help;
				help.ptr = ptr;
				if (!is_end())
					++ptr;
				return help;
			}
	};
	
};
	template<typename T>
	inline ostream& operator<<(ostream& o, const Vector<T>& v) {
		return v.print(o);
	}

#endif
