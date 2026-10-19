#include <iostream>
#include <random>

using namespace std;

template <typename T>
struct Node {
    int key;
    T value;
    int priority;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;
    Node<T>* parent = nullptr;

    inline static mt19937 gen{random_device{}()};

    Node(int k, T v, int p = -1) : key{k}, value{v} {
        priority = (p >= 0) ? p : uniform_int_distribution<int>(1, 10000)(gen);
    }

    bool has_left() {
        // if left is something other than null, that means there is a child
        return left != nullptr;
    }

    bool has_right() {
        return right != nullptr;
    }

    bool has_parent() {
        return parent != nullptr;
    }

    bool is_leaf() {
        // If it doesn't have a left child and it doesn't have a right child, it
        // must be a leaf node.
        return !has_left() && !has_right();
    }
};

template <typename T>
struct BinarySearchTree {
    Node<T>* root = nullptr;

    /// Return the node in the tree with this value. If there is no such node, return nullptr.
    Node<T>* search(int key) {
        return _search_helper(root, key);
    }

    Node<T>* _search_helper(Node<T>* current_node, int key) {
        // TODO
        if(current_node==nullptr)
        {
            return nullptr;//nothing to search
        }

        if(current_node->key==key)
        {
            return current_node;//found it!
        }

        if(key<current_node->key)
        {
            return _search_helper(current_node->left,key);
        }
        else
        {
            return _search_helper(current_node->right,key);
        }
    }

    //in right rotation, left child rises up and left childs right subtree becomes currents left subtree
    //also after rising child, update the childs parents to parent of current
    void right_rotate(Node<T>* current)
    {
        Node<T>* child = current->left;
        Node<T>* middle = child->right;
        Node<T>* cur_parent = current->parent;

        //making child take current position under currents parent
        child->parent=cur_parent;//updating parent of child
        //making currents parent point to child
        if(cur_parent==nullptr)
        {
            //current was the root, since child replaces current
            root=child;
        }
        else if(cur_parent->left==current)
        {
            //current was left child
            cur_parent->left=child;
        }
        else
        {
            //current was a right child
            cur_parent->right=child;
        }

        //making current go under child
        child->right=current;
        current->parent=child;//updating currents parent

        //moving middle under current
        current->left=middle;
        if(middle!=nullptr)//this prevents accessing nullptr
        {
            middle->parent=current;//if middle exists, update its parent
        }
    }

    //in left rotation, right child rises up and right childs left subtree becomes currents right subtree
    //also after rising child, update the childs parents to parent of current
    void left_rotate(Node<T>* current)
    {
        Node<T>* child = current->right;
        Node<T>* middle = child->left;
        Node<T>* cur_parent = current->parent;

        //making child take current position under currents parent
        child->parent=cur_parent;//updating parent of child
        //making currents parent point to child
        if(cur_parent==nullptr)
        {
            //current was the root, since child replaces current
            root=child;
        }
        else if(cur_parent->left==current)
        {
            //current was left child
            cur_parent->left=child;
        }
        else
        {
            //current was a right child
            cur_parent->right=child;
        }

        //making current go under child
        child->left=current;
        current->parent=child;//updating currents parent

        //moving middle under current
        current->right=middle;
        if(middle!=nullptr)//this prevents accessing nullptr
        {
            middle->parent=current;//if middle exists, update its parent
        }
    }

    void rise_up(Node<T>* node)
    {
        //rise up using rotations based on priority
        //as along as nodes parents not null, rise node while its priority is greater than parents
        while(node->parent != nullptr && node->priority > node->parent->priority)
        {
            Node<T>* node_parent = node->parent;
            if(node_parent->left==node)
            {
                right_rotate(node_parent);//if node is the left child, we have to right rotate the parent
            }
            else
            {
                left_rotate(node_parent);//else nodes the right child, so left rotate the parent
            }
        }
    }

    void insert(int key, T value, int priority = -1) {
        Node<T>* val = new Node<T>(key, value, priority);
        // First, if the tree is empty, just make this the root node
        if (root == nullptr) {
            root = val;
            return;
        }
        // Again, I need a helper function to handle my recursion input
        return _insert_helper(root, val);
    }

    void _insert_helper(Node<T>* current_node, Node<T>* inserting) {
        // TODO
        if(inserting->key<current_node->key)
        {
            if(current_node->left==nullptr)
            {
                //insert there and update its parents
                current_node->left=inserting;
                inserting->parent=current_node;

                //rising up based on priority
                rise_up(inserting);
            }
            else
            {
                return _insert_helper(current_node->left,inserting);
            }
        }
        else if(inserting->key>current_node->key)
        {
            if(current_node->right==nullptr)
            {
                //insert there and update its parents
                current_node->right=inserting;
                inserting->parent=current_node;
                //rising up based on priority
                rise_up(inserting);
            }
            else
            {
                return _insert_helper(current_node->right,inserting);
            }
        }
        else//inserting key which is duplicate of currents key, so just update value 
        //and delete inserting node(because at start of insertion we create new node, without delete its memory unused )
        {
            current_node->value=inserting->value;
            delete inserting;
        }
    }

