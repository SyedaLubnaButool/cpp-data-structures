#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct ContactBook;
struct ContactBookNode;

// You may assume that the User->nickname always matches the nickname it is inserted under.
struct User {
    string nickname;
    string display_name;
    int user_score;
};

struct ContactBookNode {
    string nickname;
    User* associated_user;
    // This will remain a nullptr if we don't see a collision.
    ContactBook* recursed_book = nullptr;

    // Define this after the ContactBook struct is defined.-
    ~ContactBookNode();
};

struct ContactBook {
    const int table_size = 27;
    // Depth determines how the hash table functions
    int depth = 0;
    int size = 0;
    ContactBookNode** table = new ContactBookNode*[table_size];
    
    ContactBook() : ContactBook(0) {}
    ContactBook(int start_depth) : depth(start_depth) {
        //every position must initially be empty
        for(int i=0;i<table_size;i++)
        {
            table[i]=nullptr;
        }
    }
    ~ContactBook() {
        for (int i=0; i<table_size; i++) {
            delete table[i];
        }
        delete[] table;
    }


    //uses char at current depth to choose one of the 27 positions
    int hash(const string& nickname) {
        // Use the depth parameter!
        if(depth>=nickname.length())
        {
            return 26;//for when theres no char at this depth
            //it gets the last, special, no charecter place- 26
        }
        return nickname[depth]-'a';//using a to convert letters to 0-25
    }


    // stores directly if empty, on collision it creates a nested contactbook at next depth 
    //and recursively inserts both contacts
    void insert_user(const string& nickname, User* user_data) {
        // Task 1
        
        int position = hash(nickname);
        //1st case: have nothing at this position
        if(table[position]==nullptr)
        {
            //storing the contact as new node in the empty position
            ContactBookNode* node = new ContactBookNode;
            
            node->nickname=nickname;
            node->associated_user=user_data;
            
            table[position]=node;
            size++;
            return;
        }
        //2nd case: having one contactbook at the position, 
        //a collision happened here previously, so contactbook already exists
        ContactBookNode* node =table[position];
        if(node->recursed_book!=nullptr)
        {
            //letting the deeper contactbook handle insertion
            node->recursed_book->insert_user(nickname,user_data);
            size++;
            return;
        }

        //3rd case: one user at this position and collision with nicknames

        //remembering old users info
        string old_nickname = node->nickname;
        User* old_user = node->associated_user;

        //this node now represents a nested contactbook 
        //than just user which it represented previously
        node->nickname="";
        node->associated_user=nullptr;

        //creating a new contactbook with 1 more char depth 
        node->recursed_book= new ContactBook(depth+1);

        //put the old contact into deeper book
        node->recursed_book->insert_user(old_nickname,old_user);
        
        //put the new user into deeper book
        //if they collide again, insert_user recursively creates another contactbook
        node->recursed_book->insert_user(nickname,user_data);

        size++;

    }

    User* retrieve_user(const string& nickname) {
        // Task 1
        int position = hash(nickname);

        ContactBookNode* node = table[position];

        //if nothing exists at this position return nullptr
        if(node==nullptr)
        {
            return nullptr;
        }

        //if position contains another contactbook, recurse to look one level lower
        if(node->recursed_book!=nullptr)
        {
            return node->recursed_book->retrieve_user(nickname);
        }

        //reached actual contact, match nickname and return
        if(node->nickname==nickname)
        {
            return node->associated_user;
        }
        //a node existed but it wasnt our nickname
        return nullptr;
    }

    //deletion helper function, if size is one after deletion returns the single node remaining after
    // to just keep it and not the layers of nested contactbooks 
    ContactBookNode* get_single_node()
    {
        //search all 27 slots
        for (int i=0;i<table_size;i++)
        {
            if(table[i]!= nullptr)
            {
                //if theres a nested contactbook, continue search recursively
                if(table[i]->recursed_book!=nullptr)
                {
                    return table[i]->recursed_book->get_single_node();
                }
                //found the actual remaining contact
                return table[i];
            }
        }
        //no contact exists
        return nullptr;
    }

    //recursively removes the contact and collapses nested contactbooks if after deletion one contact remains
    void delete_user(const string& nickname) {
        // Task 3
        int position=hash(nickname);
        ContactBookNode* node = table[position];

        //nothing exists here so nothing can be deleted
        if(node ==nullptr)
        {
            return;
        }

        //theres a nested contactbook
        if(node->recursed_book!= nullptr)
        {
            //ask deeper book to delete
            node->recursed_book->delete_user(nickname);
        

            //one contact remains after deletion, so collapse the nested structure
            if(node->recursed_book->size==1)
            {
                ContactBookNode* one = node->recursed_book->get_single_node();
                string one_nickname = one->nickname;
                User* one_user = one->associated_user;

                delete node;

                ContactBookNode* replacement = new ContactBookNode;
                replacement->nickname=one_nickname;
                replacement->associated_user=one_user;

                table[position] = replacement;

            }
            size--;
            return;
        }

        //just one contact present and we r deleting it
        if(node->nickname==nickname)
        {
            delete node;
            table[position]= nullptr;
            size--;

        }
    }

