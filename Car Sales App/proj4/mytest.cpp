// CMSC 341 - Fall 2023 - Project 4
/*
file: mytest.cpp
author: maxwell castle 
date: 12/5/23
section: 4
email: maxwelc2@umbc.edu
description: the tester file for hash table project 
*/
#include "dealer.h"
#include <random>
#include <vector>
#include <algorithm>
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

    string randomNames[19] = {"a", "as", "mark", "null", "open", "close", "eliotte", "smorth", "test", "number", "cort", "quart", "wince", "chance", "dance", "sleeper", "horns"};
    string carModels[5] = {"challenger", "stratos", "gt500", "miura", "x101"};
    string dealers[5] = {"super car", "mega car", "car world", "car joint", "shack of cars"};

    /*testing a normal doublehash insertion*/
    bool InsertionNormalDoublehash(){

        //will create and check the completion of a doublehash function

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
        return result;
            
    }

    bool InsertionNormalQuad(){

        // will create and check the completion of the doublehash functoin

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, QUADRATIC);
        bool result = true;
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
        return result;
            
    }

    bool InsertionNormal(){

        //will create and check the completion of no probe type functoin

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, NONE);
        bool result = true;
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
        return result;
            
    }

    /*testing a doublehash find error*/
    bool FindError(){

        // creates a table and trys to find an object that isnt in the table

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<48;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        Car errorCar = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);

        // creates an objeet that is not in the array to try and find        

        // checking whether data isnt there
        result = (errorCar == cardb.getCar((errorCar).getModel(), (errorCar).getDealer()));

        if (!result) // if it is not found, itll return true. meaning the find functoin even works on failures
            return true;
        else
            return false;
        
    }
    
    /*make a car DB object with a bunch of non coliding keys*/
    bool FindNonCollidingKeys(){

        // will find keys even if they are not colliding in the hash table

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<18;i++){
            // generating random data
            Car dataObj = Car(randomNames[i], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // // for the size of the vector loop through and find the index. append it to a vector
        // //      and for every index in the vector, if it is = to any other index number
        // //      return false. otherwise return true.

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }

        return result;
        
    }
   
    /*make a car db pbject with colliding keys*/
    bool FindCollidingKeys(){

        // find keys even if they collide in the hash function

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
        return result;
    
    }
    
    /*removes objects that are colliding*/
    bool RemoveNonColliding(){

        // removes keys that are not colliding
        
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<4;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }

        //cardb.dump();

        if(cardb.remove(dataList.back())){

            dataList.pop_back();

        }
        else{

            result = false;

        }

        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }

        //cardb.dump();
        
        return result;
    
    }
    
    /**/
    bool RemoveColliding(){

        // removes keys even if they are colliding
        
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<4;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
        if(cardb.remove(dataList.back())){

            dataList.pop_back();

        }
        else{

            result = false;

        }

        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }

        return result;
        
    }
    
    /**/
    bool RehashAfterDataInsert(){
        
        // rehashes the data after the load factor exceeds 50%
        
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<53;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
            
        return result;
        
    }
    
    /**/
    bool RehashCompletion(){

        //completely rehashes successfully

        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<70;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }
        
            
        return result;
        
    }
    
    /**/
    bool RehashDataDescent(){return false;}
    
    /*rehashes using the remove function. if the delete ratio is > 80%*/
    bool RehashDataDescentCompletion(){
        
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        bool result = true;
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }    

        // checking whether all data points are inserted
        for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
            result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
        }

        for (int i = 0; i<47 ;i++){

            if(cardb.remove(dataList.back())){

                dataList.pop_back();

            }
            else{

                result = false;

            }

        }

        // checking whether all data points are inserted
        // for (unsigned int i = 0; i<dataList.size(); i++){

        //     cout << "in here" << endl;
        //     cout << cardb.getCar(dataList[i].getModel(), dataList[i].getDealer()) << endl;
        //     cout << dataList[i] << endl;

        //     result = result && (cardb.getCar(dataList[i].getModel(), dataList[i].getDealer()) == dataList[i]);

        // }
            
        return result;
        
    }

    /***************************************************/
    CarDB hashMaker(){
        vector<Car> dataList;
        Random RndID(MINID,MAXID);
        Random RndCar(0,4);// selects one from the carModels array
        Random RndQuantity(0,50);
        CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
        
        for (int i=0;i<49;i++){
            // generating random data
            Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                            RndID.getRandNum(), true);
            // saving data for later use
            dataList.push_back(dataObj);
            // inserting data in to the CarDB object
            if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
        }
        return cardb;
    }    
    /***************************************************/

    /***************************************************/
    static unsigned int hashCode(const string str) {
        unsigned int val = 0 ;
        const unsigned int thirtyThree = 33 ;  // magic number from textbook
        for (unsigned int i = 0 ; i < str.length(); i++)
            val = val * thirtyThree + str[i] ;
        return val ;
    
    }
    /***************************************************/

};

