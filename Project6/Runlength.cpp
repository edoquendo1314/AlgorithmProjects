#include <iostream>
#include <string>
#include <fstream>

using namespace std;

fstream inFile1;
fstream outFile1; 
fstream encodeFile;
fstream decodeFile;

class runLength
{
    public:
        int numRows {0};
        int numCols {0};
        int minVal {0};
        int maxVal {0};

        //default constructor
        runLength()
        {}

        runLength(int numRows, int numCols, int minVal, int maxVal)
        :numRows{numRows}, numCols{numCols}, minVal{minVal}, maxVal{maxVal}
        {}

        void runLengthEncode(int* arr, int** twoArr, int value, int** dummyEncode)
        {
            cout<<"RUNLENGTHENCODE YOUR VALUE: " <<value <<endl; 

            switch(value)
            {
                case 1: 
                    encodeMethod1(twoArr, value);
                    break;
                case 2:
                    encodeMethod2(twoArr, value, dummyEncode);
                    break;
                case 3:
                    encodeMethod3(arr, value);
                    break;
                case 4:
                    encodeMethod4(arr, value, dummyEncode);
                    break;
                default:
                    cout <<"Not one of the choices check main this shouldn't have gotten through " <<endl; 
                    break; 
            }
        }

        void output(int run, int whichMethod, int total_Bytes)
        {
            outFile1 << "Method_" << whichMethod << " was used." << " Number of Runs: " << run << " Total bytes: " << total_Bytes <<endl; 
        }

    
        void encode(int* arr, int position, int startRow, int startCol)
        {
            int n = arr[position]; 

            encodeFile << startRow << " " << startCol << " " << position << " " << n <<endl; 
        }

        void encodeZero(int* arr, int position, int startRow, int startCol, int dummy, int** dummyEncode)
        {
            
            int n = arr[position]; 

            fillDummy(arr, position, startRow, startCol, dummy, dummyEncode);
        }
        
        void reset(int* c)
        {
             for(int i = 0; i < maxVal + 1; i++)
            {
                c[i] = 0;
            }
        }

        void encodeMethod1(int** twoArr, int whichMethod)
        {
            int total_Bytes {0};
            int* c = new int[maxVal + 1];

            for(int i = 0; i < maxVal + 1; i++)
            {
                c[i] = 0;
            }

            int run {0};
            int position {0}; //color
            int startRow {0};
            int startCol {0};
            int pixel_Length {0}; 

            for(int i = 0; i < this->numRows; i++){
                int j = 0;
                startCol = j;
                startRow = i;  
                for(j; j < this->numCols; j++){
                    c[twoArr[i][j]]++; 
                    if(twoArr[i][j] != twoArr[i][j+1])
                    {
                        run++;
                        position = twoArr[i][j];  
                        encode(c, position, startRow, startCol);
                        reset(c);
                        startCol = j+1;
                    }
                    position = twoArr[i][j];
                }
                startCol = j;
                if(c[twoArr[i][numCols - 1]] > 0)
                {
                    startRow = i; 
                    encode(c, position, startRow, startCol); 
                }
                reset(c);
                
             }
    
            total_Bytes = run * (4 * 10); 
            output(run, whichMethod, total_Bytes);
        }

        void fillDummy(int* c, int position, int startRow, int startCol, int dummy, int** dummyEncode)
        {
            int n = c[position];
            dummyEncode[dummy][0] = startRow;
            dummyEncode[dummy][1] = startCol;
            dummyEncode[dummy][2] = position;
            dummyEncode[dummy][3] = n;
            dummy++; 
         
        }

        void encodeMethod2(int** twoArr, int whichMethod, int** dummyEncode)
        {
            int total_Bytes {0};
            int* c = new int[maxVal + 1];
            int dummy {0};
            
            for(int i = 0; i < maxVal + 1; i++)
            {
                c[i] = 0;
            }
            int run {0};
            int position {0};
            int startRow {0};
            int startCol {0};
            
            int startColl {0};
            int startRoww {0};
            int pos {0};

            for(int i = 0; i < this->numRows; i++){
                int j = 0;
                startCol = j;
                startRow = i;
                for( j; j < this->numCols; j++){
                    c[twoArr[i][j]]++;
                    if(c[0] == 1)
                    {
                        pos = twoArr[i][j];
                        startRoww = i; 
                        startColl = j;
                    }
                    position = twoArr[i][j];
                    if(twoArr[i][j] != twoArr[i][j+1] && twoArr[i][j] != 0)
                    {
                        if(c[0] > 0)
                        {
                         
                            fillDummy(c, pos, startRoww, startColl, dummy, dummyEncode);
                            dummy++;
                        }
                        
                        c[0] = 0;
                        run++;
                        position = twoArr[i][j];
                        encode(c, position, startRow, startCol); 
                        encodeZero(c, position, startRow, startCol, dummy, dummyEncode);
                        dummy++;
                        reset(c);
                        startCol = j + 1; 
                    }
                    position = twoArr[i][j];
                }
                if(c[twoArr[i][numCols - 1]] > 0)
                {
                    encode(c, position, startRow, startCol);
                    encodeZero(c, position, startRow, startCol, dummy, dummyEncode);
                    dummy++;
                }
                reset(c);
             }


            total_Bytes = run * (4 * 10); 
            output(run, whichMethod, total_Bytes);
        }

