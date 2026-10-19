#pragma once

#include <iostream>
#include <string>

using namespace std;

/*
--------------------
    TipTop Posts
--------------------
*/

struct Post {
    string username;
    int posted_at;
    int likes;
    int comments;
};


/*
----------------------------------------
    BST Implementation with Post objects
----------------------------------------
*/

struct Node {
    string key;
    Post* post;
    
    Node* left = nullptr;
    Node* right = nullptr;

    bool has_left() {
        // if left is something other than null, that means there is a child
        return left != nullptr;
    }

    bool has_right() {
        return right != nullptr;
    }

    bool is_leaf() {
        // If it doesn't have a left child and it doesn't have a right child, it
        // must be a leaf node.
        return !has_left() && !has_right();
    }
};

struct BinarySearchTree {
    Node* root = nullptr;

    // Number of nodes in the BST
    int size = 0;

    /// Returns the post in the tree with this key. If there is no such post, returns nullptr.
    Post* search(string key) {
        return _search_helper(root, key);
    }

    Post* _search_helper(Node* current_node, string key) {
        // If we are outside the tree, that means the key can't be found
        if (current_node == nullptr) return nullptr;
        // If current_node node has the key, return it
        if (current_node->key == key) return current_node->post;

        // Otherwise, check which child we should go to.
        if (key > current_node->key) {
            return _search_helper(current_node->right, key);
        } else {
            return _search_helper(current_node->left, key);
        }
    }

    void insert(string key, Post* post) {
        // If the tree is empty, just make this the root node
        if (root == nullptr) {
            root = new Node;
            root->key = key;
            root->post = post;
            size++;
            return;
        }
        // Again, I need a helper function to handle my recursion input
        return _insert_helper(root, key, post);
    }

    void _insert_helper(Node* current_node, string key, Post* post) {
        // If this node has this key, raise an error (because we are keeping our BST items unique)
        if (current_node->key == key) throw "Key already present in BST";

        // Check which direction we should go
        if (key > current_node->key) {
            // We should go to the right
            // If there is no right child, that means this is exactly where
            // the new node should be inserted
            if (current_node->right == nullptr) {
                current_node->right = new Node;
                current_node->right->key = key;
                current_node->right->post = post;
                size++;
                return;
            }
            // Otherwise, if there is a right child, continue there
            return _insert_helper(current_node->right, key, post);
        } else {
            // We should go to the left
            // If there is no left child, that means this is exactly where
            // the new node should be inserted
            if (current_node->left == nullptr) {
                current_node->left = new Node;
                current_node->left->key = key;
                current_node->left->post = post;
                size++;
                return;
            }
            // Otherwise, if there is a left child, continue there
            return _insert_helper(current_node->left, key, post);
        }
    }

    void delete_key(string key) {
        // First make sure the tree isn't empty
        if (root == nullptr) throw "Key not found, tree is empty";
        _delete_helper(nullptr, root, key);
    }

    void _delete_helper(Node* parent_node, Node* current_node, string key) {
        // Base case: is this the node that should be deleted?
        if (current_node->key == key) {
            // Scenario 1: if it's a leaf, nothing needs to be done.
            // Just remove the link from the parent to this, and delete
            // it from the memory
            if (current_node->is_leaf()) {
                // First delete this node
                delete current_node;
                // Then update its parent's link
                if (parent_node == nullptr) {
                    // It's the root node, and it doesn't have children
                    // So just assign the root to null
                    root = nullptr;
                } else if (parent_node->left == current_node) {
                    // It was the left child of its parent
                    parent_node->left = nullptr;
                } else if (parent_node->right == current_node) {
                    // It was the right child of its parent
                    parent_node->right = nullptr;
                }
            }
            // Scenario 2: it only has one child, which we should move up
            else if (current_node->has_left() && !current_node->has_right()) {
                // It has a left child
                if (parent_node == nullptr) {
                    // If this is the root, bringing up its child node means
                    // making it the new root node
                    root = current_node->left;
                    delete current_node;
                } else if (parent_node->left == current_node) {
                    // If this is the left child of its parent...
                    parent_node->left = current_node->left;
                    delete current_node;
                } else if (parent_node->right == current_node) {
                    // If this is the right child of its parent...
                    parent_node->right = current_node->left;
                    delete current_node;
                }
            } else if (!current_node->has_left() && current_node->has_right()) {
                // It has a right child
                if (parent_node == nullptr) {
                    root = current_node->right;
                    delete current_node;
                } else if (parent_node->left == current_node) {
                    parent_node->left = current_node->right;
                    delete current_node;
                } else if (parent_node->right == current_node) {
                    parent_node->right = current_node->right;
                    delete current_node;
                }
            }
            // Scenario 3: the node has both children, so I should actually
            // find the successor
            else if (current_node->has_left() && current_node->has_right()) {
                // I define a new method dedicated to finding the successor of
                // a node below, and I call it here:
                Node* successor = successor_of(current_node);
                // Remember its key
                string successor_key = successor->key;
                Post* successor_post = successor->post;
                // Delete the successor recursively
                delete_key(successor_key);
                // Put its key in the current node
                current_node->key = successor_key;
                current_node->post = successor_post;
                // Return before decrementing the size again, deleteing the successor did that.
                return;
            }
            size--;
            return;
        }

        // Otherwise, we need to continue our search for the node:
        if (key < current_node->key) {
            // If we need to go left, but there is no left, throw error
            if (!current_node->has_left()) throw "Value not found";
            // Otherwise, recurse. In the recursive call, the parent node
            // will be what's currently the `current_node`.
            _delete_helper(current_node, current_node->left, key);
        } else {
            if (!current_node->has_right()) throw "Value not found";
            _delete_helper(current_node, current_node->right, key);
        }
    }

