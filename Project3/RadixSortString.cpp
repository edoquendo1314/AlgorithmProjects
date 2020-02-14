#include <iostream>
#include <fstream>
#include <string> 
#include <cstdlib>
#include <string.h>


using namespace std; 

//global variables
ifstream in_File;
ofstream outFile1;
ofstream outFile2;

class ListNode
{

private:
    friend class LLStack;
    friend class LLQ;
    friend class RadixSort;
public:
    string data;
    ListNode* next;
    
    //Overloaded Constructors
    ListNode();
    ListNode(string data);
    ListNode(string data, ListNode* next);
    
    //Copy Constructor
    ListNode(const ListNode &source);
};

ListNode :: ListNode(string data, ListNode* next):data{data}, next{next}
{
}

ListNode :: ListNode(string data):ListNode{data, NULL}
{
}

ListNode::ListNode():ListNode {" ", NULL}
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

    LLStack(const LLStack &source);
    
    void push(ListNode* newNode);
    ListNode pop(LLStack *S);
    bool isEmpty();
    void printStack(LLStack S);
};

LLStack :: LLStack(const LLStack &source):top{source.top}
{
}

void LLStack :: push(ListNode *newNode)
{
    ListNode *temp = new ListNode();
    temp->data = newNode->data;
    temp->next = top;
    top = temp; 
}

ListNode LLStack :: pop(LLStack *S)
{   
    ListNode* temp = new ListNode;
    if(isEmpty() == true)
    {
        cerr<<"error"<<endl; 
    }
    else
    {
        temp = top;
        top = top->next;
        temp->next = NULL;
    }
    return *temp; 
}


