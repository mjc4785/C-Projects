// CMSC 341 - Fall 2023 - Project 3

#include "pqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Priority functions compute an integer priority for a patient.  Internal
// computations may be floating point, but must return an integer.

int priorityFn1(const Patient & patient);
int priorityFn2(const Patient & patient);

// a name database for testing purposes
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green", 
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong", 
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};

// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:

    //start of tests

    //normal case min heap with 300 node inserts
    bool minTest(){

        PQueue queue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 300); // creates a new pqueue object

        if(heapIntegrity(queue.m_heap, queue.m_heapType, queue.m_priorFunc)){

            return true;

        }
        else{

            return false;

        }

        return false;

    }       
    //normal case max heap with 300 node inserts
    bool maxTest(){
        
        PQueue queue = pqueueMaker(MAXHEAP, LEFTIST, priorityFn2, 300); // creates a new pqueue object

        if(heapIntegrity(queue.m_heap, queue.m_heapType, queue.m_priorFunc)){

            return true;

        }
        else{

            return false;

        }

        return false;
        
        }       
    
    //remove test for 300 nodes. checks to see if theyre removed in the corrects order
    bool removeMinTest(){
        
        PQueue queue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        vector<Patient> qvector;
        bool flag = true;

        while (queue.m_heap != nullptr){ //while the heap is not null

            qvector.push_back(queue.getNextPatient()); //  getnext patient and append it to a vector.

        }

        for (unsigned int i = 0; i < qvector.size()-1; i++){

            if(queue.m_priorFunc(qvector[i]) > queue.m_priorFunc(qvector[i+1])){flag = false;}

        }

        return flag;
        
        } 
    //remove test for 300 nodes. checks to see if theyre removed in the correct order
    bool removeMaxTest(){
        
        PQueue queue = pqueueMaker(MAXHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        vector<Patient> qvector;
        bool flag = true;

        while (queue.m_heap != nullptr){ //while the heap is not null

            qvector.push_back(queue.getNextPatient()); //  getnext patient and append it to a vector.

        }

        for (unsigned int i = 0; i < qvector.size()-1; i++){

            if(queue.m_priorFunc(qvector[i]) < queue.m_priorFunc(qvector[i+1])){flag = false;}

        }

        return flag;
        
        } 
    
    //checks if all the nodes in a leftest heap have the correct npl value
    bool nplTest(){
        
        PQueue queue = pqueueMaker(MAXHEAP, LEFTIST, priorityFn2, 6); // creates a new pqueue object

        // if the check of the new queue's heap's npl values is correct...

        cout << endl << endl;
        queue.dump();
        cout << endl << endl;

        if(nplCheck(queue.m_heap)){return true;}
        // if its not correct
        else{return false;}
        
        }

    // checks if leftist heap is preserved
    bool leftistTest(){

        PQueue queue = pqueueMaker(MAXHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object

        if(heapIntegrity(queue.m_heap, queue.m_heapType, queue.m_priorFunc)){return true;}
        // if its not correct
        else{return false;}

        return false;
        
        }
    
    //tests to see if the function change priority works
    bool priorChangeTest(){
        
        PQueue queue = pqueueMaker(MINHEAP, SKEW, priorityFn2, 30); // creates a new pqueue object
        queue.setPriorityFn(priorityFn1, MAXHEAP);

        if(heapIntegrity(queue.m_heap, queue.m_heapType, queue.m_priorFunc)){

            return true;

        }
        else{

            return false;

        }

        return false;
    
    }

    // tests to see if when you merge two heaps but one is null that it returns the full heap
    bool nullMergeTest(){
        
        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        PQueue Bqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 0);  // creates a new pqueue object
        int size = Aqueue.m_size;

        Aqueue.mergeWithQueue(Bqueue);

        if (size == Aqueue.m_size){return true;}
        else{return false;}
        
        }

    //PQueue copy for a normal case
    bool copyTest(){
        
        //getnextPatient == 
        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        bool flag = true;
        PQueue Bqueue(Aqueue);

        while(Aqueue.m_heap != nullptr && Bqueue.m_heap != nullptr){

            if(Aqueue.getNextPatient() == Bqueue.getNextPatient()){}
            else {flag = false;}

        }

        return flag;
        
    }

    //PQueue cop for edge case
    bool copyEdgeTest(){
        
        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 0); // creates a new pqueue object
        bool flag = true;
        PQueue Bqueue(Aqueue);

        while(Aqueue.m_heap != nullptr && Bqueue.m_heap != nullptr){

            if(Aqueue.getNextPatient() == Bqueue.getNextPatient()){}
            else {flag = false;}

        }

        return flag;
    
    }

    //PQueue assignment operator for normal case
    bool assignmentTest(){

        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        bool flag = true;
        PQueue Bqueue = Aqueue;
        while(Aqueue.m_heap != nullptr && Bqueue.m_heap != nullptr){

            if(Aqueue.getNextPatient() == Bqueue.getNextPatient()){}
            else {flag = false;}

        }

        return flag;

    }

    //PQueue assignment op for edge case
    bool assignmentEdgeTest(){
        
        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 0); // creates a new pqueue object
        bool flag = true;
        PQueue Bqueue = Aqueue;
        while(Aqueue.m_heap != nullptr && Bqueue.m_heap != nullptr){

            if(Aqueue.getNextPatient() == Bqueue.getNextPatient()){}
            else {flag = false;}

        }

        return flag;

    }

    //test that attempting dequeue an emptyqueue throws out a out of range exception
    void dequeueTest(){
        
        PQueue queue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 0); // creates a new pqueue object

        queue.getNextPatient();
        
        }

    //test that maerge with two different priority function pqueues throws a domain error exception
    void diffPriorTest(){
        
        PQueue Aqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn2, 30); // creates a new pqueue object
        PQueue Bqueue = pqueueMaker(MINHEAP, LEFTIST, priorityFn1, 30); // creates a new pqueue object

        Aqueue.mergeWithQueue(Bqueue);

    }