    // Returns the node that contains the successor key of the input node
    // Based on the logic above, it is assumed that the input `node`
    // has both children.
    Node* successor_of(Node* node) {
        // Go right once (no need to check there is a right - there must be)
        node = node->right;
        // Go left as much as possible
        while (node->has_left()) node = node->left;
        // Return
        return node;
    }
};

/*
----------------------------------------------
    Max Heap Implementation with Post objects
----------------------------------------------
*/

struct MaxHeap {
    Post** array = new Post*[1];
    int current_capacity = 0;

    // Size means how many items are in the heap. We will need to resize
    // the array when size = capacity and new items are being added.
    int size = 0;

    void resize() {
        current_capacity = 2 * (current_capacity + 1);
        Post** new_array = new Post*[current_capacity + 1];

        for (int i = 1; i <= size; i++) {
            new_array[i] = array[i];
        }

        // We don't need the old array anymore!
        delete[] array;

        array = new_array;
    }

    int parent(int index) {
        return index / 2;
    }

    int left_child(int node_index) {
        return 2 * node_index;
    }

    int right_child(int node_index) {
        return 2 * node_index + 1;
    }

    // Add a new value to the heap
    void add(Post* value) {
        if (size == current_capacity) {
            resize();
        }

        array[size + 1] = value;
        size++;

        // Now bubble up the item until it's in its current place
        int current_index = size;
        // While there is a parent, and that parent is smaller than this...
        while (parent(current_index) >= 1 && array[parent(current_index)]->likes < array[current_index]->likes) {
            swap(array[parent(current_index)], array[current_index]);
            current_index = parent(current_index);
        }
    }

    void sink(int node_index) {
        // While there is a child...
        while (left_child(node_index) <= size) {
            int larger_child_index = left_child(node_index);
            // If there is also a right child...
            if (right_child(node_index) <= size) {
                // If the right child is larger than left, remember that
                if (array[right_child(node_index)]->likes > array[left_child(node_index)]->likes) {
                    larger_child_index = right_child(node_index);
                }
            }
            // If the larger of children is larger than this node, swap them
            if (array[larger_child_index]->likes > array[node_index]->likes) {
                swap(array[larger_child_index], array[node_index]);
                node_index = larger_child_index;
            }
            else {
                // As soon as this isn't the case, break the loop
                break;
            }
        }
    }

    // Remove the max item from the heap and return it.
    Post* extract_max() {
        if (size == 0) {
            return nullptr;
        }

        Post* max_val = array[1];
        swap(array[1], array[size]);
        size--;

        sink(1);
        return max_val;
    }

    // Construct this heap with the given array input
    void heapify(Post* arr[], int n) {
        delete[] array;
        array = new Post*[n + 1];
        current_capacity = n;

        for (int i = 0; i < n; i++) {
            array[i + 1] = arr[i];
        }
        size = n;

        for (int i = n; i >= 1; i--) {
            sink(i);
        }
    }
};
