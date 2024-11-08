// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"
PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{

  // initialize the members of PQueue
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;

}

PQueue::~PQueue()
{
  
  ClearHelper(m_heap); // calls the clear helper helper
  delete m_heap;       // delete the head
  m_heap = nullptr;    // set m_head to nullptr

}

void PQueue::clear() {

  ClearHelper(m_heap);   // calls clear helper
  delete m_heap;         // deletes the m_heap pointer
  m_heap = nullptr;      // sets m_heap to nullptr
    
}

PQueue::PQueue(const PQueue& rhs)
{
  
  m_heap = nullptr;                     // sets m_head ot nullptr
  heapCopyHelper(m_heap, rhs.m_heap);   // calls the headCopyHelper function with the m_heap and rhs's m_heap
  m_size = rhs.m_size;                  // sets the m_size to rhs's m_size
  m_priorFunc = rhs.m_priorFunc;        // sets the priority function = rhs's priority function
  m_heapType = rhs.m_heapType;          // sets m_heapType = rhs's m_heapType
  m_structure = rhs.m_structure;        // m_structure = rhs's m_structure

}

PQueue& PQueue::operator=(const PQueue& rhs) {

  m_heap = nullptr;                       // sets m_head to nullptr
  heapCopyHelper(m_heap, rhs.m_heap);     // heapCopyHelper the OG heap and the rhs's mheap
  m_size = rhs.m_size;                    // sets m_size = rhs's m_size
  m_priorFunc = rhs.m_priorFunc;          // sets priority function = rhs's
  m_heapType = rhs.m_heapType;            // m_heapType = rhs's
  m_structure = rhs.m_structure;          // sets m_structure = rhs's

  return *this;                           // returns the origional tree

}


void PQueue::mergeWithQueue(PQueue& rhs) {

  // checks if the m_heap is null
  if(rhs.m_heap != nullptr){ 

    // checks if the structure heaptpye priority functoin are the same
    if(m_structure == rhs.m_structure && m_heapType == rhs.m_heapType && m_priorFunc == rhs.m_priorFunc){

      // if the heaptype is max
      if(m_heapType == MAXHEAP){

        // set the heap = the merge of the two heaps and call the merge helper (which will call the skew or leftist forms)
        m_heap = MaxMergeHelper(m_heap, rhs.m_heap);

      }
      else{

        // set the heap = the merge of the two heaps and call the merge helper (which will call the skew or leftist forms)
        m_heap = MinMergeHelper(m_heap, rhs.m_heap);

      }

    }

    // if the heap is null call a domain error
    else{throw domain_error("Queues arent same type");}

  }

}


void PQueue::insertPatient(const Patient& patient) {

  //create a patient node to insert
  Node* newPatient = new Node(patient);
    
  // if the heap type is max
  if(m_heapType == MAXHEAP){

    // set the heap = the merge of the two heaps and call the merge helper (which will call the skew or leftist forms)
    m_heap = MaxMergeHelper(m_heap, newPatient);

  }
  else{

    // set the heap = the merge of the two heaps and call the merge helper (which will call the skew or leftist forms)
    m_heap = MinMergeHelper(m_heap, newPatient);

  }

  //dump();

  m_size++; // increment the size
}

int PQueue::numPatients() const{

  return m_size; // returns the number of patients
  
}

prifn_t PQueue::getPriorityFn() const {

  return m_priorFunc; // returns the priority function
  
}

Patient PQueue::getNextPatient() {

  if (m_heap != nullptr){

    m_size--; // decrement the size

    Patient tempP = m_heap->getPatient();  // cretae a temp patient

    Node* left = m_heap->m_left;  // set children pointers and temp heap head pointer
    Node* right = m_heap->m_right;  
    
    delete m_heap; // delete the head
    m_heap = nullptr; // set the head = nullptr
    
    if (m_heapType == MINHEAP){ // if the heaptype is minheap

      m_heap = MinMergeHelper(left, right);  // merge the two remaining trees

    }
    else { // else

      // merge the two remaining trees in the max
      m_heap = MaxMergeHelper(left, right);

    }   

    return tempP; // return the temp patient

  }

  //throw an exception if the queue is out of range
  else {throw out_of_range("queue is empty");} 

}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {

  PQueue tempQueue(*this);                              // create copy of tree
  clear();                                              // delete contents of OG
  m_priorFunc = priFn;                                  // switch priority member variable of OG 
  m_heapType = heapType;                                // switch heaptype
  while(tempQueue.m_heap != nullptr){                   //insert into the new tree

    try{                                                // try this...
      insertPatient(tempQueue.getNextPatient());        // insert the temp queues highest priority
    }

    catch(out_of_range& e){                             // catch the error if thats the case
      cout << "out of range error" << endl;             // output the error
      return;
    }
  
  }   
}

