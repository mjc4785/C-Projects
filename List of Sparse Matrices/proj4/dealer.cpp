// CMSC 341 - Fall 2023 - Project 4
/*
file: dealer.cpp
author: maxwell castle 
date: 12/5/23
section: 4
email: maxwelc2@umbc.edu
description: the cpp file for hashtable project
*/
#include "dealer.h"
CarDB::CarDB(int size, hash_fn hash, prob_t probing = DEFPOLCY){
    
    // current hash situation
    if (!isPrime(size)){                                // if the size is not a prime number
                                                        
        size = findNextPrime(size);                     // the size = the nect prime up

    }                   
    else if (size < MINPRIME){m_currentCap = MINPRIME;}      // if the size is less than the minimum, set it as the MINPRIME variable
    else if (size > MAXPRIME){m_currentCap = MAXPRIME;}      // if the size is greater than the maximum, set it as the MAXPRIME variable
    else{m_currentCap = size;}

    m_hash = hash;                                      // set the hash variable as the passed hash function
    m_newPolicy = probing;                              // set the probing function as the passed probing function
    m_currentTable = new Car[m_currentCap];             // set the current table to nullptr
    m_currentSize = 0;                                  // set the current size as zero
    m_currNumDeleted = 0;                               // zero deleted so far
    m_currProbing = probing;                            // set the probing function = to the passed function pointer.

    m_oldTable = nullptr;                               // nothing allocated to the old table
    m_oldCap = 0;                                       // setting the old cap of the size
    m_oldSize = 0;                                      // sets the old cap size 
    m_oldNumDeleted = 0;                                // sets the number of deleted from the previous table to 0 
    m_oldProbing = m_currProbing;                             // sets the old table's probing functoin to the current probing method. 

}

CarDB::~CarDB(){

    //deallocates, the data that was dynamically allocated.

    if (m_currentTable != nullptr) {    
        m_currentTable = nullptr;
    }
 
    if (m_oldTable != nullptr) {
        m_oldTable = nullptr;
    }
    
}

void CarDB::changeProbPolicy(prob_t policy){

    m_newPolicy = policy;  // sets the porbing policy to the parameter policy
    
}

bool CarDB::insert(Car car){

    int i = 0;  // iterator
    int index;  // the index to calculate
    float ratio = 0.5;  // ratio of the 

    if (m_currProbing == QUADRATIC){ // quadratic probing
                  
        do {

            index = ((m_hash(car.m_model) % m_currentCap) + (i * i)) % m_currentCap;
            i++;
        
        }
        while(!m_currentTable[index].m_model.empty() && i <= m_currentCap); // while the current table isnt empty and we havent iterated throught the entire table... do the above

        if(i == m_currentCap){return false;}    // if the cap was reached in iterating... return false
        m_currentTable[index] = car;    // set the current tables index found to the car passed
        m_currentSize++;    
        if(lambda() >= ratio || m_oldTable != nullptr){ // if the load factor is over 0.5... rehash
            
            rehash(*this);
            
            }
        return true;
            
    } // helper fnuction for quadratic probing
        
    else if (m_currProbing == DOUBLEHASH){ // double hash probing
            
        do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

            index = ((m_hash(car.m_model) % m_currentCap) + i * (11-(m_hash(car.m_model) % 11))) % m_currentCap;
            i++;
        
        }
        while(!m_currentTable[index].m_model.empty() && i <= m_currentCap);

        // if(i == m_currentCap){return false;}        
        m_currentTable[index] = car;
        m_currentSize++;
        if(lambda() >= ratio || m_oldTable != nullptr){
            
            rehash(*this);
            
            }
        return true;
            
    } // helper for double probin 
        
    else { // normal linear probing

        do {

            index = (m_hash(car.m_model) + i) % m_currentCap;
            i++;
        
        }
        while(!(m_currentTable[index] == EMPTY) && i <= m_currentCap);

        if(i == m_currentCap){return false;}
        m_currentTable[index] = car;
        m_currentSize++;
        if(lambda() >= ratio || m_oldTable != nullptr){
            
            rehash(*this);
            
            }
        return true;

    }

    return false;
}         

