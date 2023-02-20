// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            //       to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, parent{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *parent; // idk if this should be parent or previous
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, count{ 0 } {
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, count{ 0 } {
        while(start != end) {
            addNode(*start);
            start++;
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        // TODO: Implement this function.
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        //       lecture.

        // This line is present only so that this provided file compiles.
        (void)rhs; // TODO: Delete this line

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        std::deque<Node*> queue;
        Node* child = root->child;
        delete root;
        if(child == nullptr) {
            this->root = nullptr;
            count = 0;
            return;
        }
        Node* temp = child;
        child->parent = nullptr;
        size_t originalCount = count;

        // push all children to a queue, separating them
        while(child != nullptr) {
            queue.push_back(child);
            child = child->sibling;
            temp->sibling = nullptr;
            temp = child;
        }

        Node* root1;
        Node* root2;
        PairingPQ pqResult;
        pqResult.root = queue.front();
        while(queue.size() > 1) {
            root1 = queue.front(); queue.pop_front();
            root2 = queue.front(); queue.pop_front();
            PairingPQ pq1; pq1.root = root1;
            PairingPQ pq2; pq2.root = root2;
            pqResult = meld(pq1, pq2);
            queue.push_back(pqResult.root);
        }

        this->root = pqResult.root;
        count = originalCount - 1;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return count == 0;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        Node* parent = node->parent;
        // check if the node being updates is the root, or if it's parent value is still
        // more extreme. In either case, nothing else needs to be done
        if(this->root == node || !this->compare(parent->elt, new_value)) {
            return;
        }

        // make a new pairing heap with the updated node as the root
        PairingPQ pq;
        pq.root = node;
        //

        // cut off the two trees
        Node* parentNode = node->parent;
        parentNode->child = nullptr;
        node->parent = nullptr;
        //

        meldThis(*this, pq);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        Node *newNode = new Node(val);
        // if the current pq is empty
        if(root == nullptr) {
            root = newNode;
        }
        // if the current pq is not empty
        else {
            // create a new pairing heap with the new value
            PairingPQ pq;
            pq.root = newNode;
            //

            // meld the new heap with the exising one
            meldThis(*this, pq);
        }
        count++;
        return newNode;
    } // addNode()


private:

    // returns a new priority queue which melded the two inputs
    PairingPQ meld(PairingPQ& pq1, PairingPQ& pq2) {
        Node* pq1Root = pq1.root;
        Node* pq2Root = pq2.root;
        PairingPQ pq;
        // if the most extreme element of pq1 is less extreme than that of pq2
        if(this->compare(pq1.top(), pq2.top())) {
            pq1Root->sibling = pq2Root->child;
            pq1Root->parent = pq2Root;
            pq2Root->child = pq1Root;
            pq.root = pq2Root;
        }
        // if the most extreme element of pq1 is equal to or more extreme than that of pq2
        else {
            pq2Root->sibling = pq1Root->child;
            pq2Root->parent = pq1Root;
            pq1Root->child = pq2Root;
            pq.root = pq1Root;
        }
        pq.count = pq1.count + pq2.count;
        return pq;
    }

    // modifies this
    void meldThis(PairingPQ& pq1, PairingPQ& pq2) {
        Node* pq1Root = pq1.root;
        Node* pq2Root = pq2.root;
        // if the most extreme element of pq1 is less extreme than that of pq2
        if(this->compare(pq1.top(), pq2.top())) {
            pq1Root->sibling = pq2Root->child;
            pq1Root->parent = pq2Root;
            pq2Root->child = pq1Root;
            root = pq2Root;
        }
        // if the most extreme element of pq1 is equal to or more extreme than that of pq2
        else {
            pq2Root->sibling = pq1Root->child;
            pq2Root->parent = pq1Root;
            pq1Root->child = pq2Root;
            root = pq1Root;
        }
        count = pq1.count + pq2.count;
    }

    Node* root;
    size_t count;

};


#endif // PAIRINGPQ_H
