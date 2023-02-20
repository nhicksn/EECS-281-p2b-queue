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
            Node *parent;
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
        BaseClass{ other.compare }, root{ nullptr }, count{ 0 } {
        std::deque<Node*> queue;
        queue.push_back(other.root);
        while(!queue.empty()) {
            Node* current = queue.front(); queue.pop_front();
            if(current->child != nullptr) { queue.push_back(current->child); }
            if(current->sibling != nullptr) { queue.push_back(current->sibling); }
            addNode(current->elt);
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        PairingPQ temp(rhs);

        std::swap(count, temp.count);
        std::swap(root, temp.root);
        
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        if(root == nullptr) { return; }
        std::deque<Node*> queue;
        queue.push_back(root);
        while(!queue.empty()) {
            Node* current = queue.front(); queue.pop_front();
            if(current->child != nullptr) { queue.push_back(current->child); }
            if(current->sibling != nullptr) { queue.push_back(current->sibling); }
            delete current;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        if(root == nullptr) { return; }
        std::deque<Node*> queue;
        queue.push_back(root);
        root = nullptr;
        Node* current;
        while(!queue.empty()) {
            current = queue.front(); queue.pop_front();
            if(current->child != nullptr) { queue.push_back(current->child); }
            if(current->sibling != nullptr) { queue.push_back(current->sibling); }
            current->child = nullptr; current->sibling = nullptr; current->parent = nullptr;
            meldThis(root, current);
        }
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
        Node* child = root->child;
        delete root;

        // if there was only one node in the tree
        if(child == nullptr) {
            root = nullptr;
            count = 0;
            return;
        }

        Node* temp = child;
        child->parent = nullptr;
        std::deque<Node*> queue;

        // push all children to a queue, separating them
        while(child != nullptr) {
            queue.push_back(child);
            child = child->sibling;
            temp->sibling = nullptr;
            temp->parent = nullptr;
            temp = child;
        }

        Node* root1;
        Node* root2;

        // set to queue.front() in case the queue has only size 1
        Node* resultRoot = queue.front();

        while(queue.size() > 1) {
            root1 = queue.front(); queue.pop_front();
            root2 = queue.front(); queue.pop_front();
            resultRoot = meld(root1, root2);
            queue.push_back(resultRoot);
        }

        this->root = resultRoot;
        count--;
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
        if(node == nullptr) { return; }
        node->elt = new_value;
        Node* parentNode = node->parent;
        // check if the node being updated is the root, or if it's parent value is still
        // more extreme or equal. In either case, nothing else needs to be done
        if(parentNode == nullptr || !this->compare(parentNode->elt, new_value)) {
            return;
        }

        // cut off the two trees
        parentNode->child = nullptr;
        node->parent = nullptr;
        if(node->sibling != nullptr) {
            Node* sibling = node->sibling;
            while(sibling->sibling != node) {
                sibling = sibling->sibling;
            }
            sibling->sibling = nullptr;
        }
        node->sibling = nullptr;
        
        //

        meldThis(root, node);
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
            // meld the new heap with the exising one
            meldThis(root, newNode);
        }
        count++;
        return newNode;
    } // addNode()


private:

    // returns a new root node which melded the two inputs
    Node* meld(Node* pq1Root, Node* pq2Root) {
        Node* result;
        // if the most extreme element of pq1 is less extreme than that of pq2
        if(this->compare(pq1Root->elt, pq2Root->elt)) {
            pq1Root->sibling = pq2Root->child;
            pq1Root->parent = pq2Root;
            pq2Root->child = pq1Root;
            result = pq2Root;
        }
        // if the most extreme element of pq1 is equal to or more extreme than that of pq2
        else {
            pq2Root->sibling = pq1Root->child;
            pq2Root->parent = pq1Root;
            pq1Root->child = pq2Root;
            result = pq1Root;
        }
        return result;
    }

    // modifies this
    void meldThis(Node* pq1Root, Node* pq2Root) {
        if(pq1Root == nullptr) { root = pq2Root; return; }
        else if(pq2Root == nullptr) { root = pq1Root; return; }
        // if the most extreme element of pq1 is less extreme than that of pq2
        if(this->compare(pq1Root->elt, pq2Root->elt)) {
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
        return;
    }

    Node* root;
    size_t count;

};


#endif // PAIRINGPQ_H
