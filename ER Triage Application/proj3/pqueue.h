// CMSC 341 - Fall 2023 - Project 3
#ifndef PQUEUE_H
#define PQUEUE_H

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

class Grader; // forward declaration (for grading purposes)
class Tester; // forward declaration (for test functions)
class PQueue; // forward declaration
class Patient;// forward declaration
#define EMPTY Patient() // This is an empty object (invalid patient)
enum HEAPTYPE {MINHEAP, MAXHEAP};
enum STRUCTURE {SKEW, LEFTIST};
// Priority function pointer type
typedef int (*prifn_t)(const Patient&);

// Triage parameters, min and max values
const int MINTEMP = 35; // Body temperature, celsius
const int MAXTEMP = 42;
const int MINOX = 70;   // Level of oxygen saturation (SpO2)
const int MAXOX = 101;
const int MINRR = 10;   // Respiratory Rate, per minute
const int MAXRR = 40;
const int MINBP = 70;   // Blood Pressure
const int MAXBP = 160;
const int MINOPINION = 1;   // Nurse opinion, between 1 - 10
const int MAXOPINION = 10;  // 1 is highest priotity
//
// patient class
//
class Patient {
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    friend class PQueue;
    Patient() {
        // This is an empty object since name is empty
        m_patient = ""; m_temperature = 37; m_oxygen = 100;
        m_RR = 20;m_BP = 100;m_opinion=10;
    }
    Patient(string name, int temp, int ox, int rr, int bp, int op) {
        if ( (temp < MINTEMP || temp > MAXTEMP) || 
        (ox < MINOX || ox > MAXOX) || (rr < MINRR || rr > MAXRR) || 
        (bp < MINBP || bp > MAXBP) || (op < 1 || op > 10)){
            // create an empty object
            m_patient = ""; m_temperature = 37; m_oxygen = 100;
            m_RR = 20; m_BP = 100;m_opinion=10;
        }
        else{
            m_patient = name; m_temperature = temp; m_oxygen = ox;
            m_RR = rr; m_BP = bp;m_opinion=op;
        }
    }
    string getPatient() const {return m_patient;}
    int getTemperature() const {return m_temperature;}
    int getOxygen() const {return m_oxygen;}
    int getRR() const {return m_RR;}
    int getBP() const {return m_BP;}
    int getOpinion() const {return m_opinion;}
    void setPatient(string name) {m_patient=name;}
    void setTemperature(int val) {m_temperature=val;}
    void setOxygen(int val) {m_oxygen=val;}
    void setRR(int val) {m_RR=val;}
    void setBP(int val) {m_BP=val;}
    void setOpinion(int val) {m_opinion=val;}
    // Overloaded assignment operator
    const Patient & operator=(const Patient& rhs){
        if (this != &rhs){
            m_patient = rhs.m_patient;
            m_temperature = rhs.m_temperature;
            m_oxygen = rhs.m_oxygen;
            m_RR = rhs.m_RR;
            m_BP = rhs.m_BP;
            m_opinion = rhs.m_opinion;
        }
        return *this;
    }
    // Overloaded equality operator
    bool operator==(const Patient & rhs) const {
        return ((m_patient == rhs.m_patient) &&
                (m_temperature == rhs.m_temperature) &&
                (m_oxygen == rhs.m_oxygen) &&
                (m_RR == rhs.m_RR) &&
                (m_BP == rhs.m_BP) &&
                (m_opinion == rhs.m_opinion));
    }
    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Patient& patient);

    private:
    string m_patient;  // Patient's name, no need to be unique
    int m_temperature; // Body temperature, celsius
    int m_oxygen;      // Level of oxygen saturation (SpO2), percentage
    int m_RR;          // Respiratory Rate, per minute
    int m_BP;          // Blood Pressure
    int m_opinion;     // Nurse opinion, 1 - 10
};

class Node {
    // this is a node in the skew/leftist heap
    public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    friend class PQueue;
    Node(Patient patient) {  
        m_patient = patient;
        m_right = nullptr;
        m_left = nullptr;
        m_npl = 0;
    }
    Patient getPatient() const {return m_patient;}
    void setNPL(int npl) {m_npl = npl;}
    int getNPL() const {return m_npl;}

    // Overloaded insertion operator
    friend ostream& operator<<(ostream& sout, const Node& node);

    private:
    Patient m_patient;   // Patient information
    Node *m_right;       // Right child
    Node *m_left;        // Left child
    int m_npl;           // null path length for leftist heap
};

class PQueue {
    // stores the skew/leftist heap, minheap/maxheap
public:
    friend class Grader; // for grading purposes
    friend class Tester; // contains test functions
    PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure);
    ~PQueue();
    PQueue(const PQueue& rhs);
    PQueue& operator=(const PQueue& rhs);
    void insertPatient(const Patient& input);
    Patient getNextPatient();
    void mergeWithQueue(PQueue& rhs);
    void clear();
    int numPatients() const;
    // Print the queue using preorder traversal.  Although the first patient
    // printed should have the highest priority, the remaining patients will
    // not necessarily be in priority order.
    void printPatientQueue() const;
    prifn_t getPriorityFn() const;
    // Set a new priority function.  Must rebuild the heap!!!
    void setPriorityFn(prifn_t priFn, HEAPTYPE heapType);
    HEAPTYPE getHeapType() const;
    STRUCTURE getStructure() const;
    // Set a new data structure (skew/leftist). Must rebuild the heap!!!
    void setStructure(STRUCTURE structure);
    void dump() const;  // For debugging purposes.

private:
    Node * m_heap;          // Pointer to root of skew heap
    int m_size;             // Current size of the heap
    prifn_t m_priorFunc;    // Function to compute priority
    HEAPTYPE m_heapType;    // either a MINHEAP or a MAXHEAP
    STRUCTURE m_structure;  // skew heap or leftist heap

    void dump(Node *pos) const; // helper function for dump

    /******************************************
    * Private function declarations go here! *
    ******************************************/

    //merge helper min and max
    Node* MinMergeHelper(Node* rhs, Node* heap);
    Node* MaxMergeHelper(Node* rhs, Node* heap);

    //max and min leftist
    Node* MaxLeftistMergeHelper1(Node* LH, Node* RH);
    Node* MinLeftistMergeHelper1(Node* LH, Node* RH);

    // max and min skew
    Node* MaxSkewMergeHelper1(Node* LH, Node* RH);
    Node* MinSkewMergeHelper1(Node* LH, Node* RH);

    // child swap functoin
    void ChildSwap(Node*& heap);

    // checks the npl value
    int nplHelper(Node* node);

    // clears the heap passed
    void ClearHelper(Node*& heap);

    // helps copy the node passed
    void heapCopyHelper(Node*& heap, const Node* rhs);

    // prints the contents of the heap passed
    void printPatientQueueHelper(const Node* heap)const;

};

#endif