void PQueue::setStructure(STRUCTURE structure){

  PQueue tempQueue(*this);                                // create copy of tree
  clear();                                                // delete contents of OG
  m_structure = structure;                                // switch structure member variable of OG 
  while(tempQueue.m_heap != nullptr){                     //insert into the new tree

    try{                                                  // try the functions 
      insertPatient(tempQueue.getNextPatient());          // insert the tempQueue's next patient
    }

    catch(out_of_range& e){                               // catch the out of range error
      cout << "value is out of range" << endl;
      return;   
    }

  }
}

STRUCTURE PQueue::getStructure() const {

  return m_structure; // get the m_structure
  
}
void PQueue::printPatientQueue() const {

  printPatientQueueHelper(m_heap);  // call the patient queue helper

}

//dumps the queue contents
void PQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}

//pass the queue contents
void PQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
    else
        cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

//overloaded << op for patients
ostream& operator<<(ostream& sout, const Patient& patient) {
  sout  << patient.getPatient() 
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
  return sout;
}

//overloaded << op for nodes
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getPatient();
  return sout;
}

//for help with merging two heaps
Node* PQueue::MinMergeHelper(Node* LH, Node* RH){

  // if LH is null then RH is the new subrtree so return it
  if (LH == nullptr){return RH;} 

  // opposite of above
  else if (RH == nullptr){return LH;} 
  
  // if the priority value of the left is less than the right... 
  else if(m_priorFunc(LH->m_patient) < m_priorFunc(RH->m_patient)){ 
    
    if (getStructure() == SKEW){ // and the structure type is skew

      return MinSkewMergeHelper1(LH, RH); // call the min skew merge helper
    
    } // min skew
    
    else{

      return MinLeftistMergeHelper1(LH, RH); // if its not a skew type but the left heap has a higher priority for the heaptype return leftist of the merge

    } // min leftist

    } 
  
  else {

    // if the structure recieved is skew but the left heaps priority value is greater than the right
    if(getStructure() == SKEW){

      // return the merge of the right and left heap 
      return MinSkewMergeHelper1(RH, LH);
    
    } // min skew

    else{

      // return the merge of the right and left heap as a leftist
      return MinLeftistMergeHelper1(RH, LH);
    
    } // min leftist
  }

}

Node* PQueue::MaxMergeHelper(Node* LH, Node* RH){

  // if LH is null then RH is the new subrtree so return it
  if (LH == nullptr){return RH;} 

  // opposite of above
  else if (RH == nullptr){return LH;} 
  
  // if the priority value of the left is less than the right... 
  else if(m_priorFunc(LH->m_patient) > m_priorFunc(RH->m_patient)){ 
    
    // if the structure is a skew
    if (getStructure() == SKEW){

      // return the max skew merge of the left heap and right heap
      return MaxSkewMergeHelper1(LH, RH); 
    
    } // max skew
    
    else{

      // return the max leftist merge of the left heap and right heap
      return MaxLeftistMergeHelper1(LH, RH); 

    } // max leftist

    } 
  
  else {

    // if the structure type is skew but the lh is less than rh...
    if(getStructure() == SKEW){ 
      
      // merge the right heap and left heap
      return MaxSkewMergeHelper1(RH, LH); 
    
    } // max skew

    else{
      
      //merge the right heap and left as a leftist 
      return MaxLeftistMergeHelper1(RH, LH);  
    
    } // max leftist
  }

}

