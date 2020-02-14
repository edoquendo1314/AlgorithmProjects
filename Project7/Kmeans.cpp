#include <iostream>
#include <fstream>
#include <string> 
#include <math.h>
#include <cmath>


using namespace std;

fstream inFile;
fstream outFile;
fstream outFile2;
fstream outFile3;
class Point
{
public:
    int Xcoord {0};
    int Ycoord {0};
    int Label {0};
    double distance {0.0};
    
    //default
    Point()
    {}

    Point(int Xcoord, int Ycoord, int Label, double distance)
    {
        this->Xcoord = Xcoord;
        this->Ycoord = Ycoord;
        this->Label = Label; 
        this->distance = distance;
    }
};
class Kmean : public Point
{
private:
    friend class Point; 
public:
    int numRows {0};
    int numCols {0};
    int minVal {0};
    int maxVal {0};
    int numPts {0};
    int K {0};
    int change {0};
  
    Kmean(int numRows, int numCols)
    {
        this->numRows = numRows;
        this->numCols = numCols; 
    }

    Kmean(int numRows, int numCols, int minVal, int maxVal, int numPts, int K)
    {
        this->numRows = numRows;
        this->numCols = numCols; 
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->numPts = numPts; 
        this->K = K; 
    }

    int extractPts(int** twoArr)
    {
        for(int i = 0; i < this->numRows; i++){
            for(int j = 0; j < this->numCols; j++){
                if(twoArr[i][j] > 0)
                {
                    numPts++;
                    outFile << i << " " << j << endl;
                }
            } 
        }
        return numPts; 
    }

    void loadPointSet(Point *setPoint)
	{
		int i, j;
		for (int m = 0; m < numPts; m++)
		{
			outFile >> i >> j;
			setPoint[m] = Point(i, j, 0, 9999.0);
		}
	}

    void kMeansClustering(int** imgAry, Point *Kcentroid, Point *setPoint)
    {
        int iteration {0};
        int index {0};
        assignLabel(setPoint);
        change = 1;
        while(change > 0)
        {
            //test Point2Image it might not work like that
            Point2Image(setPoint, imgAry);// retrieve the imgAry
            printImage(imgAry, iteration);
            change = 0;
            computeCentroids(setPoint, Kcentroid);//create in constructor
            index = 0;

            while(index < numPts)
            {
                DistanceMinLable(setPoint[index], Kcentroid);
                index++;
            }
            iteration++;
        }
    }

    void assignLabel(Point *setPoint)
    {
        int front {0};
        int back {numPts - 1};
        int label = 1; 
        while(front <= back)
        {
            setPoint[front].Label = label; 
            front++;
            label++;
            if(label > this->K)
            {
                label = 1;
            }
            
            setPoint[back].Label = label;
            back--;
            label++;

            if(label > this->K)
            {
                label = 1;
            }
        }
    }
    void computeCentroids(Point *setPoint, Point *Kcentroid)
    {
        int *sumX = new int[K + 1];
        int *sumY = new int[K + 1];
        int *totalPt = new int[K + 1];
        
        for(int i = 1; i < K + 1; i++)
        {
            sumX[i] = 0;
            sumY[i] = 0;
            totalPt[i] = 0;
        }

        int index {0};
        int label {1};
        while(index < numPts)
        {
            label = setPoint[index].Label;
            sumX[label] += setPoint[index].Xcoord;
            sumY[label] += setPoint[index].Ycoord;
            totalPt[label]++;

            index++;        
        }

        label = 1;  
        while(label <= K)
        {
            if(totalPt[label] != 0)
            {
                Kcentroid[label].Xcoord = (int)(sumX[label]/totalPt[label]);
                Kcentroid[label].Ycoord = (int)(sumY[label]/totalPt[label]);
            }
            label++;
        }  
    }

