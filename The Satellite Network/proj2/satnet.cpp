// UMBC - CMSC 341 - Fall 2023 - Proj2
#include "satnet.h"
SatNet::SatNet(){

    m_root = nullptr; //ASSIGNMENT Operator for satnet object 

}

SatNet::~SatNet(){
    
    DeallocateHelper(m_root); //deallocates all information in the satnet object by calling helper function
    delete m_root; // delets the root node

}

void SatNet::insert(const Sat& satellite){ // recursion neccessary for this because
                                            // the balancing of trees for the AVL tree recursion 
                                            // makes it easier to back track
    if (satellite.getID() > MAXID || satellite.getID() < MINID){

        return; // if this isnt in the range  return

    }

    InsertHelper(satellite, m_root); // to insert in the tree

    HeightHelper(m_root); // fix tree height

}

void SatNet::clear(){

    DeallocateHelper(m_root); // deallocates all information starting at the root
    
}

void SatNet::remove(int id){

    RemoveHelper(m_root, id); // calls remove helper functoin

}

void SatNet::dumpTree() const {
    dump(m_root); // calls dump helper functoin
}

void SatNet::dump(Sat* satellite) const{ // reccursively dumps the tree contents
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}



void SatNet::listSatellites() const {
    ListHelper(m_root); // calls the list helper function
}

bool SatNet::setState(int id, STATE state){
    
    return false; // unfinished 

}

void SatNet::removeDeorbited(){

    RemoveDeorbitedHelper(m_root); // calls the deorbited helper function
    
}

bool SatNet::findSatellite(int id) const{

    return FindSatHelper(m_root, id); // call tge find sat helper funvtion

}

const SatNet & SatNet::operator=(const SatNet & rhs){
    
    return *this; // unfinished

}

int SatNet::countSatellites(INCLIN degree) const{
    
    int num = 0; 
    num = CountHelper(m_root, degree, num); // calls the couthelper function

    return num; // returns the value of the satllites

}

void SatNet::InsertHelper(const Sat& satellite, Sat*& root){

    // if the node passed in null 
    if (root == nullptr){
        Sat* newLeaf = new Sat(satellite.getID(), satellite.getAlt(), satellite.getInclin(), satellite.getState());
        root = newLeaf;
        root->setHeight(HeightHelper(root));
        
    }
    
    //if the node passed is == 
    else if(root->getID() == satellite.getID()){
        return;
        //ignore
    }

    //if the node passed is >
    else if(root->getID() > satellite.getID()){
        // pass InsertHelper the left leaf node of root
        InsertHelper(satellite, root->m_left);
        root->setHeight(HeightHelper(root));
        BalanceFactor(root);
    }

    // if the node psased is <
    else{
        // pass InsertHelper the right leaf node of the root
        InsertHelper(satellite, root->m_right);
        root->setHeight(HeightHelper(root));
        BalanceFactor(root);
    }

}

void SatNet::ListHelper(Sat* root) const{
    if (root != nullptr){
        ListHelper(root->getLeft());//first visit the left child
        cout << root->getID() << ": "
            << root->getStateStr()  << ": " 
            << root->getInclinStr() << ": "
            << root->getAltStr() << endl;
        ListHelper(root->getRight());
    }
}

Sat* SatNet::RemoveHelper(Sat*& tree, int id){

    //root is null
    if (tree == nullptr){
        return tree;
    }

    //one child or...
            //if childs on right
            //if childs on left
    //no children 
            //same as above. minimize code

    if (id < tree->getID()){ // if sat is < tree id
        tree->m_left = RemoveHelper(tree->m_left, id);
        tree->setHeight(HeightHelper(tree));
        BalanceFactor(tree);
    }
    else if (id > tree->getID()){ // if sat is > tree id
        tree->m_right = RemoveHelper(tree->m_right, id);
        tree->setHeight(HeightHelper(tree));
        BalanceFactor(tree);
    }

    else { // if value is found


        if (tree->m_left == nullptr){

            Sat* temp = tree->m_right;
            delete tree;
            return temp;

        }

        else if (tree->m_right == nullptr){

            Sat* temp = tree->m_left;
            delete tree;
            return temp;

        }

        //twochildren 
            // in order successor vvvvvv

        Sat* temp = FindMinHelper(tree->m_right);

        tree->setID(temp->getID());

        tree->m_right = RemoveHelper(tree->m_right, temp->getID());

    }

    return tree;

}

Sat* SatNet::FindMinHelper(Sat*& node){

    if (node->m_left != nullptr){ // if the left node exists
        Sat* min = FindMinHelper(node->m_left); // the fine min helper functoin passed the neft node
        return min; // return the min
    }
    Sat* min = node; // min is the node passed by the param
    return min;

}

void SatNet::DeallocateHelper(Sat*& node){

    if (node != nullptr){ //if node exists

        DeallocateHelper(node->m_left); // recursively call down the left side of the tree
        delete node->m_left; // once at the bottom delete the bottom left nodew
        DeallocateHelper(node->m_right); // go to right if you can
        delete node->m_right; // delete right if not

    }

}

