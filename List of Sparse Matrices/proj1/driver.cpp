// CMSC 341 - Fall 2023 - Project 1
#include "csr.h"

class Tester{
    
};
// this program presents a sample usage of project 1
int main()
{
    try{
        Tester tester;
        CSR aCSR;
        int array1[] = {10,20,0,0,0,0,0,30,0,40,0,0,0,0,50,60,70,0,0,0,0,0,0,80};
        aCSR.compress(4,6,array1,24);//initialize object aCSR
        aCSR.dump();
        CSR bCSR(aCSR);//create bCSR using copy constructor
        CSR cCSR;
        int array2[] = {0,0,0,0,100,200,0,0,300};
        cCSR.compress(3,3,array2,9);//initialize object cCSR
        CSR dCSR(cCSR);//create dCSR using copy constructor

        CSRList aCSRList;//create aCSRList
        aCSRList.insertAtHead(aCSR);
        aCSRList.insertAtHead(cCSR);
        CSRList bCSRList;//create bCSRList
        bCSRList.insertAtHead(dCSR);
        bCSRList.insertAtHead(bCSR);

        cout << endl << "Dumping aCSRList:" << endl;
        aCSRList.dump();
        cout << endl << "Dumping bCSRList:" << endl;
        bCSRList.dump();
        
        cout << endl << aCSRList.getAt(1,2,4) << endl;//returns the value 70

        cout << endl << aCSRList.getAt(5,2,2) << endl;//throws an exception
    }
    catch (exception &e){
        cout << e.what() << endl;
    }
    return 0;
}