        void encodeMethod3(int* arr, int whichMethod)
        {
            int total_Bytes {0};
            int* c = new int[maxVal + 1];

            for(int i = 0; i < maxVal + 1; i++)
            {
                c[i] = 0;
            }
            int run {0};
            int position {0};
            int startRow {0};
            int startCol {0};
            int colCnt {0};

            for(int i = 0; i < (this->numRows * this->numCols); i++){
                c[arr[i]]++;
                colCnt++; 
                if(colCnt == 20)
                {
                    colCnt = 0;
                    startRow++; 
                }
                if(arr[i] != arr[i+1])
                {
                    run++;
                    position = arr[i];  
                    encode(c, position, startRow, startCol);
                    reset(c);
                    startCol = i;
                }
                position = arr[i];
                
                if(i == ((this->numRows * this->numCols) - 1))
                { 
                    encode(c, position, startRow, startCol); 
                }
            }  

             total_Bytes = run * (4 * 10); 
             output(run, whichMethod, total_Bytes);
        }

        void encodeMethod4(int* arr, int whichMethod, int** dummyEncode)
        {
            int total_Bytes {0};
            int* c = new int[maxVal + 1];
            int dummy {0};

            for(int i = 0; i < maxVal + 1; i++)
            {
                c[i] = 0;
            }

            int run {0};
            int position {0};
            int startRow {0};
            int startCol {0}; 
            int colCnt {0};
            int startColl {0};
            int startRoww {0};
            int pos {0};

            for(int i = 0; i < (this->numRows * this->numCols); i++){
                colCnt++;
                if(colCnt == 20)
                {
                    colCnt = 0;
                    startRow++;
                }
                c[arr[i]]++; 
                if(c[0] == 1)
                {
                    pos = arr[i];
                    startRoww = i; 
                    startColl = colCnt;
                }
                if(arr[i] != arr[i+1] && arr[i] != 0)
                {
                    if(c[0] > 0)
                    {
                        fillDummy(c, pos, startRoww, startColl, dummy, dummyEncode);
                        dummy++;
                    }
                    c[0] = 0;
                    run++;
                    position = arr[i];  
                    encode(c, position, startRow, startCol);
                    encodeZero(c, position, startRoww, startColl, dummy, dummyEncode);
                    dummy++; 
                    reset(c);
                    startCol = i;
                }
            }   

            total_Bytes = run * (4 * 10); 
            output(run, whichMethod, total_Bytes);
        }
        
        void runLengthDecode(int value, int** dummyEncode)
        {
            decodeFile << this->numRows << " " << this->numCols << " " << this->minVal << " " << this->maxVal <<endl; 

             switch(value)
            {
                case 1: 
                    decodeMethod1();
                    break;
                case 2:
                    decodeMethod2(dummyEncode);
                    break;
                case 3:
                    decodeMethod3(dummyEncode);
                    break;
                case 4:
                    decodeMethod4(dummyEncode);
                    break; 
                default:
                    cout <<"Not one of the choices check main this shouldn't have gotten through " <<endl; 
                    break; 
            }
        }
        
        void decodeZero(int startRow, int startCol, int color, int pixel_Len, int colCnt, int** dummyEncode)
        {
            cout << "WHAT IS COLCNT: " << colCnt <<endl;
            for(int i = 0; i < pixel_Len; i++)
            {
                decodeFile << color << " ";
            }

            return;
            
        }

        void decode(int startRow, int startCol, int color, int pixel_Len, int colCnt)
        {

            for(int i = 0; i < pixel_Len; i++)
            {
                decodeFile << color << " "; 
            }
            if(colCnt == 20)decodeFile <<endl; 
            return;
        }
        void decodeMethod1()
        {
            int startRow;
            int startCol;;
            int color;
            int pixel_Len;
            int colCnt {0};

            
           while(encodeFile >> startRow >> startCol >> color >> pixel_Len)
           {
                colCnt += pixel_Len; 
                decode(startRow, startCol, color, pixel_Len, colCnt); 
                if(colCnt == 20)colCnt = 0;
           }

        }

