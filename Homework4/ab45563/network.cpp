#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <stdio.h>     
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#include "network.h"
#include "node.h"

network::network() {
}

network::network(const network& orig) {
}

network::~network() {
}

node* network::searchNode(string name) {
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes.at(i).getName().compare(name) == 0)
            return &(nodes.at(i));
    }
    return NULL;
}

void network::addNode(string name) {
    node newNode;
    newNode.setName(name);
    nodes.push_back(newNode);
}

void network::listNodes() {
    for (int i = 0; i < nodes.size(); i++) {
        node x = nodes.at(i);
        cout << i + 1 << ": " << x.getName() << "-" << x.getNparents() << " parents are:" ;
		x.listParents();
    }
}

void network::addLink(string parent, string child) {
    node* p = searchNode(parent);
    node* c = searchNode(child);

    (*c).addParent(p);
    (*p).addChild(c);
}

double network::pjoint(int* assign) {
    double pjoint = 1;
    for (int i = 0; i < nodes.size(); i++) {
        nodes.at(i).setValue(assign[i]);
        pjoint = pjoint * (nodes.at(i).factor(assign[i]));
    }
    return pjoint;
}

double network::pCnode(int pos, int value) {
    nodes.at(pos).setObserved(0);
    vector< vector<int> > assignlist;
    vector<int> assignArray;
    for (int i = 0; i < nodes.size(); i++) {
        assignArray.push_back(nodes.at(i).getValue());
    }
    assignlist.push_back(assignArray);
    assignArray.clear();
    for (int i = 0; i < nodes.size(); i++) {
        if (!nodes.at(i).isObserved()) {
            int listSize = assignlist.size();
            for (int j = 0; j < listSize; j++) {
                ;
                for (int k = 0; k < nodes.size(); k++) {
                    if (k == i) {
                        assignArray.push_back(!(assignlist.at(j).at(k)));
                    } else {
                        assignArray.push_back(assignlist.at(j).at(k));
                    }
                }
                assignlist.push_back(assignArray);
                assignArray.clear();
            }
        }
    }


    double pEvidence = 0;
    double pnode = 0;

    int aarray[nodes.size()];

    for (int i = 0; i < assignlist.size(); i++) {
        for (int k = 0; k < nodes.size(); k++) {
            aarray[k] = assignlist.at(i).at(k);
        }
        double pjointv = pjoint(aarray);
        if (aarray[pos] == value) {
            pnode += pjointv;
            pEvidence += pjointv;
        } else {
            pEvidence += pjointv;
        }
    }

    return pnode / pEvidence;

}

double network::gibbsSampling(int pos, int value, int t) {
    int size = nodes.size();
    int sample[2][size];

    srand((unsigned int) time(NULL));
    topologicalSort();
    for (int i = 0; i < size; i++) {
        int j = topOrder[i];
        if (nodes.at(j).isObserved()) {
            sample[0][j] = nodes.at(j).getValue();
        } else {
            sample[0][j] = (toss() <= (nodes.at(j).factor(1)));
            nodes.at(j).setValue(sample[0][j]);
        }
    }

    //GIBBS SAMPLING
    double sumMix = 0;
    double sum = 0;
    int mixSize = t*0.02;
    int mixStart = 0;
    for (int i = 1; i <= t; i++) {
        assign(sample[0]);
        for (int j = 0; j < size; j++) {
            if ((*(nodeAt(j))).isObserved()) {
                sample[1][j] = sample[0][j];
            } else {
                sample[1][j] = (toss() <= (pSamplingNode(j, 1)));
                (*(nodeAt(j))).setValue(sample[1][j]);
            }
        }
        //MIXING PROCESS
        if (i <= mixStart + mixSize) {
            sumMix += pSamplingNode(pos, value);
        } else {
            sum += pSamplingNode(pos, value);
        }
        if (i == mixStart + mixSize * 2) {
            if (abs(sumMix / mixSize - sum / mixSize) < 0.001) {
                sum += sumMix;
            }else{
                sumMix = sum;
                sum = 0;
                mixStart += mixSize;
            }
        }
        for (int j = 0; j < size; j++) {
            sample[0][j] = sample[1][j];
        }
    }
    return sum/(t-mixStart);
}

double network::rejectionSampling(int pos, int value, int t) {
    int size = nodes.size();
    int sample[2][size];

    srand((unsigned int) time(NULL));
    //Create a starting point for samppling
    topologicalSort();
	for (int i = 0; i < size; i++) {
        int j = topOrder[i];
        if (nodes.at(j).isObserved()) {
            sample[0][j] = nodes.at(j).getValue();
        } else {
            sample[0][j] = (toss() <= (nodes.at(j).factor(1)));
            nodes.at(j).setValue(sample[0][j]);
        }
    }    

    //REJECTION SAMPLING
    int rejects = 0;
	double count = 0;
	bool reject = false;
    for (int i = 1; i <= t; i++) {
        assign(sample[0]);
		reject = false;
        for (int j = 0; j < size; j++) {
			sample[1][j] = (toss() <= (pSamplingNode(j, 1)));
			if ((*(nodeAt(j))).isObserved()  && sample[1][j] == 0) {
				reject = true;
			}
        }
		if (reject){
        		rejects++;
		} else if(sample[1][pos]==value){
			count++;
		}
		for (int j = 0; j < size; j++) {
            sample[0][j] = sample[1][j];
        }
	}
    cout << "Rejection Sampling : Values with needed result :"<< count << ", Samples collected: " << t << ", Samples rejected: " << rejects << endl;
	return count/(t-rejects);
}

double network::pSamplingNode(int pos, int value) {

    nodes.at(pos).setValue(value);
    double pEvidence = 0;
    double pnode = 0;

    pnode = nodes.at(pos).factor();
    for (int i = 0; i < nodes.at(pos).getNchildren(); i++) {
        pnode *= (*(nodes.at(pos).getChild(i))).factor();
    }

    nodes.at(pos).setValue(!value);
    pEvidence = nodes.at(pos).factor();
    for (int i = 0; i < nodes.at(pos).getNchildren(); i++) {
        pEvidence *= (*(nodes.at(pos).getChild(i))).factor();
    }
    pEvidence += pnode;

    return pnode / pEvidence;
}

/*Assign the nodes the sample values generated */
void network::assign(int* assign) {
    for (int i = 0; i < nodes.size(); i++) {
        nodes.at(i).setValue(assign[i]);
    }
}

/*Return the node a position pos */
node* network::nodeAt(int pos) {
    return &(nodes.at(pos));
}

/*Function to set the level in the network before processing begins on the network  */
void network::topologicalSort() {
    topOrder = new int[nodes.size()];
    int level = 0;
    int count = 0;
    while (count < nodes.size()) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes.at(i).getLevel() == level) {
                topOrder[count] = i;
                count++;
            }
        }
        level++;
        if (level > nodes.size())
            break;
    }

}

/*Function to generate Random no*/
double network::toss() {
    for (int i = 0; i < ((double) rand()) / ((double) RAND_MAX)*1000; i++) {
        rand();
    }
    return ((double) rand()) / ((double) RAND_MAX);
}
