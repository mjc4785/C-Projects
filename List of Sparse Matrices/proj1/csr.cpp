/*
file: CSR.cpp
author: maxwell castle 
date: 10/3/23
section: 4
email: maxwelc2@umbc.edu
description: a class created for the creation of a linked list of matricies
*/
#include "csr.h"
///////////////CSR Class Implementation///////////////
CSR::CSR(){ // populates a CSR object without ovelroading

    m_values = nullptr;
    m_col_index = nullptr;
    m_row_index = nullptr;
    m_nonzeros = 0;
    m_m = 0;
    m_n = 0;
    m_next = nullptr;

}
CSR::~CSR(){ //deletion of dynamically allocated information in a object
    
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;

}
CSR::CSR(const CSR & rhs){ // copy constructor for object

    m_m = rhs.m_m;
    m_n = rhs.m_n;
    m_nonzeros = rhs.m_nonzeros;

    m_values = new int[m_nonzeros];
    m_row_index = new int [m_m + 1];
    m_col_index = new int [m_nonzeros];

    for (int i = 0; i<=m_m; i++){ // rows go slower

        m_row_index[i] = rhs.m_row_index[i];

    }
        
    for(int j = 0; j<m_nonzeros; j++){ // columns go quicker

        m_values[j] = rhs.m_values[j];
        m_col_index[j] = rhs.m_col_index[j];

    }


}
void CSR::clear(){ // deletes dynamically allocates information from object
    
    delete[] m_values;
    delete[] m_col_index;
    delete[] m_row_index;

}
bool CSR::empty() const{ // returns true if empty otherwise returns false

    if (m_nonzeros == 0){

        return true;

    }

    else {

        return false;

    }

}

void CSR::compress(int m, int n, int array[], int arraySize){ // compresses data into 
//form of three arrays from a matrix
    m_m = m; // rows
    m_n = n;// columns
    m_nonzeros = 0;
    int counter = 0;
    int rowCount = 0;
    int loopNum = 0;

    if (m_m > 0 && m_n > 0){

        if(arraySize < m_m*m_n){
            arraySize = m_m*m_n;
        }

        if(arraySize > 0){
            
            for (int i = 0; i<arraySize; i++){
            
                if(array[i] != 0){
                    
                    m_nonzeros++;
            
                }
            }

            m_values = new int[m_nonzeros];
            m_row_index = new int[m_m + 1];
            m_col_index = new int[m_nonzeros];

            for (int i = 0; i<m_m; i++){ // rows 

                m_row_index[i] = rowCount;

                for(int j = 0; j<m_n; j++){ // columns go quicker

                    if(array[j+loopNum] != 0){

                        m_values[counter] = array[j+loopNum];
                        m_col_index[counter] = j;
                        counter++;
                        rowCount++;

                    }
                    if (i == (m_m - 1)){

                        m_row_index[i+1] = rowCount;

                    }

                }

                loopNum += m_n;

            }   

        }
    }

}

int CSR::getAt(int row, int  col) const{ // get the information at parameters given
    if( ( row > m_m ) || ( row < 1 ) || ( col > m_n ) || ( col < 1 ) ) {
        throw runtime_error("Out of Bounds");
    }
    else{
        for (int i = m_row_index[row - 1]; i<=m_row_index[row]; i++){
            
            if (m_col_index[i] == col){

                return m_values[i];

            }
        }
    }
    
  /*  catch (runtime_error) {

        cout << "exception occured: " << endl;

    }
    */

    return 0;

} 
bool CSR::operator==(const CSR & rhs) const{ // checks to see if objects are equal

    bool flag = true;

/*
if (){

    flag = false;   

}
    */

    if(rhs.empty() && empty()){

        return true;

    }

    if(!rhs.empty() && !empty()){

        if (m_nonzeros != rhs.m_nonzeros){

            flag = false;
        
        }

        if (m_m != rhs.m_m){

            flag = false;

        }

        if (m_n != rhs.m_n){

            flag = false;

        }

        for (int i = 0; i<m_nonzeros; i++){

        if (m_values[i] != rhs.m_values[i]){

            flag = false;   

        }

        if (m_col_index[i] != rhs.m_col_index[i]){

            flag = false;   

        }

        }

        for (int i = 0; i<m_m+1; i++){

            if (m_row_index[i] != rhs.m_row_index[i]){

                flag = false;   

            }

        }
    }

    return flag;

}

