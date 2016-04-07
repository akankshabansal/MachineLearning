#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <time.h>
//#include <utility>
#include <iostream>
#include <vector>

#define chromeL 200

using namespace std;

class SortNet{
    vector<vector<int>> inputs;
	vector<vector<int>> sortedInputs; //Sorted inputs 
    vector<vector<pair<int, int>>> sols;
    int arrSize; //Size of each of the sample (default value to be 16)
    int testCases; //No of Samples Inputs to test upon
    int solCount; //No of Solutions we test with 
    int optimal; //Solution with smallest length 
    double optFit; //Fitness of the optimal solution
	int worst;
	double worFit; 
	double weight; 
	int mutation; 

    public:
    SortNet(int,int,int,double);
    void lifecycle(int);
    double fitness(vector<double>&);
    void output();
    void solCrossover(vector<pair<int, int>>& , vector<pair<int, int>>&, vector<vector<pair<int, int>>> &, int &);
    void generateInputs(); //Generate new Input test arrays and then 
    void optimalSol();//Find the optimal Solution based on the Fitness matrix 
    ~SortNet();
};

// Function for quick sort
// result: array to be sorted
void quickSortHelper(vector<int> &result, int start, int end){
    if (start < end){
        int pivot = result[start];
        int point = start;

        swap(result[start], result[end]);

        for (int i=start; i< end; i++){
            if (result[i] <= pivot){
                swap(result[i], result[point]);
                point++;
            }
        }
        swap(result[point], result[end]);

        if (point > 0)
            quickSortHelper(result, start, point-1);
        if (point < end)
            quickSortHelper(result, point+1, end);
    }
};

/* quick sort for int array
 array: original array
 size: size of the array
 result: sorted array
*/
void quickSort(vector<int> &array, int size, vector<int> &result){
    result = array;
    int start = 0;
    int end = size - 1;
    quickSortHelper(result, start, end);
};

bool compare(const pair<double, int>& pr1, const pair<double, int>& pr2){
    return pr1.first > pr2.first;
}

// Generate Inputs
void SortNet::generateInputs(){
 	// construct inputs randomly
    for (int i=0; i< testCases; i++){
        for (int j=0; j< arrSize; j++){
            this->inputs[i][j] = rand() % 100;
        }
    }
    //Store them in sorted manner them
    for (int i=0; i< this->testCases; i++){
        quickSort( this->inputs[i], this->arrSize, this->sortedInputs[i]);
    }
};

SortNet::SortNet(int arrSize, int testCases, int solCount, double weight){
    this->testCases = testCases;
    this->arrSize = arrSize;
    this->solCount = solCount; // *2 was to make sure the soultions are even in no so we can split them into mothers and fathers when we do corss-over :P
	this->weight = weight;
	//Generatetion of Inputs 
	this->inputs.resize(this->testCases);
	this->sortedInputs.resize(this->testCases);
	for (int i=0; i< testCases; i++){
        this->inputs[i].resize(arrSize);
        for (int j=0; j< arrSize; j++){
            this->inputs[i][j] = rand() % 100;
        }
    }
	for (int i=0; i< this->testCases; i++){
        this->sortedInputs[i].resize(this->arrSize);
        quickSort( this->inputs[i], this->arrSize, this->sortedInputs[i]);
    }    
	this->mutation = 1; 

    // construct solutions the number given as input
    this->sols.resize(this->solCount);
    for (int i=0; i< this->solCount; i++){
        // Each Solution is created first of length 50
        this->sols[i].resize(chromeL);
        for (int j=0; j < chromeL; j++){
            int left = rand()%arrSize;
            int right = rand()%arrSize;
            //Randomly create the swap condition. However only swap them from lower to higher index.
            if (left < right)       this->sols[i][j] = make_pair(left,right);
            else if (left > right)  this->sols[i][j] = make_pair(right, left);
            else if (left > 0)      this->sols[i][j] = make_pair(left - 1 ,right );
            else                    this->sols[i][j] = make_pair(0, 1);
        }
    }
};


// destructor
SortNet::~SortNet(){	
	this->inputs.clear();
    this->sols.clear();
};

