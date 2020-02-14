#include <iostream>
#include <fstream>
#include <string> 
#include <cstdlib>

using namespace std; 

class ListNode
{

private:
    friend class LLStack;
    friend class LLQ;
    friend class RadixSort;
public:
    int data;
    ListNode* next;
    
    //Overloaded Constructors
    ListNode();
    ListNode(int data);
    ListNode(int data, ListNode* next);
    
    //Copy Constructor
    ListNode(const ListNode &source);
};

ListNode :: ListNode(int data, ListNode* next):data{data}, next{next}
{
}

ListNode :: ListNode(int data):ListNode{data, NULL}
{
}

ListNode::ListNode():ListNode {0, NULL}
{
}

//Implementing the Copy Constructor

ListNode :: ListNode(const ListNode &source):data{source.data}, next{source.next}
{  
}

class LLStack
{
    friend class RadixSort;
public:
    ListNode* top = new ListNode;
    LLStack()
    {
        top->next = NULL; 
    }
    
    void push(ListNode* newNode);
    ListNode pop(LLStack *S);
    bool isEmpty();
    void printStack();
};

void LLStack :: push(ListNode *newNode)
{
    ListNode *temp = new ListNode();
    temp->data = newNode->data;
    temp->next = top;
    top = temp; 
}

ListNode LLStack :: pop(LLStack *S)
{

    ListNode* temp = new ListNode();
    temp->data = top->data; 
    if(top != NULL)
    {
        top = top->next;
    }
    else
    {
        cout<<"Stack is empty"<<endl; 
    }
    cout<<top->data<<endl;
    return temp->data;
}