int CSR::sparseRatio(){ // sees how much information from the data is zeros
    
    int zeros = (m_m*m_n) - m_nonzeros;
    int members = (m_m*m_n);

    int ratio = (zeros/members) * 100;

    return ratio;

}
void CSR::dump(){ // prints out the information from the 
    cout << endl;
    if (!empty()){
        for (int i=0;i<m_nonzeros;i++)
            cout << m_values[i] << " ";
        cout << endl;
        for (int i=0;i<m_nonzeros;i++)
            cout << m_col_index[i] << " ";
        cout << endl;
        for (int i=0;i<m_m+1;i++)
            cout << m_row_index[i] << " ";
    }
    else
        cout << "The object is empty!";
    cout << endl;
}





//////////////CSRList Class Implementation///////////////
CSRList::CSRList(){ // constructs data without parameters
    m_head = nullptr;
    m_size = 0;
}
CSRList::CSRList(const CSRList & rhs){ // copy constructor overloaded

    CSR* temp = m_head;

    if (rhs.m_size != 0){
        for (int j = rhs.m_size; j<0; j++){ // inserts the node in the right position using i

            for (int i = rhs.m_size; i>-1; i--){//gets temp to the right position}

                temp = temp->m_next;
            
            }

            insertAtHead(*temp);

        }

    }

}

CSRList::~CSRList(){ //deletes dynamically allocated
    m_head = nullptr;
}

bool CSRList::empty() const{ // if the CSR list is empty return true

    if(m_size == 0){

        return true;
    
    }
    
    else{
   
        return false;
   
    }
    
}

void CSRList::insertAtHead(const CSR & matrix){ // put the node passed at the front of thge list

    CSR* mCSR = new CSR(matrix);

     if(m_size == 0){

        m_head = mCSR;
        mCSR->m_next = nullptr;
        m_size++;

     }

     else{

        mCSR->m_next = m_head;
        m_head = mCSR;
        m_size++;
     }

    //dynamically allocate
    //if csr object is m_size == 0 make it the head
    //else add it to the begining 

}

void CSRList::clear(){ // deletes the information thats dynamically allocated

    CSR* temp = m_head;

    for (int i = 0 ; i<m_size ; i++){

        temp = temp->m_next;
        m_head->clear();
        m_head = temp;

    }

    m_head = nullptr;

}

int CSRList::getAt(int CSRIndex, int row, int col) const{
//gets information at passed parameters
    CSR* temp = m_head;

    if(CSRIndex > m_size) {// if the csrindex is too big or small
        
        throw runtime_error("Exception Error: Object is not in the list!");

    }

    for (int i = 0; i< CSRIndex; i++){ // gets temp to the csr object wanted 
     
        temp = temp->m_next;
    }

    if(col > temp->m_n || col < 1) {// if the column is too large or small
        
        throw runtime_error("Exception Error: Object is not in the list!");

    }

    if(row > temp->m_m || row < 1) {// if the row is too large or small
        
        throw runtime_error("Exception Error: Object is not in the list!");

    }

   for (int i = temp->m_row_index[row]; i<=temp->m_row_index[row+1]; i++){
            
        if (temp->m_col_index[i] == col){

            return temp->m_values[i];

        }
    }

    return 0;

}

bool CSRList::operator== (const CSRList & rhs) const{ 
// checks to see if the lists passed are equal
    
    bool flag = true;
    CSR* temp = m_head;
    CSR* rhsTemp = rhs.m_head;

    for (int i = 0; i<m_size ; i++){

        if (temp != rhsTemp){
            flag = false;
        }

        temp = temp->m_next;
        rhsTemp = rhsTemp->m_next;

    }

    return flag;

}

const CSRList& CSRList::operator=(const CSRList & rhs){ 
    // creates a copy of the object passed
       
    CSR* temp = m_head;
    CSR* real = rhs.m_head;

    if (rhs.m_size != 0){
        for (int j = rhs.m_size; j<0; j++){ // inserts the node in the right position using i

        cout << "in here, pussyboy" << endl;

            for (int i = rhs.m_size; i>0; i--){//gets temp to the right position}

                temp = temp->m_next;
                real = real->m_next;
            
            }

            temp = real;
            insertAtHead(*temp);

        }

    }

    return *this;

}

int CSRList::averageSparseRatio(){
    //checks to see how much of the data in the list is zeros

    int total = 0;
    CSR* temp = m_head;

    for (int i = 0; i<m_size; i++){
   
        total = total + temp->sparseRatio();
        temp = temp->m_next;
   
    }

    return total;

}
void CSRList::dump(){
    // prints out the data given 
    if (!empty()){
        CSR* temp = m_head;
        while (temp != nullptr){
            temp->dump();
            temp = temp->m_next;
        }
    }
    else
        cout << "Error: List is empty!" << endl;
}