    // recursively searches stored contacts for any nickname beginning with given prefix
    bool search(const string& nickname) {
        // Task 4 

        //once we reach end of nickname searched, if theres any contact's present-- stop here as prefixs been matched and we have contacts
        if(depth>=(int)nickname.length())
        {
            return size>0;
        }
        
        int position = hash(nickname);
        ContactBookNode* node = table[position];

        //if nothing exists at this position return nullptr
        if(node==nullptr)
        {
            return false;
        }

        //if theres contactbook, recursively search
        if(node->recursed_book!= nullptr)
        {
            return node->recursed_book->search(nickname);
        }

        //if we reached here, just one contact present
        string stored_nickname = node->nickname;

        //a shorter nickname cannot contain a longer prefix
        if(stored_nickname.length()<nickname.length())
        {
            return false;
        }

        //compare each char of prefix
        for(int i=0;i<int(nickname.length());i++)
        {
            //if any chars differs its not a match
            if(stored_nickname[i]!=nickname[i])
            {
                return false;
            }
        }
        //all chars matched
        return true;

    }
};

ContactBookNode::~ContactBookNode() {
    // This needs to be defined after the ContactBook definition.
    delete recursed_book;
}

// Nicknames are only allowed to contain the lowercase characters a-z, so anything
// else would send hash() outside the bounds of the table. main() checks this before
// it hands anything to the ContactBook.
bool valid_nickname(const string& nickname) {
    if (nickname.empty()) {
        return false;
    }
    for (int i = 0; i < (int)nickname.length(); i++) {
        if (nickname[i] < 'a' || nickname[i] > 'z') {
            return false;
        }
    }
    return true;
}

int main() {
    ContactBook book;

    // The ContactBook only ever borrows a User*: insert_user stores the pointer, and
    // delete_user frees its own node but never the User behind it. So main() owns every
    // User it allocates, and is responsible for deleting them again.
    vector<User*> users;

    string menu = "1. Add a contact\n2. Retrieve a contact\n3. Remove a contact\n4. Search by prefix\n5. Show the number of contacts\n6. See the menu again\n7. Quit";
    int command = 6;
    while (command != 7) {
        if (command == 1) {
            cout << "Enter the nickname:" << endl;
            string nickname;
            cin >> nickname;

            if (!valid_nickname(nickname)) {
                cout << "Nicknames must use the lowercase letters a-z only" << endl;
            } else if (book.retrieve_user(nickname) != nullptr) {
                // Checked here so that insert_user is only ever called on a nickname
                // we know is free, and never has to throw.
                cout << "The nickname '" << nickname << "' is already taken" << endl;
            } else {
                cout << "Enter the display name:" << endl;
                string display_name;
                cin >> display_name;

                cout << "Enter the user score:" << endl;
                int user_score;
                cin >> user_score;

                User* user = new User;
                user->nickname = nickname;
                user->display_name = display_name;
                user->user_score = user_score;

                book.insert_user(nickname, user);
                users.push_back(user); // main() keeps hold of it so it can free it later
                cout << "Added " << display_name << " under the nickname '" << nickname << "'" << endl;
            }
        } else if (command == 2) {
            cout << "Enter the nickname to retrieve:" << endl;
            string nickname;
            cin >> nickname;

            if (!valid_nickname(nickname)) {
                cout << "Nicknames must use the lowercase letters a-z only" << endl;
            } else {
                User* user = book.retrieve_user(nickname);
                if (user == nullptr) {
                    cout << "No contact is saved under '" << nickname << "'" << endl;
                } else {
                    cout << "Nickname: " << user->nickname
                         << " | Display name: " << user->display_name
                         << " | Score: " << user->user_score << endl;
                }
            }
        } else if (command == 3) {
            cout << "Enter the nickname to remove:" << endl;
            string nickname;
            cin >> nickname;

            User* user = valid_nickname(nickname) ? book.retrieve_user(nickname) : nullptr;
            if (user == nullptr) {
                // Checked here so that delete_user is only ever called on a nickname
                // we know is present, and never has to throw.
                cout << "No contact is saved under '" << nickname << "'" << endl;
            } else {
                book.delete_user(nickname);

                // The book has dropped its node, so now main() frees the User itself.
                for (int i = 0; i < (int)users.size(); i++) {
                    if (users[i] == user) {
                        users.erase(users.begin() + i);
                        break;
                    }
                }
                delete user;
                cout << "Removed the contact '" << nickname << "'" << endl;
            }
        } else if (command == 4) {
            cout << "Enter the prefix to search for:" << endl;
            string prefix;
            cin >> prefix;

            if (!valid_nickname(prefix)) {
                cout << "Prefixes must use the lowercase letters a-z only" << endl;
            } else if (book.search(prefix)) {
                cout << "Yes - at least one contact has the prefix '" << prefix << "'" << endl;
            } else {
                cout << "No contact has the prefix '" << prefix << "'" << endl;
            }
        } else if (command == 5) {
            cout << "The contact book holds " << book.size << " contact(s)" << endl;
        } else if (command == 6) {
            cout << menu << endl;
        }

        cout << "Enter command (1-7):" << endl;
        if (!(cin >> command)) {
            // End of input, or something that isn't a number - stop rather than loop forever.
            break;
        }
    }

    // Everything main() allocated, main() cleans up.
    for (int i = 0; i < (int)users.size(); i++) {
        delete users[i];
    }
    users.clear();

    return 0;
}