bool CarDB::remove(Car car){

    // hashes the table until dealer and model matches. when they do, remove
    //  to the parameter quantity and return true

    int i = 0;  // iterator
    int index = 0;  // the calculated index's variable
    float ratio = 0.8;  // the deletion ration variable

    if (m_currProbing == QUADRATIC){ // quadratic probing
                  
        do {

            index = ((m_hash(car.m_model) % m_currentCap) + (i * i)) % m_currentCap;
            i++;
        
        }
        while(m_currentTable[index].m_dealer != car.m_dealer && m_currentTable[index].m_model != car.m_model && i <= m_currentCap); 
        // while the dealer doesnt match th dealer, the model doesnt match the model and the iterator is less than the cap of the table...
            
    } // helper fnuction for quadratic probing
        
    else if (m_currProbing == DOUBLEHASH){ // double hash probing
            
        do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

            index = ((m_hash(car.m_model) % m_currentCap) + i * (11-(m_hash(car.m_model) % 11))) % m_currentCap;
            i++;
        
        }
        while(m_currentTable[index].m_dealer != car.m_dealer && m_currentTable[index].m_model != car.m_model && i <= m_currentCap);
            
    } // helper for double probin 
        
    else { // normal linear probing

        do {

            index = m_hash(car.m_model) + i;
            i++;
        
        }
        while(m_currentTable[index].m_dealer != car.m_dealer && m_currentTable[index].m_model != car.m_model && i <= m_currentCap);

    }

    //////
    // old table 
    //////

    if(i == m_currentCap){ // if the data isnt found in the current table, find it in the old one
        
        i = 0; // reset the iterator 
            
        if (m_oldProbing == QUADRATIC){ // quadratic probing
                    
            do {

                index = ((m_hash(car.m_model) % m_oldCap) + (i * i)) % m_oldCap;
                i++;
            
            }
            while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldCap); 
            // while the dealer doesnt match the dealer, the model doesnt match the model and the iterator is less than the maximum
                
        } // helper fnuction for quadratic probing
            
        else if (m_oldProbing == DOUBLEHASH){ // double hash probing
                
            do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

                index = ((m_hash(car.m_model) % m_oldCap) + i * (11-(m_hash(car.m_model) % 11))) % m_oldCap;
                i++;
            
            }
            while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldCap);
                
        } // helper for double probin 
            
        else { // normal linear probing

            do {

                index = m_hash(car.m_model) + i;
                i++;
            
            }
            while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldCap);

        }

        if(i == m_oldCap){return false;} // if the iterator searched through the whole table return false
        m_oldTable[index] = EMPTY; // set the index that we want = to null
        m_oldNumDeleted++; // increase the umber of deleted 
        // checked the deleted ratio. if the ratio is > 0.8 or the old table exists, rehash the table
        if(deletedRatio() >= ratio || m_oldTable != nullptr){
            
            rehash(*this);
            
            }
        return true;
        
    }

    // if the object is found in the table
    m_currentTable[index] = EMPTY; // set the index = to empty
    m_currNumDeleted++; // increase the number of deleted
    if(deletedRatio() >= ratio || m_oldTable != nullptr){ // and if the deleted ration is too large or the old table exists... rehash
            
            rehash(*this);
            
    }
    return true;

}

Car CarDB::getCar(string model, int dealer) const{

    int index;
    int i = 0;

    if (m_currProbing == QUADRATIC){ 
    
        do{
        
            index = ((m_hash(model) % m_currentCap) + (i * i)) % m_currentCap;
            i++;

        }
        while((m_currentTable[index].m_dealer != dealer || m_currentTable[index].m_model != model) && i <= m_currentCap);
        // while the dealer doesnt equal the dealer or the model doesnt match the model and the iterator hasntl looked through the whole table...

    } // quad index



    else if(m_currProbing == DOUBLEHASH){

        do{

            index = ((m_hash(model) % m_currentCap) + i * (11-(m_hash(model) % 11))) % m_currentCap;
            i++;
            // cout << index << ": " << m_currentTable[index] << endl;

        }
        while((m_currentTable[index].m_dealer != dealer || m_currentTable[index].m_model != model) && i <= m_currentCap);

    } // double index
    


    else{

        do{

            index = (m_hash(model) + i) % m_currentCap;
            i++;

        }
        while((m_currentTable[index].m_dealer != dealer || m_currentTable[index].m_model != model) && i <= m_currentCap);

    } // neither index

    //////
    //old table
    //////

    if(i == m_currentCap){

        i = 0;
        
        if (m_oldProbing == QUADRATIC){
    
            do{
            
                index = ((m_hash(model) % m_oldCap) + (i * i)) % m_oldCap;
                i++;

            }
            while((m_oldTable[index].m_dealer != dealer || m_oldTable[index].m_model != model) && i <= m_oldCap);

        } // quad index



        else if(m_oldProbing == DOUBLEHASH){

            do{

                index = ((m_hash(model) % m_oldCap) + i * (11-(m_hash(model) % 11))) % m_oldCap;
                i++;

            }
            while((m_oldTable[index].m_dealer != dealer || m_oldTable[index].m_model != model) && i <= m_oldCap);

        } // double index
    


        else{

            do{

                index = m_hash(model) + i;
                i++;

            }
            while((m_oldTable[index].m_dealer != dealer || m_oldTable[index].m_model != model) && i <= m_oldCap);

        } // neither index

        if(i == m_oldCap){return EMPTY;}
    
    }

    return m_currentTable[index];

}