bool LLStack :: isEmpty()
{
    if(this->top == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void LLStack :: printStack(LLStack S)
{
    ListNode* temp = top;

    outFile1 <<"Top -> ("<< temp->data << ",";
    temp = temp->next; 

    while(temp != NULL)
    {
        outFile1<< temp->data << ")->(" << temp->data << ",";
        temp = temp->next;
    }

    outFile1<<"NULL)->NULL"<<endl;

}

class LLQ
{
private:
    friend class RadixSort;
public:
    ListNode* head = new ListNode;
    ListNode* tail = new ListNode;
    ListNode* dummy = new ListNode;
    LLQ()
    {
        dummy->data = "9999"; 
        head = dummy;
        tail = dummy;
    }
    
    void addTail(LLQ Q, ListNode* newNode);
    ListNode deleteHead(LLQ Q);
    bool isEmpty();

    void printQueue(LLQ Q, int index, int currentTable);
};

void LLQ :: addTail(LLQ Q, ListNode* newNode)
{
    this->tail->next = newNode;
    this->tail = newNode;
} 

ListNode LLQ :: deleteHead(LLQ Q)
{
    
    ListNode* temp = new ListNode; 
    temp = head; 
    if(isEmpty() == true)
    {
        cerr<<"LLQ is empty "<<endl; 
    }
    else
    {
        temp = Q.head->next;
        Q.head->next = Q.head->next->next;
        temp->next = NULL;

        if(Q.tail == temp)
        {
            Q.tail = Q.head;
            return *temp; 
        }
    }
}

bool LLQ :: isEmpty()
{
    if(this->head->next == NULL)
    {
        return true;
    }
    else
    {
        return false; 
    }
}


void LLQ :: printQueue(LLQ Q, int index, int currentTable)
{


    ListNode* front = this->head;
    ListNode* temp = this->tail;

    outFile2 << "Table[" << currentTable << "][" << index << "]:";
    outFile2<< "(" << front->data <<",";  
    front = front->next;
    while(front != NULL)
    {
        outFile2 << front->data << ")->(" << front->data << ","; 
        front = front->next;
    }
    outFile2 <<")->(" << temp->data << "," << "NULL)->NULL"<< endl;

}

class RadixSort
{
public:
    static const int tableSize = 256;
    LLStack *new_Stack_Object;
    LLStack *object = new LLStack;
    LLStack *ptr;
    LLQ size_Object;
    LLQ hashTable[2][tableSize];
    string data;
    int currentTable = 0;
    int previousTable = 0;
    int stackCounter = 0;
    int longestStringLength = 0;
    int currentPosition = 0;

    //Methods
    void firstReading();
    void loadStack(ListNode* Top);
    void dumpStack(LLStack *top, int currentDigit, int currentTable);
    int tableIndex(int index);//which index of the given hash table
    char getChar(ListNode *newNode, int currentDigit);
    int hashIndex(char digit);
    void radix_Sort(LLStack* Top);
    string padString(string pad);

    RadixSort()
    {
        hashTable[2][tableSize];
    }

    void printTable(int currentTable);//call printQueue to print only those non-empty 
    //Queues in the table

};

void RadixSort :: firstReading()
{
    int LongestNum {0};

   while(in_File >> data)
   {
        if(data.length() > LongestNum)
        {
            LongestNum = data.length();
        } 
   }

    this->longestStringLength = LongestNum;
    this->currentPosition = longestStringLength - 1;
}

int RadixSort :: hashIndex(char digit)
{
    return (int)digit % this->tableSize; 
}

void RadixSort :: loadStack(ListNode *Top)
{
    Top;
    string read_In_Values;
    LLStack print; 

    ListNode *temp;
    while(in_File >> read_In_Values)
    {
        temp = new ListNode(padString(read_In_Values));

        Top = temp; 

        object->push(Top);
        this->stackCounter++;
    }

    object->printStack(print); 
}

string RadixSort :: padString(string pad)
{
    for(int i = pad.length(); i <= longestStringLength - 1; i++)
    {
        pad =  " " + pad;
    }

    return pad; 
}

void RadixSort :: radix_Sort(LLStack *top)
{

    LLQ node_Call;
    ListNode *newNode;

    dumpStack(top, currentPosition, currentTable);
    this->currentPosition--;
    currentTable = 1;
    previousTable = 0;
    int currentQueue = 0;
    int temp = 0;
    char digit;
    int hash_Index;

    do{
        for(currentQueue = 0; currentQueue < tableSize - 1; currentQueue++)
        {
            while(this->hashTable[previousTable][currentQueue].head->next == this->hashTable[previousTable][currentQueue].tail)
            {
                newNode = this->hashTable[previousTable][currentQueue].head->next;
                digit = getChar(newNode, currentPosition);
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
    
        this->currentPosition--;
        
    }while(this->currentPosition >= 0);


    printTable(currentTable);

}

void RadixSort :: printTable(int currentTable)
{ 
   for(int index = 0; index < tableSize; index++)
   {
       if(this->hashTable[currentTable][index].isEmpty())
       {

       }
       else
       {
           hashTable[currentTable][index].printQueue(hashTable[currentTable][index], index, currentTable);
       }
       
   }

}

int RadixSort :: tableIndex(int digit)
{
    int index = digit % tableSize; 
    return index;
}


char RadixSort :: getChar(ListNode *node, int currentPosition)
{
    string char_Value = node->data;
    return char_Value[currentPosition];
}


void RadixSort :: dumpStack(LLStack *newNode, int currentPosition, int currentTable)
{   
   
    while(this->stackCounter != 0)
    {   
        ListNode *node = new ListNode;
        *node = object->pop(new_Stack_Object); 
        char digit = getChar(node, currentPosition);
        int value = hashIndex(digit);
        this->hashTable[currentTable][value].addTail(this->hashTable[currentTable][value], node); 
        cout<<this->hashTable[currentTable][value].tail->data<<endl; 
        this->stackCounter--;
        delete node; 
    }

    printTable(this->currentTable);
}

int main(int argc, char *argv[])
{   
    RadixSort firstRead;
    ListNode *Top;
    LLStack *top;

    in_File.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    if(in_File.is_open())
    {
        firstRead.firstReading();
    }
    else
    {
        cerr<<"Could not open file" <<endl;
        return 1;
    }
    
    in_File.close(); 

    in_File.open(argv[1]);

    firstRead.loadStack(Top);

    outFile1.close();
    in_File.close();
    

    firstRead.radix_Sort(top);

    outFile2.close();
}
