#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    currentIndex = bag.head;
    currentFreq = 1;
}

TComp SortedBagIterator::getCurrent() {
    if (!valid()) {
           throw exception();
       }
    return bag.nodes[currentIndex].pair.first;
}

bool SortedBagIterator::valid() {
    return currentIndex != -1;
}

void SortedBagIterator::next() {
    if (!valid()) {
          // cerr << "next() called on invalid iterator.\n";
           throw exception();
       }

    int nodeFreq = bag.nodes[currentIndex].pair.second;
      // cerr << "next(): currentIndex = " << currentIndex << ", nodeFreq = " << nodeFreq << ", currentFreq = " << currentFreq << "\n";
       if (currentFreq < nodeFreq) {
           // Still have more occurrences of this element to return
           currentFreq++;
           //cerr << "Staying on same node, currentFreq = " << currentFreq << "\n";
       } else {
           // We have returned all occurrences of this element
           currentIndex = bag.nodes[currentIndex].next;
           currentFreq = 1;
           
       }
}

void SortedBagIterator::first() {
    currentIndex = bag.head;
    currentFreq = 1;
}

