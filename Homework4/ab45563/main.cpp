#include <cstdlib>
#include <iostream>
#include "network.h"
#include "node.h"

using namespace std;

double toss();

int main(int argc, char** argv) {

    //Bayes Network Initialization
    network net;

	net.addNode("Recession");
    net.addNode("Burglary");
    net.addNode("Earthquake");
	net.addNode("Police");
    net.addNode("RadioAnoucement");
    net.addNode("Alarm");
    net.addNode("TV");
    net.addNode("Nap");
    net.addNode("JohnCall");
    net.addNode("MaryCall");

	net.addLink("Recession", "Burglary");
	net.addLink("Burglary", "Police");
	net.addLink("Earthquake", "RadioAnoucement");
    net.addLink("Burglary", "Alarm");
    net.addLink("Earthquake", "Alarm");
    net.addLink("Alarm", "JohnCall");
    net.addLink("Alarm", "MaryCall");
    net.addLink("TV", "JohnCall");
    net.addLink("Nap", "MaryCall");

	
	(*(net.searchNode("Recession"))).initTable();
	(*(net.searchNode("Recession"))).setTableValue(0, 0.0001);

    (*(net.searchNode("Burglary"))).initTable();//Only one parent so two entries to table
    (*(net.searchNode("Burglary"))).setTableValue(0, 0.0001);//Recession=0
	(*(net.searchNode("Burglary"))).setTableValue(1, 0.001);//Recession=1

	(*(net.searchNode("Police"))).initTable();
    (*(net.searchNode("Police"))).setTableValue(0, 0.000001);
	(*(net.searchNode("Police"))).setTableValue(0, 0.1);

    (*(net.searchNode("Earthquake"))).initTable();
    (*(net.searchNode("Earthquake"))).setTableValue(0, 0.002);//No parent only one entry to table

	(*(net.searchNode("RadioAnoucement"))).initTable();
    (*(net.searchNode("RadioAnoucement"))).setTableValue(0, 0.00001);
    (*(net.searchNode("RadioAnoucement"))).setTableValue(0, 0.9);

    (*(net.searchNode("Alarm"))).initTable();
    (*(net.searchNode("Alarm"))).setTableValue(0, 0.001);//Parent assignment Burglary=0 Earthquake=0
    (*(net.searchNode("Alarm"))).setTableValue(1, 0.29);//Parent assignment Burglary=0 Earthquake=1
    (*(net.searchNode("Alarm"))).setTableValue(2, 0.94);//Parent assignment Burglary=1 Earthquake=0
    (*(net.searchNode("Alarm"))).setTableValue(3, 0.95);//Parent assignment Burglary=1 Earthquake=1

    (*(net.searchNode("TV"))).initTable();
    (*(net.searchNode("TV"))).setTableValue(0, 0.7);

    (*(net.searchNode("Nap"))).initTable();
    (*(net.searchNode("Nap"))).setTableValue(0, 0.3);

    (*(net.searchNode("JohnCall"))).initTable();
    (*(net.searchNode("JohnCall"))).setTableValue(0, 0.05);//Parent assignment Alarm=0 TV=0
    (*(net.searchNode("JohnCall"))).setTableValue(1, 0.02);//Parent assignment Alarm=0 TV=1
    (*(net.searchNode("JohnCall"))).setTableValue(2, 0.9);//Parent assignment Alarm=1 TV=0
    (*(net.searchNode("JohnCall"))).setTableValue(3, 0.4);//Parent assignment Alarm=1 TV=1

    (*(net.searchNode("MaryCall"))).initTable();
    (*(net.searchNode("MaryCall"))).setTableValue(0, 0.01);//Parent assignment Alarm=0 Nap=0
    (*(net.searchNode("MaryCall"))).setTableValue(1, 0.002);//Parent assignment Alarm=0 Nap=1
    (*(net.searchNode("MaryCall"))).setTableValue(2, 0.7);//Parent assignment Alarm=1 Nap=0
    (*(net.searchNode("MaryCall"))).setTableValue(3, 0.2);//Parent assignment Alarm=1 TV=1


    int assign[10];//No of nodes in the system finally
    assign[0] = 1;
    assign[1] = 1;
    assign[2] = 1;
    assign[3] = 0;
    assign[4] = 1;
    assign[5] = 1;
    assign[6] = 1;
    assign[7] = 1;
    assign[8] = 0;
	(*(net.nodeAt(8))).setObserved(1);
    assign[9] = 1;
	(*(net.nodeAt(9))).setObserved(1);
	net.assign(assign);

    //Actual Probability Distribution for Comparison
    //P(B|j0,m1) 
    net.listNodes();
	cout << net.pCnode(8, 1) << endl;
	//Gibbs sampling
    cout << net.gibbsSampling(8,1,100000) << endl;
	//Rejection sampling 
	cout << net.rejectionSampling(8,1,100000) << endl;
    //cout << (*(net.searchNode("Alarm"))).getLevel() << endl;
    net.topologicalSort();
    return 0;
}

double toss() {
    for (int i = 0; i < ((double) rand()) / ((double) RAND_MAX)*1000; i++) {
        rand();
    }
    return ((double) rand()) / ((double) RAND_MAX);
}