    void remove(int key) {
        return _remove_helper(root, key);
    }

    void _remove_helper(Node<T>* current_node, int key) {
        // TODO
        Node<T>* node = search(key);
        if(node==nullptr)
        {
            throw "Value not found";
        }
        //giving node low priority to force it down before deletion
        node->priority=-1;
        //while we dont make the delete node a leaf...
        //left rotate(if it has right child) or right rotate(if it has leftt child) to force it down
        //otherwise 2 children, rotate according to highest priority child
        while(!node->is_leaf())
        {
            if(node->has_left() && !node->has_right())
            {
                right_rotate(node);
            }
            else if(!node->has_left() && node->has_right())
            {
                left_rotate(node);
            }
            else
            {
                if(node->left->priority>node->right->priority)
                {
                    right_rotate(node);
                }
                else
                {
                    left_rotate(node);
                }
            }
        }
        //now that its a leaf,make its parent pont to ullpptr instead of pointing to delete node
        if(node->parent==nullptr)
        {
            //deleting root
            root=nullptr;
        }
        else if(node->parent->left == node)
        {
            //it was left child, so making parents left null
            node->parent->left=nullptr;
        }
        else
        {
            //it was right child, so making parents right null
            node->parent->right=nullptr;
        }
        //finally delete
        delete node;
    }

    void update(int key, T value) {
        Node<T>* node = search(key);
        if (node) node->value = value;
        else insert(key, value);
    }
};

int ontrack_test() {
    // Ontrack tests, do not change
    BinarySearchTree<string> bst;
    Node<string>* context_node = nullptr;
    string menu = "0. Show this menu\n1. Insert a key\n2. Update a key\n3. Search for a key\n4. Remove a key\n5. Exit";
    int command = 0;
    while (command != 5) {
        if (command == 0) {
            cout << menu << endl;
        } else if (command == 1) {
            //insert
            int key;
            string value;
            int priority;
            cout << "Enter key, value, priority: " << endl;
            cin >> key;
            cin >> value;
            cin >> priority;
            bst.insert(key, value, priority);
        } else if (command == 2) {
            //update
            int key;
            string value;
            cout << "Enter key, value: " << endl;
            cin >> key;
            cin >> value;
            bst.update(key, value);
        } else if (command == 3) {
            //search
            cout << "Enter key to search: " << endl;
            int key;
            cin >> key;
            context_node = bst.search(key);
            if (context_node) {
                cout << "Found node (" << context_node->key << ", " << context_node->value << ", " << context_node->priority << ")\n";
                cout << "Parent: " << (context_node->parent ? to_string(context_node->parent->key) : "nullptr") << " Left: " << (context_node->left ? to_string(context_node->left->key) : "nullptr") << " Right: " << (context_node->right ? to_string(context_node->right->key) : "nullptr") << endl;
            } else cout << "Could not find node with key: " << key << endl;
        } else if (command == 4) {
            //remove
            cout << "Enter key to remove: " << endl;
            int key;
            cin >> key;
            bst.remove(key);
        } 
        cout << "Enter command (0-5)" << endl;
        if (!(cin >> command)) {
            cout << "ERROR: could not read command" << endl;
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
    if (argc == 2) return ontrack_test();
    
    BinarySearchTree<string> bst;
    bst.insert(3, "a", 8);
    bst.insert(11, "b", 6);
    bst.insert(8, "c", 10);
    // At this point, c should be the root, and a/b should be left/right
    cerr << "Root: (" << bst.root->value << ", " << bst.root->priority << ")" << endl;
    cerr << "Left: (" << bst.root->left->value << ", " << bst.root->left->priority << ")" << endl;
    cerr << "Right: (" << bst.root->right->value << ", " << bst.root->right->priority << ")" << endl;
    bst.insert(7, "d", 5);
    bst.insert(9, "e", 4);
    bst.insert(5, "f", 2);
    // Now we should see the final structure of the tree.
    cerr << "===" << endl;
    cerr << "Root: (" << bst.root->value << ", " << bst.root->priority << ")" << endl; // c
    cerr << "Left: (" << bst.root->left->value << ", " << bst.root->left->priority << ")" << endl; // a
    cerr << "Right: (" << bst.root->right->value << ", " << bst.root->right->priority << ")" << endl; // b
    cerr << "Left->Right: (" << bst.root->left->right->value << ", " << bst.root->left->right->priority << ")" << endl; // d
    cerr << "Left->Right->Left: (" << bst.root->left->right->left->value << ", " << bst.root->left->right->left->priority << ")" << endl; // f
    cerr << "Right->Left: (" << bst.root->right->left->value << ", " << bst.root->right->left->priority << ")" << endl; // e

    return 0;
}