float CarDB::lambda() const {

    float size = float(m_currentSize); // float the currsize of the the table
    float cap = float(m_currentCap); // float the current cap of the table
    return (size / cap); // return the dividend of the two 

}

float CarDB::deletedRatio() const {

    float deleted = float(m_currNumDeleted); // float the numebr of deleted
    float size = float(m_currentSize); // float current size of the table
    return (deleted / size); // dividend between the deleted and the size

}

void CarDB::dump() const {
    cout << "Dump for the current table: " << endl;
    if (m_currentTable != nullptr)
        for (int i = 0; i < m_currentCap; i++) {
            cout << "[" << i << "] : " << m_currentTable[i] << endl;
        }
    cout << "Dump for the old table: " << endl;
    if (m_oldTable != nullptr)
        for (int i = 0; i < m_oldCap; i++) {
            cout << "[" << i << "] : " << m_oldTable[i] << endl;
        }
}

bool CarDB::updateQuantity(Car car, int quantity){

    int oldIndex = oldCarFinder(car); // the index of the car if its iin the old table
    int currIndex = currCarFinder(car); // the index of the car if its in the current table
    int NOTFOUND = -1; // if the car isnt found, the index will be -1 

    if (currIndex == NOTFOUND && oldIndex == NOTFOUND){ // if eithe rof the indexes are not found

        return false; // return false
    
    }
    else {

        if (oldIndex == NOTFOUND){ // if the current index is found, update the quantity in there

            m_currentTable[currIndex].m_quantity = quantity;
            return true;

        }
        else { // if the old index is found, update the old quantity

            m_oldTable[oldIndex].m_quantity = quantity;
            return true;

        }

    }

    return false;

}

bool CarDB::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int CarDB::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}

ostream& operator<<(ostream& sout, const Car &car ) {
    if (!car.m_model.empty())
        sout << car.m_model << " (" << car.m_dealer << "," << car.m_quantity<< ")";
    else
        sout << "";
  return sout;
}

bool operator==(const Car& lhs, const Car& rhs){
    // since the uniqueness of an object is defined by model and delaer
    // the equality operator considers only those two criteria
    return ((lhs.m_model == rhs.m_model) && (lhs.m_dealer == rhs.m_dealer));
}

CarDB CarDB::rehash(CarDB& car){

    int percent = 0; // the percent of the data transfered over

    // sets all the old table variables = to the new table variables
    if (m_oldTable == nullptr){

        m_oldTable = m_currentTable;
        m_oldCap = m_currentCap;
        m_oldSize = m_currentSize;
        m_oldNumDeleted = 0;
        m_oldProbing = m_currProbing;

        m_currentCap = findNextPrime((m_currentSize - m_currNumDeleted) * 4);
        m_currentTable = new Car[m_currentCap];
        m_currentSize = 0;
        m_currNumDeleted = 0;

        if (m_newPolicy != NONE) {
            m_currProbing = m_newPolicy;
        }

    }

    // if the size of the old table != m_oldDeleted, loop
    if(m_oldSize != m_oldNumDeleted){

        // for the length 1/4th of the tabls size, loop.
        
        for (int i = 0; i < m_oldCap; i++){

            // if the current table at the index isnt empty
            if (!m_oldTable[i].m_model.empty()){

                // if it inserts
                if(helperTransfer(m_oldTable[i])){    // insert the car at the index in the current table
                
                    m_oldTable[i] = EMPTY;                   // set the old tables index to empty
                    m_oldNumDeleted++;
                    percent++;
                    if(percent >= (m_oldSize/4)){  

                        return car;
                        
                        }

                    if (m_oldSize == m_oldNumDeleted){ // delete the old table if it runs out

                        if (m_oldTable != nullptr) {
                            delete [] m_oldTable;
                            m_oldTable = nullptr;
                        }

                        // nothing allocated to the old table
                        m_oldCap = 0;                                       // setting the old cap of the size
                        m_oldSize = 0;                                      // sets the old cap size 
                        m_oldNumDeleted = 0;                                // sets the number of deleted from the previous table to 0 
                        m_oldProbing = m_currProbing;    

                    }
                
                }
                // only iterate if a car is moved over so that you truly get 25%
                
            }

        }

        //while (percent <= (m_oldSize/4) && m_oldNumDeleted != m_oldSize);
            
    }
    else{

        if (m_oldTable != nullptr) {
            delete [] m_oldTable;
            m_oldTable = nullptr;
        }

        // nothing allocated to the old table
        m_oldCap = 0;                                       // setting the old cap of the size
        m_oldSize = 0;                                      // sets the old cap size 
        m_oldNumDeleted = 0;                                // sets the number of deleted from the previous table to 0 
        m_oldProbing = m_currProbing;                       // sets the old table's probing functoin to the current probing method. 

    }

    return car;
 
}