Sat* SatNet::LeftRotation(Sat* gp){

    //gp means granparent

    Sat* parent = gp->m_right; 
    Sat* child = parent->m_right;
    Sat* x2 = parent->m_left;
    parent->m_left = gp;
    gp->m_right = x2;

    parent->setHeight(HeightHelper(parent));
    if (child != nullptr) {
        child->setHeight(HeightHelper(child));
    }

    return parent;

}

Sat* SatNet::RightRotation(Sat* gp){

    //gp means granparent

    Sat* parent = gp->m_left;
    Sat* child = parent->m_left;
    Sat* x3 = parent->m_right;
    parent->m_right = gp;
    gp->m_left = x3;

    parent->setHeight(HeightHelper(parent));
    if (child != nullptr) {
        child->setHeight(HeightHelper(child));
    }

    return parent;
    
    }

int SatNet::HeightHelper(Sat*& node){

    if (node == nullptr){

        return 0;

    }

    else if((node->m_left == nullptr) and (node->m_right == nullptr)){
        
        return 0; // if neither exist

    }

    else{

        int leftH = 1 + HeightHelper(node->m_left); // go down the entire left side of the tree if it exists and then everytime you come back add one to the height of the previous node
        int rightH = 1 + HeightHelper(node->m_right);// go down the entire right side of the tree if it exists and then everytime you come back add one to the height of the previous node

        // Update the height of the current node
        if(leftH > rightH){ // return the largest height

            return leftH;

        }

        else{

            return rightH;

        }

    }

}

void SatNet::BalanceFactor(Sat*& node){ 

    if(node != nullptr){
        if (NodeBalance(node) > 1){ // if right rotation meaning 
        //nodes are to the left...        
            if(NodeBalance(node->getLeft()) < 0){ // check if the next node is to the right or not

                node->setLeft(LeftRotation(node->getLeft()));
                node = RightRotation(node);

            } // if left right

            else{

                node = RightRotation(node);

            } // right right
            
        }

        else if(NodeBalance(node) < -1){ // if left
            
            if(NodeBalance(node->getLeft()) > 0){

                node->setRight(RightRotation(node->getRight()));
                node = LeftRotation(node);

            } // if right left
        
            else{

                node = LeftRotation(node);

            } // //left left
            
        }

        else{

            return;

        }
    }
}

int SatNet::NodeBalance(const Sat* node){ // checks the balance of a tree 

    int leftFactor = 0;
    int rightFactor = 0;


    if (node != nullptr){
        if(node->getRight() == nullptr){
            
            rightFactor = -1; // if the node to the right is null make the right factor -1
            
            } //left factor
        
        else{
            
            rightFactor = node->getRight()->getHeight(); // else make it the height of the right
            
            } // if not null


        if(node->getLeft() == nullptr){
            
            leftFactor = -1; // if left node is null
            
            } //right factor

        else{
            
            leftFactor = node->getLeft()->getHeight(); // if the left factor isnt null 

            } // if not null

        int factor = leftFactor - rightFactor; // find the difference and return
        return factor;    
    }
    return 0;
}

Sat* SatNet::RemoveDeorbitedHelper(Sat*& node){

    if (m_root == nullptr){ // if the node is null
        
        return node;

    }

    else{

        if(node->getState() == DEORBITED){ // if the state of the passed node is deorbited then...

            if (node->m_left == nullptr){ // if the node has one or no children

            Sat* temp = node->m_right;
            delete node;
            return temp;

        }

        else if (node->m_left == nullptr){ // if the node has one or no children

            Sat* temp = node->m_left;
            delete node;
            return temp;

        }

        Sat* temp = FindMinHelper(node->m_right);

        node->setID(temp->getID());

        node->m_right = RemoveHelper(node->m_right, temp->getID());


        } 

        return node;

    }

}        

bool SatNet::FindSatHelper(Sat* tree, int id) const{ //finds the satellite 

    if (tree == nullptr){ // if the tree is null
        return false;
    }

    if (id < tree->getID()){ // if sat is < tree id
        return FindSatHelper(tree->getLeft(), id);
    }
    else if (id > tree->getID()){ // if sat is > tree id
        return FindSatHelper(tree->getRight(), id);
    }

    else if (tree->getID() == id){ //if the id is found in the tree

        return true;
    }

    else{ //return false ig none of the above

        return false;

    }

}

int SatNet::CountHelper(Sat* root, INCLIN degree, int num) const{

    if (root != nullptr){ // if the node existts

        num = CountHelper(root->getLeft(), degree, num); //recurse down the left side of the tree
        num = CountHelper(root->getRight(), degree, num); // then go down the right


        if(degree == root->getInclin()){ // if the node is at the degree we want

            num++; // add one to the total

        }

        return num; // return the number

    }   
    else {return 0;} // if error then return 0

}