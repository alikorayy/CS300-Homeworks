#include "BVHTree.h"
#include "AABB.h"


BVHTree::BVHTree() // constructor of BVHTree
{
    root = nullptr;
}

BVHTree::~BVHTree()  // destructor of BVHTree
{
    root = nullptr;
    map.clear();
}

void BVHTree::printNode(std::ostream& out, BVHTreeNode* node, int level) {
    if (root == nullptr) return; // if root is NULL dont do anything and return 
    for (int i = 0; i < level; i++) {
        out << "  ";
    }
    if (!node->isLeaf) {  // if node is not leaf 
        out << "+ branch || ";
        node->aabb.printAABB(out);
        out << std::endl;
        printNode(out, node->rightChild, level + 1);
        printNode(out, node->leftChild, level + 1);
    }
    else {  // if node is leaf
        out << "- ";
        if (node->parent) {
            if (node->parent->rightChild == node)
                out << "R ";
            else
                out << "L ";
        }
        out << "- leaf: " << node->name << " || ";
        node->aabb.printAABB(out);
        out << std::endl;
    }
}

AABB areaCombine(const AABB& lchild, const AABB& rchild);

void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
    //tree is empty
    if (root == NULL)
    {
        root = new BVHTreeNode(objectArea, name, true);
        map.insert({ name, root });
        return;
    }
    //only one node in the tree
    if (isSingleNode())
    {
        BVHTreeNode* oldRoot = root;
        AABB newarea = oldRoot->aabb + objectArea;
        root = new BVHTreeNode(newarea, "branch", false);
        root->rightChild = oldRoot;
        root->rightChild->parent = root;

        root->leftChild = new BVHTreeNode(objectArea, name, true);
        root->leftChild->parent = root;

        map.insert({ name, root->leftChild });
        return;
    }
    //there is more than one node in the tree
    BVHTreeNode* existingLeaf = map.at(findWhereToInsert(objectArea)->name);

    AABB newarea = existingLeaf->aabb + objectArea;
    BVHTreeNode* newBranch = new BVHTreeNode(newarea, "branch", false);
    newBranch->leftChild = new BVHTreeNode(objectArea, name, true);
    newBranch->leftChild->parent = newBranch;

    if (existingLeaf->parent == NULL)  // if the parent of node is NULL
    {
        root = newBranch;
        newBranch->rightChild->parent = root;
    }
    else if (existingLeaf->parent->leftChild == existingLeaf)  // if the node is the left child of its parent
    {
        newBranch->parent = existingLeaf->parent;
        newBranch->parent->leftChild = newBranch;
        map.insert({ name, newBranch->leftChild }); 
    }
    else
    {
        newBranch->parent = existingLeaf->parent;
        newBranch->parent->rightChild = newBranch;
        map.insert({ name, newBranch->leftChild });     
    }
    newBranch->rightChild = existingLeaf;
    newBranch->rightChild->parent = newBranch;  
    //update sizes
    BVHTreeNode* runner = newBranch->leftChild;
    while (runner->parent != NULL)
    {
        if (runner->parent->aabb != runner->parent->leftChild->aabb + runner->parent->rightChild->aabb)
        {
            runner->parent->aabb = runner->parent->leftChild->aabb + runner->parent->rightChild->aabb;
        }
        runner = runner->parent;
    }  
    return;
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation)  // a function moves the node
{
    BVHTreeNode* nodetoMove = map.at(name);
    
    //if inside parent area
    if (newLocation.minX >= nodetoMove->parent->aabb.minX &&
        newLocation.minY >= nodetoMove->parent->aabb.minY &&
        newLocation.maxX <= nodetoMove->parent->aabb.maxX &&
        newLocation.maxY <= nodetoMove->parent->aabb.maxY)
    {
        //update location
        nodetoMove->aabb = newLocation;
    }
    //if not inside parent area
    else
    {
        removeBVHMember(name);
        addBVHMember(newLocation, name);
    }
}