// returns the index of the car if it is in the current table
// retuns -1 if it doesnt exist in the  current table
int CarDB::currCarFinder(Car car){

    // hashes the table until dealer and model matches. when they do, remove
    //  to the parameter quantity and return true

    int i = 0;
    int index = -1;

    if (m_currProbing == QUADRATIC){ // quadratic probing
                  
        do {

            index = ((m_hash(car.m_model) % m_currentCap) + (i * i)) % m_currentCap;
            i++;
        
        }
        while((m_currentTable[index].m_dealer != car.m_dealer || m_currentTable[index].m_model != car.m_model) && i <= m_currentCap);

            
    } // helper fnuction for quadratic probing
        
    else if (m_currProbing == DOUBLEHASH){ // double hash probing
            
        do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

            index = ((m_hash(car.m_model) % m_currentCap) + i * (11-(m_hash(car.m_model) % 11))) % m_currentCap;
            i++;
        
        }
        while((m_currentTable[index].m_dealer != car.m_dealer && m_currentTable[index].m_model != car.m_model) && i <= m_currentCap);

            
    } // helper for double probin 
        
    else { // normal linear probing

        do {

            index = m_hash(car.m_model) + i;
            i++;
        
        }
        while((m_currentTable[index].m_dealer != car.m_dealer || m_currentTable[index].m_model != car.m_model) && i <= m_currentCap);


    }

    return index;


}

// returns the index of the car if it is in the old table
// retuns -1 if it doesnt exist in the old table
int CarDB::oldCarFinder(Car car ){// if this returns -1 it didn't find hte car

    // hashes the table until dealer and model matches. when they do, remove
    //  to the parameter quantity and return true

    int i = 0;
    int index = -1;

    if (m_oldProbing == QUADRATIC){ // quadratic probing
                  
        do {

            index = ((m_hash(car.m_model) % m_oldCap) + (i * i)) % m_oldCap;
            i++;
        
        }
        while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldSize);

            
    } // helper fnuction for quadratic probing
        
    else if (m_oldProbing == DOUBLEHASH){ // double hash probing
            
        do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

            index = ((m_hash(car.m_model) % m_oldCap) + i * (11-(m_hash(car.m_model) % 11))) % m_oldCap;
            i++;
        
        }
        while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldSize);

            
    } // helper for double probin 
        
    else { // normal linear probing

        do {

            index = m_hash(car.m_model) + i;
            i++;
        
        }
        while(m_oldTable[index].m_dealer != car.m_dealer && m_oldTable[index].m_model != car.m_model && i <= m_oldSize);


    }

    return index;

}

// identicle to the insert function except it doesnt chekc the load factor or delete ratio 
bool CarDB::helperTransfer(Car car){

    int i = 0;
    int index;

    if (m_currProbing == QUADRATIC){ // quadratic probing
                  
        do {

            index = ((m_hash(car.m_model) % m_currentCap) + (i * i)) % m_currentCap;
            i++;
        
        }
        while(!m_currentTable[index].m_model.empty() && i <= m_currentCap);

        m_currentTable[index] = car;
        m_currentSize++;
        return true;
            
    } // helper fnuction for quadratic probing
        
    else if (m_currProbing == DOUBLEHASH){ // double hash probing
            
        do { //  index = ((Hash(key) % TableSize) + i x (11-(Hash(key) % 11))) % TableSize

            index = ((m_hash(car.m_model) % m_currentCap) + i * (11-(m_hash(car.m_model) % 11))) % m_currentCap;
            i++;
        
        }
        while(!m_currentTable[index].m_model.empty() && i <= m_currentCap);
    
        m_currentTable[index] = car;
        m_currentSize++;
        return true;
            
    } // helper for double probin 
        
    else { // normal linear probing

        do {

            index = m_hash(car.m_model) + i;
            i++;
        
        }
        while(!m_currentTable[index].m_model.empty() && i <= m_currentCap);

        m_currentTable[index] = car;
        m_currentSize++;
        return true;

    }

    return false;

}