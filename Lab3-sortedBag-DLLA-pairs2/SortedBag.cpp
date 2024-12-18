#include "SortedBag.h"
#include "SortedBagIterator.h"
using namespace std;

SortedBag::SortedBag(Relation r) {
    maxCap = 31;
    firstEmpty = 0;
    head = -1;
    sizeBag = 0;
    nodes = new Node[maxCap];
    rel = r;
    // Initialize nodes array
    for (int i = 0; i < maxCap; i++) {
        nodes[i].pair = NULL_TPAIR;
        // Initialize next pointer
        if (i < maxCap - 1) {
            nodes[i].next = i + 1;
        } else {
            nodes[i].next = -1;  // Last node points to no next
        }
        // Initialize prev pointer
        if (i > 0) {
            nodes[i].prev = i - 1;
        } else {
            nodes[i].prev = -1;  // First node has no previous
        }
    }
}

void SortedBag::resize() {
    int oldCap = maxCap;
    maxCap *= 2;
    Node* newNodes = new Node[maxCap];
    // Copy existing nodes
    for (int i = 0; i < oldCap; i++) {
        newNodes[i] = nodes[i];
    }
    // Initialize the new portion of the array
    for (int i = oldCap; i < maxCap; i++) {
        newNodes[i].pair = NULL_TPAIR;
        // Initialize next pointer
        if (i < maxCap - 1) {
            newNodes[i].next = i + 1;
        } else {
            newNodes[i].next = -1;  // Last new node points to no next
        }
        // Initialize prev pointer for free list nodes
        newNodes[i].prev = -1;      // Free nodes have no previous in the free list
    }
    firstEmpty = oldCap;
    delete[] nodes;
    nodes = newNodes;
}

void SortedBag::add(TComp e) {
    if (firstEmpty == -1) { // Resize if no spots left
        resize();
    }
    
    // Check if the element already exists
    int nomad = head;
    while (nomad != -1) {
        if (nodes[nomad].pair.first == e) {
            nodes[nomad].pair.second += 1; // Increment frequency
            sizeBag++;
            return;
        }
        nomad = nodes[nomad].next;
    }

    // Insert the new element in the correct sorted position
    int newPos = firstEmpty;                     // Allocate a new node
    firstEmpty = nodes[firstEmpty].next;         // Update firstEmpty to the next free node

    nodes[newPos].pair = std::make_pair(e, 1);   // Set the element and its frequency

    // Case 1: Empty list, insert as the head
    if (head == -1) {
        nodes[newPos].next = -1;
        nodes[newPos].prev = -1;
        head = newPos;
    }
    // Case 2: Insert before the head
    else if (!rel(nodes[head].pair.first, e)) { // If e should come before head
        nodes[newPos].next = head;               // New node points to current head
        nodes[newPos].prev = -1;                 // New node has no previous
        nodes[head].prev = newPos;               // Old head's prev points to new node
        head = newPos;                           // Update head to new node
    }
    // Case 3 and 4: Insert in the middle or at the tail
    else {
        int prev = head;
        nomad = nodes[head].next;

        // Traverse to find the insertion point based on rel
        while (nomad != -1 && rel(nodes[nomad].pair.first, e)) {
            prev = nomad;
            nomad = nodes[nomad].next;
        }

        // Insert after prev
        nodes[newPos].next = nomad;               // New node points to current
        nodes[newPos].prev = prev;                // New node's prev points to prev

        nodes[prev].next = newPos;                // Prev node points to new node

        if (nomad != -1) {
            nodes[nomad].prev = newPos;           // Current node's prev points to new node
        }
    }
    sizeBag++;
}

bool SortedBag::remove(TComp e) {
    // Case 1: Empty bag
    if (head == -1) {
        return false;
    }

    int current = head;

    // Find the node with the given element
    while (current != -1 && nodes[current].pair.first != e) {
        current = nodes[current].next;
    }

    // Case 2: Element not found
    if (current == -1) {
        return false;
    }

    // We found the element at 'current'
    // Check frequency
    if (nodes[current].pair.second > 1) {
        // Just decrement frequency if more than one occurrence
        nodes[current].pair.second -= 1;
        sizeBag--;
        return true;
    } else {
        // Frequency is exactly 1, remove the node entirely
        sizeBag--;

        if (current == head) {
            // Removing the head node
            head = nodes[current].next;
            if (head != -1) {
                nodes[head].prev = -1;  // New head has no previous
            }
        } else {
            // Removing a node in the middle or end
            int prev = nodes[current].prev;
            int next = nodes[current].next;

            nodes[prev].next = next;

            if (next != -1) {
                nodes[next].prev = prev;
            }
        }

        // Add the removed node back to the free list
        nodes[current].pair = NULL_TPAIR;
        nodes[current].next = firstEmpty;
        nodes[current].prev = -1;  // Optional: No previous in free list
        firstEmpty = current;

        return true;
    }
}

bool SortedBag::search(TComp elem) const {
    int nomad = head;
    while (nomad != -1 && nodes[nomad].pair.first != elem) {
        nomad = nodes[nomad].next;
    }
    if (nomad != -1) {
        return true;
    } else {
        return false;
    };
}

int SortedBag::nrOccurrences(TComp elem) const {
    int nomad = head;
    while (nomad != -1) {
        if (nodes[nomad].pair.first == elem) {
            return nodes[nomad].pair.second;
        }
        nomad = nodes[nomad].next;
    }
    return 0;
}

int SortedBag::size() const {
    return sizeBag;
}

bool SortedBag::isEmpty() const {
    return sizeBag == 0;
}

SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}

SortedBag::~SortedBag() {
    delete [] nodes;
}
