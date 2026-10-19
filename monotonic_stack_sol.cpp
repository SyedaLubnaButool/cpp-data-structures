#include <iostream>

using namespace std;

template <typename T>
struct Stack{
    T* data;
    int top;
    int capacity;

    Stack(int size)
    {
        data = new T[size];
        top=-1;
        capacity=size;
    }

    ~Stack()
    {
        delete[] data;
    }

    void push(T value)
    {
        top++;
        data[top]=value;
    }

    void pop()
    {
        top--;
    }

    T peek()
    {
        return data[top];
    }

    bool empty()
    {
        return top==-1;
    }
};

int* solve(int n, int gates[], int keycards[]) {
    // Solve the reachability problem described in the spec.
    // n is the number of gates/keycards.
    // gates is the clearance level of each gate, read from top to bottom
    // keycards is the clearance level of each keycard, read from top to bottom
    // You should return an n length array where the ith index tells us how many gates could be opened from the ith room (top to bottom).

    // In the example in the spec, the input would be:
    // n: 8
    // gates: [5, 2, 3, 2, 2, 3, 1, 2]
    // keycards: [1, 4, 1, 1, 3, 2, 0, 2]
    // And your function should return:
    // [0, 1, 0, 0, 4, 0, 0, 2]

    int* result =new int[n];//this stores our results for each gate using keycard
    Stack<int> stack(n);
    
    for(int i=0;i<n;i++)//for every keycard
    {
        //remove gates that are no longer useful blockers
        while( !stack.empty() && gates[i]>=gates[stack.peek()])
        {
            stack.pop();//this maintains the monotonicality of stack 
        }

        //add current gate as possible blocker
        stack.push(i);

        //remove blocking gate current keycard can open
        while(!stack.empty()&& keycards[i]>= gates[stack.peek()])
        {
            stack.pop();
        }

        //calculating result for current keycard
        if(!stack.empty())
        {
            result[i]=i-stack.peek();//if elements in stack, whats the latest gate index it got blocked with, subtract that from current index
        }
        else
        {
            result[i]=i+1;//if no elements in stack it can open current gate+all previous giving i+1
        }
        
        
    }
    return result;
}

int ontrack_test() {
    int n;
    int* gates;
    int* keycards;
    cout << "Enter n: " << endl;
    cin >> n;
    gates = new int[n];
    keycards = new int[n];
    cout << "Enter gates: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> gates[i];
    }
    cout << "Enter keycards: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> keycards[i];
    }
    if (!cin) {
        cout << "ERROR: could not read input data" << endl;
        delete[] gates;
        delete[] keycards;
        return 1;
    }
    
    int* result = solve(n, gates, keycards);
    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    delete[] gates;
    delete[] keycards;
    delete[] result;
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 1) {
        return ontrack_test();
    } 
    int n = 8;
    int* gates = new int[] {5, 2, 3, 2, 2, 3, 1, 2};
    int* keycards =  new int[] {1, 4, 1, 1, 3, 2, 0, 2};
    int* result = solve(n, gates, keycards);
    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    delete[] gates;
    delete[] keycards;
    delete[] result;
    return 0;
}