/***************************************************************/
    //helper to test the property of the heap
    bool heapIntegrity(Node* heap, HEAPTYPE heapType, prifn_t priority){
        
        // if the heap is null, it is a valid tree so return true
        if(heap == nullptr){ return true; }

        if(heapType == MINHEAP){ //min heap    
            
            // if the left child isnt null and its priority is less than the parent's...
            // or the right child isnt null and its priority is less than the parent's...
            if((heap->m_left != nullptr && priority(heap->m_left->m_patient) < priority(heap->m_patient)) || 
               (heap->m_right != nullptr && priority(heap->m_right->m_patient) < priority(heap->m_patient))){

                //return false
                return false;

            }

        }

        else{ // max heap

            // if the left child isnt null and its priority is greater than the parent's...
            // or the right child isnt null and its priority is greater than the parent's...
            if((heap->m_left != nullptr && priority(heap->m_left->m_patient) > priority(heap->m_patient)) || 
               (heap->m_right != nullptr && priority(heap->m_right->m_patient) > priority(heap->m_patient))){

                // return false
                return false;

            }

        }

            // recursively call the integrity function 
            return heapIntegrity(heap->m_left, heapType, priority) && heapIntegrity(heap->m_right, heapType, priority);

    }
/***************************************************************/
/***************************************************************/
    //helper to build an object of PQueue
    PQueue pqueueMaker(HEAPTYPE heapType, STRUCTURE structure, prifn_t priority, int size){

        Random nameGen(0,NUMNAMES-1);
        Random temperatureGen(MINTEMP,MAXTEMP);
        Random oxygenGen(MINOX,MAXOX);
        Random respiratoryGen(MINRR,MAXRR);
        Random bloodPressureGen(MINBP,MAXBP);
        Random nurseOpinionGen(MINOPINION,MAXOPINION);
        PQueue aQueue(priority, heapType, structure);
        for (int i=0;i<size;i++){                                       // code given to us that I reused
            Patient patient(nameDB[nameGen.getRandNum()],
                      temperatureGen.getRandNum(),
                      oxygenGen.getRandNum(),
                      respiratoryGen.getRandNum(),
                      bloodPressureGen.getRandNum(),
                      nurseOpinionGen.getRandNum());
            aQueue.insertPatient(patient);
        }

        return aQueue;

    }
/***************************************************************/
/***************************************************************/
bool nplCheck(Node* heap){ // checks if the npl value is correct

    // if the heap is not null 
    if(heap != nullptr){

        // if the heaps npl is correct  and the heap != nullptr
        if (heap->m_npl == nplFinder(heap) && heap != nullptr){

            //return the npl check of the left and right child
            return nplCheck(heap->m_left) && nplCheck(heap->m_right);

        }

        else {

            return false;

        }
    }

    return true;

}
/***************************************************************/
/***************************************************************/
int nplFinder(Node* node){

    // if the node is nullptr
    if (node == nullptr){

        return 0;

    }
 
    // if the node's children both dont exist
    else if((node->m_left == nullptr) or (node->m_right == nullptr)){
        
        return 0;

    }

    else{

        int leftH = 1 + nplFinder(node->m_left); // go down the entire left side of the tree if it exists and then everytime you come back add one to the height of the previous node
        int rightH = 1 + nplFinder(node->m_right);// go down the entire right side of the tree if it exists and then everytime you come back add one to the height of the previous node

        // Update the npl of the current node
        if(leftH < rightH){ // return the largest height

            return leftH;

        }

        else{

            return rightH;

        }

    }
}
/***************************************************************/


};