bool LLStack :: isEmpty()
{
    if(top == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void LLStack :: printStack()
{
    ListNode* temp = top;
    cout<<"Top -> ("<< temp->data << ",";
    temp = temp->next; 

    while(temp != NULL)
    {
        cout<< temp->data << ")->(" << temp->data << ",";
        temp = temp->next;
    }

    cout<<"NULL)->NULL"<<endl;

}

class LLQ
{
private:
    friend class RadixSort;
public:
    int LL_size = 0;
    ListNode* head = new ListNode;
    ListNode* tail = new ListNode;
    ListNode* dummy = new ListNode;
    LLQ()
    {
        dummy->data = 9999; 
        head = dummy;
        tail = dummy;
    }
    
    void addTail(LLQ Q, ListNode* newNode);
    ListNode deleteHead(LLQ Q);
    bool isEmpty();
    void printQueue(LLQ Q, int index);
    void printQueue(LLQ Q, int index, ofstream &outFile);
    int size();
};

void LLQ :: addTail(LLQ Q, ListNode* newNode)
{
    ListNode* temp = new ListNode();
    temp->data = newNode->data;
    tail->next = temp;
    tail = temp;
    LL_size++;
} 

ListNode LLQ :: deleteHead(LLQ Q)
{
    
    ListNode* temp = new ListNode; 
    temp = head; 
    if(head == dummy)
    {
        return *dummy; 
    }
    else
    {
        head->next = head->next->next;
        return *temp;
        delete temp; 
    }
}

bool LLQ :: isEmpty()
{
    if(head->next == NULL)
    {
        return true;
    }
    else
    {
        return false; 
    }
}

int LLQ :: size()
{
    return LL_size;
}

void LLQ :: printQueue(LLQ Q, int index)
{
    cout<<"****Below is the output of QUEUE***"<<endl;

    ListNode* front = head;
    ListNode* temp = tail;

    cout<< "Front(" << index << ")->(" << front->data << ","; 
    front = front->next;
    while(size() != 0)
    {
        cout<< front->data << ")->(" << front->data << ","; 
        front = front->next;
        LL_size--; 
    }
    cout<<"NULL)->NULL"<<endl;
    cout<< "Tail(" << index << ")->(" << temp->data << "," << "NULL)->NULL"<< endl;
}

void LLQ :: printQueue(LLQ Q, int index, ofstream &outFile)
{
    cout<<"****Below is the output of QUEUE***"<<endl;

    ListNode* front = head;
    ListNode* temp = tail;
    while(outFile.is_open()) {
        outFile << "Front(" << index << ")->(" << front->data << ","; 
        front = front->next;
        while(size() != 0)
        {
            outFile << front->data << ")->(" << front->data << ","; 
            front = front->next;
            LL_size--; 
        }
        outFile<<"NULL)->NULL"<<endl;
        outFile<< "Tail(" << index << ")->(" << temp->data << "," << "NULL)->NULL"<< endl;
    }
}

class RadixSort
{
public:
    static const int tableSize = 10;
    LLStack *new_Stack_Object, object;
    LLStack *ptr;
    LLQ size_Object;
    LLQ hashTable[2][tableSize];
    int data {0};
    int currentTable {0};
    int previousTable {0};
    int maxDigits {0};
    int offSet {0};
    int currentDigit {0};
    int stackCounter = 0;

    //Methods
    void firstReading(std::ifstream &inFile);
    void loadStack(ifstream &inFile, ListNode* Top);
    void dumpStack(LLStack *top, int currentDigit, int currentTable);
    int tableIndex(int index);//which index of the given hash table
    int getMaxDigits(int totalDigits);
    int getDigit(ListNode *newNode, int currentDigit);
    int hashIndex(int digit);
    void radix_Sort(LLStack* Top);
    void radix_Sort(LLStack* Top, ofstream& outFile);

    RadixSort()
    {
        LLQ hashTable[2][tableSize];
    }

    void printTable(int currentTable);//call printQueue to print only those non-empty 
    //Queues in the table
    void printTable(int currentTable, ofstream& outFile);

};

void RadixSort :: firstReading(std::ifstream &inFile)
{
    int negativeNum {0};
    int positiveNum {0};

   while(inFile >> data)
   {
        if(data < negativeNum)//negativeNum is set to 0
        {
            negativeNum = data; //find the biggest negative number
        }
        else if(data > positiveNum)//positiveNum is set to 0
        {
            positiveNum = data; 
        }
   }

   this->offSet = abs(negativeNum);
   positiveNum = positiveNum + offSet;
   this->maxDigits = getMaxDigits(positiveNum);
}

int RadixSort :: hashIndex(int digit)
{
    return digit % this->tableSize; 
}

void RadixSort :: loadStack(ifstream &inFile, ListNode *Top)
{

    Top = NULL;
    int read_In_Values {0};

    ListNode *temp;
    while(inFile >> read_In_Values)
    {
        read_In_Values += offSet;

        temp = new ListNode(read_In_Values);

        Top = temp; 

        cout<<Top->data<<endl;

        object.push(Top);
        stackCounter++;
    }

    cout<<"***Below is the output of the stack***"<<endl;

    object.printStack();
}

void RadixSort :: radix_Sort(LLStack *top)
{

    LLQ node_Call;
    ListNode *newNode;

    dumpStack(top, currentDigit, currentTable);
    currentDigit++;
    currentTable = 1;
    previousTable =0;
    int currentQueue = 0;
    int temp = 0;
    int digit;
    int hash_Index;

    do{
        for(currentQueue = 0; currentQueue <= tableSize - 1; currentQueue++)
        {
            while(this->hashTable[previousTable][currentQueue].head->next != NULL)
            {
                newNode = this->hashTable[previousTable][currentQueue].head->next;
                digit = getDigit(newNode, currentDigit);
                node_Call.deleteHead(this->hashTable[previousTable][currentQueue]);

                hash_Index = hashIndex(digit);

                node_Call.addTail(this->hashTable[currentTable][hash_Index], newNode);
            }
        }

        printTable(currentTable);

        if(currentTable == 0)
        {
            temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
        }
        else
        {
            temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            currentTable = 0;
        }
    
        currentDigit++;

    }while(currentDigit < maxDigits);


    printTable(currentTable);

}
//overloaded radix_Sort to output info to file
void RadixSort :: radix_Sort(LLStack *top, ofstream& outFile)
{

    LLQ node_Call;
    ListNode *newNode;

    dumpStack(top, currentDigit, currentTable);
    currentDigit++;
    currentTable = 1;
    previousTable =0;
    int currentQueue = 0;
    int temp = 0;
    int digit;
    int hash_Index;

    do{
        for(currentQueue = 0; currentQueue <= tableSize - 1; currentQueue++)
        {
            while(this->hashTable[previousTable][currentQueue].head->next != NULL)
            {
                newNode = this->hashTable[previousTable][currentQueue].head->next;
                digit = getDigit(newNode, currentDigit);
                node_Call.deleteHead(this->hashTable[previousTable][currentQueue]);

                hash_Index = hashIndex(digit);

                node_Call.addTail(this->hashTable[currentTable][hash_Index], newNode);
            }
        }

        printTable(currentTable);

        if(currentTable == 0)
        {
            temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
        }
        else
        {
            temp = currentTable;
            currentTable = previousTable;
            previousTable = temp;
            currentTable = 0;
        }
    
        currentDigit++;

    }while(currentDigit < maxDigits);


    printTable(currentTable, outFile);

}

void RadixSort :: printTable(int currentTable)
{
    ofstream outFile; 

   for(int index = 0; index < tableSize; index++)
   {
    hashTable[currentTable][index].printQueue(hashTable[currentTable][index], index);
   }

}

//overloaded print function for output file
void RadixSort :: printTable(int currentTable, ofstream& outFile)
{
   for(int index = 0; index < tableSize; index++)
   {
    hashTable[currentTable][index].printQueue(hashTable[currentTable][index], index, outFile);
   }
}

int RadixSort :: tableIndex(int digit)
{
    int index = digit % tableSize; 
    return index;
}

int RadixSort :: getMaxDigits(int totalDigits)
{
    int length_of_MaxDigit = 0;

    for(length_of_MaxDigit; totalDigits != 0; length_of_MaxDigit++)
    {
        totalDigits /= 10;
    } 

    return length_of_MaxDigit; 
}

int RadixSort :: getDigit(ListNode *node, int currentDigit)
{
    int digit_Value = node->data;
    for(int i = 0; i < currentDigit; i++)
    {
        digit_Value /= 10;
    }

    int temp = digit_Value % 10;
    cout << temp << endl;
    return temp;
}

void RadixSort :: dumpStack(LLStack *newNode, int currentDigit, int currentTable)
{
    ListNode *node;
    

    while(this->stackCounter != 0)
    {   
        *node = object.pop(new_Stack_Object); 
        cout<< node->data<<endl;
        int digit = getDigit(node, currentDigit);
        int value = hashIndex(digit);
        this->hashTable[currentTable][value].addTail(this->hashTable[currentTable][value], node); 
        stackCounter--;
    }

    printTable(this->currentTable);
}

int main(int argc, char *argv[])
{   
    RadixSort firstRead, lastRead;
    ListNode *Top;
    LLStack *top;

    //output files 
    ofstream outFile1;
    ofstream outFile2;
    //input files
    ifstream in_file1;

    in_file1.open(argv[1]);
    
    if(in_file1.is_open())
    {
        firstRead.firstReading(in_file1);
    }
    else
    {
        cerr<<"Could not open file" <<endl;
        return 1;
    }
    
    in_file1.close(); 

    in_file1.open(argv[1]);

    firstRead.loadStack(in_file1, Top);

    in_file1.close();

    outFile1.open(argv[2]);

    firstRead.radix_Sort(top);

    lastRead.radix_Sort(top, outFile1);

    outFile1.close(); 
}
