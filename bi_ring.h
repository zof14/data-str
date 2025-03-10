#ifndef BI_RING_H
#define BI_RING_H
#include <stdexcept>
#include <iostream>
template <typename Key, typename Info>
class bi_ring	
{
private:
	struct Node{
		Key key;
		Info info;
		Node* next;
		Node* previous;
		Node(const Key& ke,const Info& inf): key(ke), info(inf), previous(nullptr), next(nullptr){};
		
	};
	Node* head=nullptr;
	unsigned size=0;
public:
	class iterator { 
		friend class bi_ring;
		private:
			Node * it=nullptr;

		public:
			iterator(Node *curr=nullptr): it(curr){};

			Info& info()const{
				return it->info;
			};
			Key& key()const{
				return it->key;};
			
			iterator& operator++(){
				if(it){
				it = it->next;}
				return *this;
			};
			iterator& operator--(){
				it = it->previous;
				return *this;
			};
			void set_info(const Info& new_info) {
				if (it) {
					it->info = new_info;
				} 
				else {
					throw std::runtime_error("Invalid");
				}
			}
			
			bool operator==(const iterator& other) const {
				return it == other.it;
			}

			bool operator!=(const iterator& other) const {
				return it != other.it;
			}

	 };

	
	class const_iterator {
		friend class bi_ring;
		private:
			const Node * it=nullptr;

		public:
			const_iterator(Node* curr=nullptr): it(curr){};

			const Info& info()const{
				return it->info;
			};
			const Key& key()const{
				return it->key;
			};
			const_iterator& operator++(){
				it = it->next;
				return *this;
			};
			const_iterator& operator--(){
				it = it->previous;
				return *this;
			};
			bool operator!=(const const_iterator& other) const {
        		return it != other.it;  
			};
			bool operator==(const const_iterator& other) const {
        	return it == other.it; 
    };
			
			
	};

	bi_ring(): size(0){};
	bi_ring(const bi_ring& src): size(0){
		*this=src;
	}
	~bi_ring(){
		clear();
	};
	bi_ring& operator=(const bi_ring& src){
		if (this==&src){
			return *this;
		}
		else{
			clear();
			if(!src.empty()){
			iterator it = src.begin();
			do{
				push_back(it.key(),it.info());
				++it;
			}while(it!=src.begin());
		}}
		return *this;

		};
	
	iterator push_front(const Key& key, const Info& info) {
		if (empty()) {
			head = new Node(key, info);
			head->next = head;
			head->previous = head;
		} else {
			Node* temp = new Node(key, info);
			temp->next = head;
			temp->previous = head->previous;
			head->previous->next = temp;
			head->previous = temp;
			head = temp;
		}
		size++;
		return iterator(head);};

	iterator pop_front(){
		if (head == nullptr) {
        	throw std::runtime_error("empty");}
    
    	Node* temp = head; 
		if (size == 1) {
			head = nullptr; 
		} 
		else {
			head->previous->next = head->next;
			head->next->previous = head->previous; 
			head = head->next;
		}
		
		delete temp; 
		size--;
		return iterator(head); 
}

	iterator push_back(const Key& key, const Info& info){
		if (empty()){
			head=new Node(key,info);
			head->next = head;
        	head->previous = head;}
		else{
			Node* curr=head->previous;
			Node* added = new Node(key,info);
			curr->next=added;
			added->next=head;
			added->previous=curr;
			head->previous=added;}
		size++;
		return iterator(head->previous);
	}

	iterator insert(iterator position,const Key& key, const Info& info){
		if (!position.it){
			throw std::invalid_argument("Doesn't exist!");}
        Node* curr = position.it;
        Node* added = new Node(key, info);
        Node* prev = curr->previous;
        prev->next = added;
        added->previous = prev;
        added->next = curr;
        curr->previous = added;
        if (curr == head){
			head = added;}
        ++size;
        return iterator(added);
    };
	iterator erase(iterator position){
		if (position.it==nullptr)
			{ throw std::invalid_argument("Doesn't exist!");}
        if (empty())
			{ throw std::runtime_error("Empty list");}
        Node* curr = position.it;
        if (curr->next == curr) {
            head = nullptr;
        } 
		else {
            Node* prev = curr->previous;
            Node* next = curr->next;
            prev->next = next;
            next->previous = prev;
            if (curr == head) 
				{head = next;}
        }
        delete curr;
        --size;
        return iterator(head);
    };
	iterator begin() const{
		return iterator(head);
	};
	iterator find(Key&k){
		if(empty()){
			return nullptr;
		}
		iterator start=begin();
		iterator curr=start;
		do{
			if(curr.key()==k){
				return curr;
			}
			++curr;

		}while(curr!=start);
		return nullptr;

	}
	const_iterator cbegin() const{
				return const_iterator(head);
			};
	void print(){
		if(empty()){
			std::cout<<"{}"<<std::endl;
		}
		std::cout<<"{ "<<std::endl;

		const_iterator start= this->cbegin();
		const_iterator current=start;
		do{
			std::cout<<"{"<<current.key() << "," << current.info()<< "} ";
			++current;



		}while(current!=start);
		std::cout<<"}"<<std::endl;

	}
	

	
	void clear(){
		while (size > 0) {
            pop_front();
        }
    };
	
	bool empty() const{ 
		return head == nullptr;};
	unsigned get_size() const {
        return size; 
    };
};
#endif