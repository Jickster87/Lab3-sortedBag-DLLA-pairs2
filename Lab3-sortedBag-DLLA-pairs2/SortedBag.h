#pragma once
//DO NOT INCLUDE SORTEDBAGITERATOR
#include <utility>
//DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef TComp TFreq;
typedef std::pair<TElem, TFreq> TPair;

typedef bool(*Relation)(TComp, TComp);
#define NULL_TPAIR pair<TElem, TFreq>(-111111, 0);
#define NULL_TCOMP -11111;

class SortedBagIterator;

class SortedBag {
	friend class SortedBagIterator;

private:
    struct Node{
        int prev;
        int next;
        TPair pair;
    };
    Node * nodes; //sllarray point
    int maxCap;
    int firstEmpty;
    int sizeBag;
    int head;
    Relation rel;

public:
    void resize();
	//constructor
	SortedBag(Relation r);

	//adds an element to the sorted bag
	void add(TComp e);

	//removes one occurence of an element from a sorted bag
	//returns true if an eleent was removed, false otherwise (if e was not part of the sorted bag)
	bool remove(TComp e);

	//checks if an element appearch is the sorted bag
	bool search(TComp e) const;

	//returns the number of occurrences for an element in the sorted bag
	int nrOccurrences(TComp e) const;

	//returns the number of elements from the sorted bag
	int size() const;

	//returns an iterator for this sorted bag
	SortedBagIterator iterator() const;

	//checks if the sorted bag is empty
	bool isEmpty() const;

	//destructor
	~SortedBag();
};