int main(){
    Tester tester;
    Random nameGen(0,NUMNAMES-1);
    Random temperatureGen(MINTEMP,MAXTEMP);
    Random oxygenGen(MINOX,MAXOX);
    Random respiratoryGen(MINRR,MAXRR);
    Random bloodPressureGen(MINBP,MAXBP);
    Random nurseOpinionGen(MINOPINION,MAXOPINION);
    PQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
    for (int i=0;i<10;i++){
        Patient patient(nameDB[nameGen.getRandNum()],
                    temperatureGen.getRandNum(),
                    oxygenGen.getRandNum(),
                    respiratoryGen.getRandNum(),
                    bloodPressureGen.getRandNum(),
                    nurseOpinionGen.getRandNum());
        aQueue.insertPatient(patient);
    }
    cout << "\nDump of the leftist heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    aQueue.setStructure(SKEW);
    cout << "\nDump of the skew heap queue with priorityFn2 (MINHEAP):\n";
    aQueue.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn2 (MINHEAP):\n";
    aQueue.printPatientQueue();
    aQueue.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the skew queue with priorityFn1 (MAXHEAP):\n";
    aQueue.dump(); 
    cout << endl; // onto non-driver tests

    
    //testing a min normal case
    if(tester.minTest()){cout << "minTest: PASSED" << endl;}
    else{cout << "minTest: FAILED" << endl;}

    //testing the max case
    if(tester.maxTest()){cout << "maxTest: PASSED" << endl;}
    else{cout << "maxTest: FAILED" << endl;}

    //testing the remove function at the min case
    if(tester.removeMinTest()){cout << "removeMinTest: PASSED" << endl;}
    else{cout << "removeMinTest: FAILED" << endl;}

    //testing the remove function at the min case
    if(tester.removeMaxTest()){cout << "removeMaxTest: PASSED" << endl;}
    else{cout << "removeMaxTest: FAILED" << endl;}

    //testing if the npl value hold true throughout the leftist tree
    if(tester.nplTest()){cout << "nplTest: PASSED" << endl;}
    else{cout << "nplTest: FAILED" << endl;}

    //tests if the leftist trees properties are held
    if(tester.leftistTest()){cout << "leftistTest: PASSED" << endl;}
    else{cout << "leftistTest: FAILED" << endl;}

    //tests if the heap queue properties are kept with a priority chaange
    if(tester.priorChangeTest()){cout << "priorChangeTest: PASSED" << endl;}
    else{cout << "priorChangeTest: FAILED" << endl;}

    //tests if the null merge case is passed
    if(tester.nullMergeTest()){cout << "nullMergeTest: PASSED" << endl;}
    else{cout << "nullMergeTest: FAILED" << endl;}

    // test if the copy constructor works
    if(tester.copyTest()){cout << "copyTest: PASSED" << endl;}
    else{cout << "copyTest: FAILED" << endl;}

    // tests if the copy edge passes
    if(tester.copyEdgeTest()){cout << "copyEdgeTest: PASSED" << endl;}
    else{cout << "copyEdgeTest: FAILED" << endl;}

    //tests if the assignment op work 
    if(tester.assignmentTest()){cout << "assignmentTest: PASSED" << endl;}
    else{cout << "assignmentTest: FAILED" << endl;}

    //assingment edge case
    if(tester.assignmentEdgeTest()){cout << "assignmentEdgeTest: PASSED" << endl;}
    else{cout << "assignmentEdgeTest: FAILED" << endl;}

    //should throw an out of range error
    //tester.dequeueTest();

    //should throw an error
    //tester.diffPriorTest();

    cout << endl;
    return 0;
}

int priorityFn1(const Patient & patient) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [115-242]
    //temperature + respiratory + blood pressure
    //the highest priority would be 42+40+160 = 242
    //the lowest priority would be 35+10+70 = 115
    //the larger value means the higher priority
    int priority = patient.getTemperature() + patient.getRR() + patient.getBP();
    return priority;
}

int priorityFn2(const Patient & patient) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [71-111]
    //nurse opinion + oxygen
    //the highest priority would be 1+70 = 71
    //the lowest priority would be 10+101 = 111
    //the smaller value means the higher priority
    int priority = patient.getOpinion() + patient.getOxygen();
    return priority;
}