    void DistanceMinLable(Point &pt, Point *Kcentroids)
    {
        double minDist {99999};
        int minLabel {0};

        int label = 1;

        while(label <= K)
        {
            pt.distance = computeDist(pt, Kcentroids[label]);
            if(pt.distance < minDist)
            {
                minLabel = label;
                minDist = pt.distance; 
            }
            label++;
        }
        if(pt.Label != minLabel)
        {
            pt.Label = minLabel;
            pt.distance = minDist;
            change++;  
        }
    }

    double computeDist(Point pt, Point Kcentroid)
    {
        double dist {0.0};
       return dist = sqrt(pow(pt.Xcoord - Kcentroid.Xcoord, 2) + pow(pt.Ycoord - Kcentroid.Ycoord, 2)); 
    }

    void writePtSet(Point *setPoint)
    {
        outFile3 << numPts <<endl;
        outFile3 << numRows << " ";
        outFile3 << numCols << endl;
        
        for(int i = 0; i < numPts; i++)
        {
            outFile3<< setPoint[i].Xcoord << " " << setPoint[i].Ycoord << " " << setPoint[i].Label;
            outFile3<<endl; 
        }
    }

    void Point2Image(Point* setPoint, int** imgAry)
    {
        for(int i = 0; i < numPts; i++)
        {
            imgAry[setPoint[i].Xcoord][setPoint[i].Ycoord] = setPoint[i].Label;
        }
    }

    void printImage(int **imgAry, int iteration)
    {
        cout << "In printImage " <<endl; 
        //nested for loop build and then send to outFile2  
        outFile2 << "********* Result of iteration " << iteration << "***********" << endl;
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++)
            {
                if(imgAry[i][j] > 0)
                {
                    outFile2 << imgAry[i][j] << " ";
                }
                else if(imgAry[i][j] == 0)
                {
                    outFile2 << " ";
                }
            }
            outFile2 << endl; 
        }  
    }

};
int main(int argc, char *argv[])
{
    inFile.open(argv[1]);
    int b = atoi(argv[2]);
    outFile.open(argv[3], ios::out);
    outFile2.open(argv[4], ios::out);
    outFile3.open(argv[5], ios::out);

   
    if(!inFile.is_open())
    {
        cerr << "error in opening file" <<endl;
        return 0; 
    }
    
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int K = b; 

    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal; 
  
    cout<< numRows << ", "  << numCols << ", " << minVal << ", " << maxVal <<endl; 

    int a = {0};
    int *arr = new int[numRows * numCols];
    int **twoArr = new int*[numRows];
    int **imgArr = new int*[numRows];
    Point *Kcentroids = new Point[K + 1]; 
        
    for(int i = 0; i < numRows; i++)
    {
        imgArr[i] = new int[numCols];
    }

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            imgArr[i][j] = 0;
        }   
    }

    for(int i = 0; i < numRows; i++){
        twoArr[i] = new int[numCols];
    }

    
    for(int i = 0; i < (numRows * numCols); i++){
        inFile >> a;
        arr[i] = a;
    }

    int x {0};

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            twoArr[i][j] = arr[x]; 
            x++; 
        }
    }

    for (int i = 1; i < K + 1; i++)
	{
		Kcentroids[i] = Point(0, 0, 0, 0.0);
	}

    int numPts;
    Kmean obj(numRows, numCols);
    numPts = obj.extractPts(twoArr);
    Point *pointSet = new Point[numPts];
	for (int i = 0; i < numPts; i++)
	{
		pointSet[i] = Point(0, 0, 0, 99999);
	}

    cout << "NumPts: " << numPts <<endl; 
    outFile.close();

    outFile.open(argv[3], ios::in);
    Kmean first(numRows, numCols, minVal, maxVal, numPts, K);
    first.loadPointSet(pointSet);
    first.kMeansClustering(imgArr, Kcentroids, pointSet);
    first.writePtSet(pointSet);

    inFile.close(); 
    outFile.close();
    outFile2.close();
    outFile3.close();
}