// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef SORTEDPQ_H
#define SORTEDPQ_H

#include "Eecs281PQ.h"
#include <algorithm>
#include <iostream>

// A specialized version of the priority queue ADT that is implemented with
// an underlying sorted array-based container.
// Note: The most extreme element should be found at the end of the 'data'
//       container, such that traversing the iterators yields the elements in
//       sorted order.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class SortedPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison
    //              functor.
    // Runtime: O(1)
    explicit SortedPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
    } // SortedPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n log n) where n is number of elements in range.
    template<typename InputIterator>
    SortedPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        while(start != end) {
            data.push_back(*start);
            start++;
        }
        std::sort(data.begin(), data.end(), this->compare);
    } // SortedPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~SortedPQ() {
    } // ~SortedPQ()


    // Description: Add a new element to the PQ.
    // Runtime: O(n)
    virtual void push(const TYPE &val) {
        auto iter = std::lower_bound(data.begin(), data.end(), val, this->compare);
        data.insert(iter, 1, val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the PQ is empty. Though you are welcome to if
    //       you are familiar with them, you do not need to use exceptions in
    //       this project.
    // Runtime: Amortized O(1)
    virtual void pop() {
        data.pop_back();
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the vector. This should be a reference for speed. It
    //              MUST be const because we cannot allow it to be modified,
    //              as that might make it no longer be the most extreme
    //              element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return data.back();
    } // top()


    // Description: Get the number of elements in the PQ.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return data.size();
    } // size()


    // Description: Return true if the PQ is empty.
    //              This has been implemented for you.
    // Runtime: O(1)
    virtual bool empty() const {
        return data.empty();
    } // empty()


    // Description: Assumes that all elements inside the PQ are out of order
    //              and 'rebuilds' the PQ by fixing the PQ invariant.
    // Runtime: O(n log n)
    virtual void updatePriorities() {
        std::sort(data.begin(), data.end(), this->compare);
    } // updatePriorities()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;

}; // SortedPQ

#endif // SORTEDPQ_H