unsigned int hashCode(const string str);

string carModels[5] = {"challenger", "stratos", "gt500", "miura", "x101"};
string dealers[5] = {"super car", "mega car", "car world", "car joint", "shack of cars"};

int main(){
    
    vector<Car> dataList;
    Random RndID(MINID,MAXID);
    Random RndCar(0,4);// selects one from the carModels array
    Random RndQuantity(0,50);
    CarDB cardb(MINPRIME, hashCode, DOUBLEHASH);
    bool result = true;
    
    for (int i=0;i<49;i++){
        // generating random data
        Car dataObj = Car(carModels[RndCar.getRandNum()], RndQuantity.getRandNum(), 
                          RndID.getRandNum(), true);
        // saving data for later use
        dataList.push_back(dataObj);
        // inserting data in to the CarDB object
        if (!cardb.insert(dataObj)) cout << "Did not insert " << dataObj << endl;
    }    

    // dumping the data m_currentTable[index] ints to the standard output
    cardb.dump();

    // checking whether all data points are inserted
    for (vector<Car>::iterator it = dataList.begin(); it != dataList.end(); it++){
        result = result && (*it == cardb.getCar((*it).getModel(), (*it).getDealer()));
    }
    if (result)
        cout << "All data points exist in the CarDB object!\n";
    else
        cout << "Some data points are missing in the CarDB object\n";
   
 
    /***************************
                TESTS
    ***************************/

    Tester tester; // tester object created

    //testing insertion
    if(tester.InsertionNormalDoublehash()){cout << "InsertionNormalDoublehash: PASSED" << endl;}
    else{cout << "InsertionNormalDoublehash: FAILED" << endl;}

    if(tester.InsertionNormalQuad()){cout << "InsertionNormalQuad: PASSED" << endl;}
    else{cout << "InsertionNormalQuad: FAILED" << endl;}

    if(tester.InsertionNormal()){cout << "InsertionNormal: PASSED" << endl;}
    else{cout << "InsertionNormal: FAILED" << endl;}

    if(tester.FindError()){cout << "FindError: PASSED" << endl;}
    else{cout << "FindError: FAILED" << endl;}

    if(tester.FindNonCollidingKeys()){cout << "FindNonCollidingKeys: PASSED" << endl;}
    else{cout << "FindNonCollidingKeys: FAILED" << endl;}

    if(tester.FindCollidingKeys()){cout << "FindCollidingKeys: PASSED" << endl;}
    else{cout << "FindCollidingKeys: FAILED" << endl;}

    if(tester.RemoveNonColliding()){cout << "RemoveNonColliding: PASSED" << endl;}
    else{cout << "RemoveNonColliding: FAILED" << endl;}

    if(tester.RemoveColliding()){cout << "RemoveColliding: PASSED" << endl;}
    else{cout << "RemoveColliding: FAILED" << endl;}

    if(tester.RehashAfterDataInsert()){cout << "RehashAfterDataInsert: PASSED" << endl;}
    else{cout << "RehashAfterDataInsert: FAILED" << endl;}

    if(tester.RehashCompletion()){cout << "RehashCompletion: PASSED" << endl;}
    else{cout << "RehashCompletion: FAILED" << endl;}

    if(tester.RehashDataDescent()){cout << "RehashDataDescent: PASSED" << endl;}
    else{cout << "RehashDataDescent: FAILED" << endl;}

    if(tester.RehashDataDescentCompletion()){cout << "RehashDataDescentCompletion: PASSED" << endl;}
    else{cout << "RehashDataDescentCompletion: FAILED" << endl;}

//*/
    return 0;

}

unsigned int hashCode(const string str) {
   unsigned int val = 0 ;
   const unsigned int thirtyThree = 33 ;  // magic number from textbook
   for (unsigned int i = 0 ; i < str.length(); i++)
      val = val * thirtyThree + str[i] ;
   return val ;
}