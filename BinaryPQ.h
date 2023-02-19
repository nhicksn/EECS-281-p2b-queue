// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
    } // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        while(start != end) {
            data.push_back(*start);
            start++;
        }
        updatePriorities();
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of
    //              order and 'rebuilds' the heap by fixing the heap
    //              invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        size_t maxVal = (size_t)-1;
        for(size_t i = data.size()/2; i < maxVal; i--) {
            fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        data.push_back(val);
        fixUp(data.size() - 1);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: O(log(n))
    virtual void pop() {
        data[0] = data.back();
        data.pop_back();
        fixDown(0);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return data.front();
    } // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    virtual void fixUp(size_t index) {
        if(index == 0 || !this->compare(data[(index - 1)/2], data[index])) {
            return;
        }
        else {
            std::swap(data[(index - 1)/2], data[index]);
            fixUp((index - 1)/2);
        }
    }

    virtual void fixDown(size_t index) {
        // if you're at a leaf node
        if(index >= (data.size()/2)) {
            return;
        }
        size_t largestIndex = index;
        // check if the left child is bigger than the current node, update index if so
        if(this->compare(data[index], data[(2*index) + 1])) {
            largestIndex = (2*index) + 1;
        }
        // make sure right child exists, then do the same as above with current largest
        if(((2*index) + 2) < data.size() && this->compare(data[largestIndex], data[(2*index) + 2])) {
            largestIndex = (2*index) + 2;
        }
        // if a larger node was found, swap and fixDown next node, otherwise return
        if(largestIndex == index) return;
        else {
            std::swap(data[index], data[largestIndex]);
            fixDown(largestIndex);
        }
    }

    // TODO: Add any additional member functions you require here. For
    //       instance, you might add fixUp() and fixDown().
}; // BinaryPQ


#endif // BINARYPQ_H