        void decodeMethod2(int** dummyEncode)
        {
            int startRow;
            int startCol;;
            int color;
            int pixel_Len;
            int colCnt {0};

            
           while(encodeFile >> startRow >> startCol >> color >> pixel_Len)
           {
               colCnt += pixel_Len; 
                decodeZero(startRow, startCol, color, pixel_Len, colCnt, dummyEncode); 
                if(colCnt == 20)colCnt = 0;
           }
        }

        void decodeMethod3(int** dummyEncode)
        {
            int startRow;
            int startCol;;
            int color;
            int pixel_Len;
            int colCnt {0};

            
           while(encodeFile >> startRow >> startCol >> color >> pixel_Len)
           {
               colCnt += pixel_Len; 
                decodeZero(startRow, startCol, color, pixel_Len, colCnt, dummyEncode); 
                if(colCnt == 20)colCnt = 0;
           }
        }

        void decodeMethod4(int** dummyEncode)
        {
            int startRow;
            int startCol;;
            int color;
            int pixel_Len;
            int colCnt {0};

            
           while(encodeFile >> startRow >> startCol >> color >> pixel_Len)
           {
                colCnt += pixel_Len; 
                decodeZero(startRow, startCol, color, pixel_Len, colCnt, dummyEncode); 
                if(colCnt == 20)colCnt = 0;
           }
        }
    
};

int main(int argc, char* argv[])
{
    inFile1.open(argv[1]);
    outFile1.open(argv[2]);

    if(!inFile1.is_open())
    {
        cerr << "error in opening file" <<endl;
        return 0; 
    }
    
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    inFile1 >> numRows;
    inFile1 >> numCols;
    inFile1 >> minVal;
    inFile1 >> maxVal; 
  
    cout<< numRows << ", "  << numCols << ", " << minVal << ", " << maxVal <<endl; 

    int a = {0};
    int *arr = new int[numRows * numCols];
    int **twoArr = new int*[numRows];
    int **dummyEncode = new int*[63];

    for(int k = 0; k < 63; k++){
        dummyEncode[k] = new int[4]; 
    }

    for(int i = 0; i < 63 ; i++){
        for(int j = 0; j < 4; j++)
        {
            dummyEncode[i][j] = 0;
        }
    }


    for(int i = 0; i < numRows; i++){
        twoArr[i] = new int[numCols];
    }

    
    for(int i = 0; i < (numRows * numCols); i++){
        inFile1 >> a;
        arr[i] = a;
    }

    int x {0};

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            twoArr[i][j] = arr[x]; 
            x++; 
        }
    }
    int value {0}; 
    char yesNo {' '};
    string filename {""};
    string decodefn {""};
    runLength obj(numRows, numCols, minVal, maxVal);

    do
    {
        cout << "Would you like to encode a file? " <<endl; 
        cin >> yesNo;
        cout << "Your choice to encode a file was: " << yesNo <<endl; 

        if(yesNo == 'N')
        {
            break; 
        }
        else if(yesNo == 'Y')
        {
            do{
                cout<< "Which method would you like to choose? click 1,2,3,4 to the corresponding method " <<endl;
                cout << "Method 1: " <<endl;
                cout << "Method 2: " <<endl;
                cout << "Method 3: " <<endl;
                cout << "Method 4: " <<endl; 
                cin >> value; 
                if(value == 1 || value == 2 || value == 3 || value == 4)
                {
                    cout << "You chose " << value <<endl; 
                    break;
                }
                else
                {
                    cout << "That isn't one of your choices try again." << endl;
                }
                
            }while(value != 1 || value != 2 || value != 3 || value != 4);
            
            filename = "EncodeMethod_" + to_string(value) + ".txt";
            encodeFile.open(filename, ios::out); 
            if(encodeFile.fail())
            {
                cerr << "Error Opening File" <<endl;
                exit(1); 
            }
            obj.runLengthEncode(arr, twoArr, value, dummyEncode);
            for(int i = 0; i < 63; i++){
                for(int j = 0; j < 4; j++)
                {
                    cout << dummyEncode[i][j] << ", "; 
                }
                cout<<endl; 
            }
            encodeFile.close();
            encodeFile.clear();

            encodeFile.open(filename, ios::in); 

             if(!encodeFile.is_open())
            {
                cerr << "error in opening file" <<endl;
                return 0; 
            }

            decodefn = "Decoded_" + filename; 
            decodeFile.open(decodefn, ios::out);
            if(decodeFile.fail())
            {
                cerr << "Error opening file" <<endl;
                exit(1);
            }
            
            obj.runLengthDecode(value, dummyEncode);

            encodeFile.close();
            decodeFile.close(); 
            
        }
        
    } while (yesNo != 'N');
    


    inFile1.close();
    outFile1.close();
}