Node* PQueue::MinLeftistMergeHelper1(Node* LH, Node* RH){

  // if the left pointer's right child is null... return the right child as the other heap passed 
  if(LH->m_left == nullptr){
    
    LH->m_left = RH;
    
    }

  // if the LH's left child isnt null isnt null
  else{

    // set the right child = the merge of the two 
    LH->m_right = MinMergeHelper(LH->m_right, RH);

    // if the left child's npl value is less than the right
    if (LH->m_left->m_npl < LH->m_right->m_npl){

      // swap the children  
      ChildSwap(LH);
    
    }

    // set the npl value of the right child
    LH->m_npl = nplHelper(LH);

  }

  // increment the size value
  m_size++;
  return LH;

}

Node* PQueue::MaxLeftistMergeHelper1(Node* LH, Node* RH){

  // if the left pointer's right child is null... return the right child as the other heap passed
  if(LH->m_left == nullptr){
    
    LH->m_left = RH;
    
    }

  // if the LH's left child isnt null isnt null
  else{

    // set the right child = the merge of the two 
    LH->m_right = MaxMergeHelper(LH->m_right, RH);

    // if the left child's npl value is less than the right
    if (LH->m_left->m_npl > LH->m_right->m_npl){
    
      // swap the children  
      ChildSwap(LH);
    
    }

    // set the npl value of the right child
    LH->m_npl = nplHelper(LH);

  }

  // increment the size
  m_size++;
  return LH;

}

Node* PQueue::MaxSkewMergeHelper1(Node* LH, Node* RH){

  // if the left child of lh is null return rh 
  if(LH->m_left == nullptr){LH->m_right = RH;}

  else{

    //swap the children
    ChildSwap(LH);
    // set the right child = the merge
    LH->m_right = MaxMergeHelper(LH->m_right, RH);
  
  }

  // increment the size
  m_size++;
  return LH;

}

Node* PQueue::MinSkewMergeHelper1(Node* LH, Node* RH){

  // if the left child of lh is null return rh 
  if(LH->m_left == nullptr){LH->m_left = RH;}

  else{

    //swap the children
    ChildSwap(LH);
    // set the right child = the merge
    LH->m_right = MinMergeHelper(LH->m_right, RH);
  
  }

  //increment the size of the heap
  m_size++;
  return LH;

}

void PQueue::ChildSwap(Node*& heap){

  Node* temp = heap->m_left;        //set a temp node = the left child 
  heap->m_left = heap->m_right;     // ser the left node == the right one
  heap->m_right = temp;             // set the right node == the temp which is == the old left

}

int PQueue::nplHelper(Node* node){

    // if the node is null
    if (node == nullptr){

        return 0;

    }

    // if the left and the right node are null
    else if((node->m_left == nullptr) and (node->m_right == nullptr)){
        
        return 0;

    }

    // if either child node exists
    else{

        int leftH = 1 + nplHelper(node->m_left); // go down the entire left side of the tree if it exists and then everytime you come back add one to the height of the previous node
        int rightH = 1 + nplHelper(node->m_right);// go down the entire right side of the tree if it exists and then everytime you come back add one to the height of the previous node

        // Update the npl of the current node
        if(leftH < rightH){ // return the largest height

            return leftH;

        }

        else{

            return rightH;

        }

    }

}

void PQueue::ClearHelper(Node*& node){

    if (node != nullptr){ //if node exists

        ClearHelper(node->m_left); // recursively call down the left side of the tree
        delete node->m_left; // once at the bottom delete the bottom left nodew
        ClearHelper(node->m_right); // go to right if you can
        delete node->m_right; // delete right if not

    }
}

void PQueue::heapCopyHelper(Node*& heap, const Node* rhs) {

  // if the heap == null and the rhs == nullptr
  if (heap == nullptr && rhs != nullptr){
    
    //create a new node of the rhs patient
    heap = new Node(rhs->m_patient);
    
    // set the heap npl values =
    heap->m_npl = rhs->m_npl;
    
    heapCopyHelper(heap->m_left, rhs->m_left); // left recursive call
    heapCopyHelper(heap->m_right, rhs->m_right); // right recursice call

  }

}

void PQueue::printPatientQueueHelper(const Node* heap)const{

  // if the heap passed exists
  if(heap != nullptr){

    // cout the nodes priority and the parient information
    cout << "[" << m_priorFunc(heap->m_patient) << "] " << heap->m_patient << endl;
    // recursive call the left
    printPatientQueueHelper(heap->m_left);
    //recursive call to the right
    printPatientQueueHelper(heap->m_right);

  }

}