void BVHTree::removeBVHMember(std::string name)
{
    BVHTreeNode* toRemove = map.at(name);
    BVHTreeNode* toRemoveGrandparent = toRemove->parent->parent;

    if (toRemove->parent->leftChild == toRemove)
    {
        BVHTreeNode* toRemoveSibling = toRemove->parent->rightChild;
        //if to removes parent in a right child
        if (toRemoveGrandparent != NULL)
        {
            if (toRemoveGrandparent->rightChild == toRemove->parent)
            {
                toRemoveGrandparent->rightChild = toRemoveSibling;
            }
            //if to removes parent in a right child
            if (toRemoveGrandparent->leftChild == toRemove->parent)
            {
                toRemoveGrandparent->leftChild = toRemoveSibling;
            }
            toRemoveSibling->parent = toRemoveGrandparent;
        }
        else
        {
            root = toRemoveSibling;
        }
    } 
    else
    {
        BVHTreeNode* toRemoveSibling = toRemove->parent->leftChild;
        //if to removes parent in a right child
        if (toRemoveGrandparent != NULL)
        {
            if (toRemoveGrandparent->rightChild == toRemove->parent)
            {
                toRemoveGrandparent->rightChild = toRemoveSibling;
            }
            //if to removes parent in a right child
            if (toRemoveGrandparent->leftChild == toRemove->parent)
            {
                toRemoveGrandparent->leftChild = toRemoveSibling;
            }
            toRemoveSibling->parent = toRemoveGrandparent;
        }
        else
        {
            root = toRemoveSibling;
        }
        

    }
    map.erase(name);
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object)
{    
    std::vector<std::string> toReturn;
    BVHTreeNode* runner = root;
    getCollidingObjectsInOrder(runner, object, toReturn);
    return toReturn;
}

bool BVHTree::isSingleNode()
{
    if (root == NULL)
    {
        return false;
    }
    if ( (root->leftChild == NULL) && (root->rightChild == NULL))
    {
        return true;
    }
    return false; 
}

BVHTreeNode* BVHTree::findWhereToInsert(AABB obj)  // a function decides to go the left child or right child
{
    BVHTreeNode* runner = root;
    while (!runner->isLeaf)
    {
        int increaseInRightTreeSize = AABB::unionArea(obj,
            runner->rightChild->aabb) - runner->rightChild->aabb.getArea();
        int increaseInLeftTreeSize = AABB::unionArea(obj,
            runner->leftChild->aabb) - runner->leftChild->aabb.getArea();
        if (increaseInRightTreeSize < increaseInLeftTreeSize)
        {
            runner = runner->rightChild;
        }
        else
        {
            runner = runner->leftChild;
        }
    }
    return runner;
}

void BVHTree::getCollidingObjectsInOrder(BVHTreeNode* node, AABB object, std::vector<std::string>& v)
{
    if (node != NULL)
    {
        if (node->aabb.collide(object))
        {
            //if leaf
            if (node->isLeaf)
            {
                v.push_back(node->name);
            }
            else
            {
                if (node->rightChild->aabb.collide(object))
                {
                    getCollidingObjectsInOrder(node->rightChild, object, v);
                }

                if (node->leftChild->aabb.collide(object))
                {
                    getCollidingObjectsInOrder(node->leftChild, object, v);
                }
            }   
        }
    }
}

std::ostream& operator<<(std::ostream& out, BVHTree& tree) {
    tree.printNode(out, tree.root, 0);
    return out;
}

AABB areaCombine(const AABB& lchild, const AABB& rchild)
{
    int newMinX, newMinY, newMaxX, newMaxY;

    if (lchild.minX <= rchild.minX)
    {
        newMinX = lchild.minX;
    }
    else { newMinX = rchild.minX; }

    if (lchild.minY <= rchild.minY)
    {
        newMinY = lchild.minY;
    }
    else { newMinY = rchild.minY; }

    if (lchild.maxX >= rchild.maxX)
    {
        newMaxX = lchild.maxX;
    }
    else { newMaxX = rchild.maxX; }

    if (lchild.maxY >= rchild.maxY)
    {
        newMaxY = lchild.maxY;
    }
    else { newMaxY = rchild.maxY; }

    return AABB(newMinX, newMinY, newMaxX, newMaxY);
}