void SortNet::output(){
    cout << "Displaying sorting network:" << endl;
    cout << "inputs: " << endl;
    for (int i=0; i < this->testCases; i++){
        for (int j=0; j < this->arrSize; j++){
            cout << this->inputs[i][j] << " ";
        }
        cout << endl;
    }
	
	cout << "Displaying sorted Inputs:" << endl;
    cout << "inputs: " << endl;
    for (int i=0; i < this->testCases; i++){
        for (int j=0; j < this->arrSize; j++){
            cout << this->sortedInputs[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Solutions: "<< this->solCount << endl;
    for (int i=0; i < this->solCount; i++){
        for (int j=0; j < this->sols[i].size(); j++){
            cout << "(" << this->sols[i][j].first << "," << this->sols[i][j].second << ")" << " ";
        }
      cout << endl <<endl;
    }
	cout << "weight" << this->weight <<endl;
};

// Printing out the best solution
void SortNet::optimalSol(){
	int l = this->sols[this->optimal].size();
    cout << "optimal solution has length " << l;
    cout << " and fitness " << this->optFit << endl;
    for (int i=0; i < l; i++){
        cout << "(" << this->sols[this->optimal][i].first << "," << this->sols[this->optimal][i].second << ") ";
    }
    cout << endl;
}

// Compare and swap the values
void compswap(pair<int, int> &pr, vector<int>& result){
    if(result[pr.first] > result[pr.second])
        swap(result[pr.first], result[pr.second]);
};

// swap a and b
void swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/** Calculate the fitness of each solution with respect to all input array
 i: the ith solution
 fitMat: fitness matrix to be filled in
 return averaged fitness for given set of Test Cases
**/
double SortNet::fitness(vector<double> &fitMat){
    double avgfit = 0;
	this->optimal = 0; 
	this->optFit = 0;
	this->worFit = 1;
	this->worst = this->solCount - 1; 
	//cout << endl << "Fitness Start!!"<< endl;
	
	int longest = 0; 
	int smallest = 32767; 
	for (int i = 0 ; i < this->solCount ; i++){
		if (this->sols[i].size() > longest){
			longest = this->sols[i].size();
		} 
		if(this->sols[i].size() < smallest ){
			smallest = this->sols[i].size();
		}
	}	

	for(int i = 0 ; i < this->solCount; i++){
    	int fit = 0; 
		for (int j=0; j < this->testCases; j++){
        	vector<int> result(this->inputs[j].size());
			result = this->inputs[j];
        	for (int k=0; k < this->sols[i].size(); k++){
				pair<int,int> chrome = this->sols[i][k];
            	compswap(chrome, result);
        	}
        	for (int k=0; k < this->arrSize; k++){
				if (result[k] == this->sortedInputs[j][k])  
					fit++;
        	}
		}
        fitMat[i] = (((double) fit)/this->arrSize)/this->testCases;
		//cout << fitMat[i] << ", " ;
		if(longest !=smallest){
			//if ((this->sols[i].size() > 80) || (this->sols[i].size() < 61) ){
			fitMat[i] = fitMat[i] - this->weight*((((double)this->sols[i].size()) - smallest )*(((double)this->sols[i].size()) - smallest)*(((double)this->sols[i].size()) - smallest)/((longest-smallest)*(longest-smallest)*(longest-smallest)));
			//cout << fitMat[i] << "; " ;
			//}
			this->mutation = 1;
		}else if (longest != chromeL) {
			cout << "Needed Special Mutation" << endl;
			this->mutation = 800;
		}	
		
        avgfit += fitMat[i];
		if(this->optFit < fitMat[i]){
			this->optimal = i;
			this->optFit = fitMat[i];
		}
		if(this->worFit > fitMat[i]){
            this->worst = i;
            this->worFit = fitMat[i];
        }
    }
	//cout << endl << "Fitness Done!!"<< endl;
    return avgfit/this->solCount;
};


// Solutions cross over 2 Solutions 
void SortNet::solCrossover(vector<pair<int, int>> &father, vector<pair<int, int>> &mother, vector<vector<pair<int, int>>> &newSols, int &n){
	int child1_size = 0; //fat + mother.size() - mot; 
	int child2_size = 0; //mot + father.size() - fat;
	int fat = 0;
	int mot = 0;  
	while (child1_size > 600 || child1_size < 60 || child2_size > 600 || child2_size < 60 ) {
		fat = rand() % father.size();
    	mot = rand() % mother.size();
		//mot = fat;
		child1_size = fat + mother.size() - mot;
		child2_size = mot + father.size() - fat;
	}
	vector<pair<int, int>> child1(child1_size) ; 
	vector<pair<int, int>> child2(child2_size);
    for (int i=0; i < fat; i++){
			child1[i] = father[i]; 
	}
	for(int i = fat; i < child1_size ; i++){
			child1[i] = mother[i - fat + mot]; 
	}
	for (int i=0; i < mot; i++){
            child2[i] = mother[i];
    }
	for(int i = mot ; i < child2_size; i++){
            child2[i] = father[i - mot + fat];
    }
	newSols[n++] = child1; 
	if (n < this->solCount) 
		newSols[n++] = child2; 
};

// Evolve both the input/solution arrays for the no of generation iterations provided
void SortNet::lifecycle(int generations){
    cout << "Life cycle begins:" << endl;
    for (int generation=0; generation < generations; generation++){
		// 1. evaluate fitness: store fitness for each solution with respect to each input
		vector<double> fitMat(this->solCount);
		vector<double> solFit(this->solCount);
        // averaged & normalized fitness for each solution/question
        double avgFit = this->fitness(fitMat);
		
		for(int i =0; i < this->solCount; i++){
			solFit[i] = (fitMat[i] - this->worFit)/(this->optFit - this->worFit);
		}
		if (! (generation%500)){
            cout << "generation: " << generation << endl;
        	this->optimalSol();
		}
        // 2. select m out of total solutions with replacement, cross over, generate n new solutions
        // opitional: top k solutions from previous iteration can be kept
        // sols: rank of selected solutions
        int m = (this->solCount)/2  ;
        int i = 0;
        int selectSols[m];
        while(m){
			int idx = rand() % this->solCount;
            double prob = (double)rand() / RAND_MAX;
            if (prob < solFit[idx] ){
                selectSols[i++] = idx;
                m--;
            }
        }
		m = (this->solCount)/2;
		//cout << "selected " << m << " survivors" << endl;
		int n = 0;
        vector<vector<pair<int, int>>> newSols(this->solCount);
		newSols[n++] = this->sols[this->optimal];
		//n = n+1;
		while(n < this->solCount){
			int father = selectSols[rand() % m];
			int mother = selectSols[rand() % m];
			//cout<< father << ", " << mother << endl;  
		    this->solCrossover(this->sols[father], this->sols[mother], newSols, n);
        }   
		//cout << "Generated n values" << endl;
        this->sols = newSols; 
		newSols.clear();
		// 3. introducing randomness(mutation) in the data set pool frequency: 0.1% for lower fitness sorting nets
        for (int i=0; i < this->solCount; i++){
			if ( rand() % 1000 < this->mutation){
				this->sols[i].resize(chromeL);
				for (int j=0; j< this->sols[i].size(); j++){
					int left = rand() % this->arrSize;
                    int right = rand() % this->arrSize;
                    if (left < right)       this->sols[i][j] = make_pair(left,right);
                    else if (left > right)  this->sols[i][j] = make_pair(right,left);
                    else if (left > 0)      this->sols[i][j] = make_pair(left - 1,right);
                    else                    this->sols[i][j] = make_pair(0, 1);
                }
            }
        }
        // generate the Testing inputs again for the next cycle
		this->generateInputs();
    }
    cout << "life cycle ends." << endl;
};


int main(int argc, char* argv[]){
    srand(time(NULL));
    if (argc != 5){
        cerr <<  argv[0] << " [input number: Samples to test on] [number of solutions ] [weight of length in double] [generations: no of iterations to run]" << endl;
        return 0;
    }

    double start_time = time(0);
    // Initialize the Sorting net with network size, number of inputs, number of solutions, weight for the length of the sorting sequence.. 
    SortNet* sn = new SortNet(16, atoi(argv[1]), atoi(argv[2]) * 2, atof(argv[3]));
    cout << "Constructed the sorting network!!!" << endl;
    //sn->output();
    sn->lifecycle(atoi(argv[4]));
   	vector<double> fitMat(atoi(argv[2]) * 2); 
	sn->fitness(fitMat); 
	sn->optimalSol();
    cout << "Total time: " << time(0) - start_time << endl;
    //sn->output();
    return 0;
}
