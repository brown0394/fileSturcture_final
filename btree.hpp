//btree.tc
#pragma once
#ifndef BTREE_TC
#define BTREE_TC

#include "btnode.h"
#include "indbuff.h"
#include "btree.h"
#include <iostream>

const int MaxHeight = 5;
template <class keyType>
BTree<keyType>::BTree(int order, int keySize, int unique)
	: Buffer(1 + 2 * order, sizeof(int) + order * keySize + order * sizeof(int)),
	BTreeFile(Buffer), Root(order), HBuffer(1, 1), HeightFile(HBuffer) {
	Height = 1;
	Order = order;
	PoolSize = MaxHeight * 2;
	Nodes = new BTNode *[PoolSize];
	BTNode::InitBuffer(Buffer, order, keySize);
	Nodes[0] = &Root;
	hrecaddr = 0;
	HBuffer.AddField(1); 
}

template <class keyType>
BTree<keyType>::~BTree()
{
	Close();
	delete[] Nodes;
}

template <class keyType>
int BTree<keyType>::Open(char * name, int mode)
{
	int result;
	result = BTreeFile.Open(name, mode);
	if (!result) return result;
	// load root
	BTreeFile.Read(Root);
	HeightFile.Open(hbuffName, ios::in | ios::out);
	HeightFile.Read(hrecaddr);
	char* num = new char;
	HeightFile.GetBuffer().Unpack(num);
	Height = *num - '0'; // find height from BTreeFile!
	delete num;
	return 1;
}

template <class keyType>
int BTree<keyType>::Create(char * name, int mode) {
	int result;
	result = BTreeFile.Create(name, mode);
	if (!result) return result;
	// append root node
	result = BTreeFile.Write(Root);
	Root.RecAddr = result;
	strcpy(hbuffName, name);
	strcat(hbuffName, ".hf");
	HeightFile.Create(hbuffName, mode);
	HeightFile.Close();
	return result != -1;
}

template <class keyType>
int BTree<keyType>::Close()
{
	int result;
	result = BTreeFile.Rewind();
	if (!result) return result;
	result = BTreeFile.Write(Root);
	if (result == -1) return 0;

	HeightFile.Open(hbuffName, ios::in | ios::out);
	char* num = new char;
	*num = Height + '0';
	HeightFile.GetBuffer().Pack(num);
	delete num;
	if (hrecaddr == 0)
		hrecaddr = HeightFile.Write();
	else
		HeightFile.Write(hrecaddr);
	HeightFile.Close();
	return BTreeFile.Close();
}


template <class keyType>
int BTree<keyType>::Insert(const keyType key, const int recAddr)
{
	int result; int level = Height - 1;
	int newLargest = 0;
	keyType prevKey, largestKey;
	BTNode * thisNode = nullptr, *newNode = nullptr, *parentNode = nullptr;
	thisNode = FindLeaf(key);

	// test for special case of new largest key in tree
	if (key > thisNode->LargestKey())
	{
		newLargest = 1;
		prevKey = thisNode->LargestKey();
	}

	result = thisNode->Insert(key, recAddr);

	// handle special case of new largest key in tree
	if (newLargest)
		for (int i = 0; i < Height - 1; i++)
		{
			Nodes[i]->UpdateKey(prevKey, key);
			if (i > 0) Store(Nodes[i]);
		}

	while (result == -1) // if overflow and not root
	{
		//remember the largest key
		largestKey = thisNode->LargestKey();
		// split the node
		newNode = NewNode();
		thisNode->Split(newNode);
		Store(thisNode);
		Store(newNode);
		level--; // go up to parent level
		if (level < 0) break;
		// insert newNode into parent of thisNode
		parentNode = Nodes[level];
		result = parentNode->UpdateKey(largestKey, thisNode->LargestKey());
		result = parentNode->Insert(newNode->LargestKey(), newNode->RecAddr);
		thisNode = parentNode;
	}
	Store(thisNode);
	if (level >= 0) return 1;// insert complete
							 // else we just split the root
	int newAddr = BTreeFile.Append(Root); // put previous root into file
										  // insert 2 keys in new root node
	Root.Keys[0] = thisNode->LargestKey();
	Root.RecAddrs[0] = newAddr;
	Root.Keys[1] = newNode->LargestKey();
	Root.RecAddrs[1] = newNode->RecAddr;
	Root.NumKeys = 2;
	Height++;
	return 1;
}

template <class keyType>
int BTree<keyType>::Remove(const keyType key, const int recAddr)
{
	int result; int level = Height - 1; int mergeResult;
	//int newLargest = 0;
	keyType largestKey;
	BTNode* thisNode = nullptr, * nextNode = nullptr, * parentNode = nullptr;
	thisNode = FindLeaf(key);
	largestKey = thisNode->LargestKey();
	result = thisNode->Remove(key, recAddr);

	if (result == -1) {
		--level;
		if (level < 0) return -1;
		parentNode = Nodes[level];
		int idx = 0;
		while (parentNode->Keys[idx] != largestKey) {
			++idx;
		}
		if (idx < parentNode->NumKeys) {
			nextNode = Fetch(parentNode->RecAddrs[idx + 1]);
			mergeResult = thisNode->Merge(nextNode);
			if (mergeResult == 0) {
				//redistribute
			}
			else {
				parentNode->Remove(key);
			}
		}
		else {
			nextNode = Fetch(parentNode->RecAddrs[idx - 1]);
			mergeResult = nextNode->Merge(thisNode);
		}
	}
	else {
		Store(thisNode);
	}
	return -1;
}

template <class keyType>
int BTree<keyType>::Search(const keyType key, const int recAddr)
{
	BTNode * leafNode;
	leafNode = FindLeaf(key);
	return leafNode->Search(key, recAddr);
}

template <class keyType>
void BTree<keyType>::Print(ostream & stream)
{
	stream << "BTree of height " << Height << " is " << endl;
	Root.Print(stream);
	if (Height > 1)
		for (int i = 0; i < Root.numKeys(); i++)
		{
			Print(stream, Root.RecAddrs[i], 2);
		}
	stream << "end of BTree" << endl;
}

template <class keyType>
void BTree<keyType>::Print
(ostream & stream, int nodeAddr, int level)
{
	BTNode * thisNode = Fetch(nodeAddr);
	stream << "BTree::Print() ->Node at level " << level << " address " << nodeAddr << ' ' << endl;
	thisNode->Print(stream);
	if (Height > level)
	{
		level++;
		for (int i = 0; i < thisNode->numKeys(); i++)
		{
			Print(stream, thisNode->RecAddrs[i], level);
		}
		stream << "end of level " << level << endl;
	}
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::FindLeaf(const keyType key)
// load a branch into memory down to the leaf with key
{
	int recAddr, level;
	for (level = 1; level < Height; level++)
	{
		recAddr = Nodes[level - 1]->Search(key, -1, 0);//inexact search
		Nodes[level] = Fetch(recAddr);
	}
	return Nodes[level - 1];
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::NewNode()
{// create a fresh node, insert into tree and set RecAddr member
	BTNode * newNode = new BTNode(Order);
	int recAddr = BTreeFile.Append(*newNode);
	newNode->RecAddr = recAddr;
	return newNode;
}

template <class keyType>
BTreeNode<keyType> * BTree<keyType>::Fetch(const int recaddr)
{// load this node from File into a new BTreeNode
	int result;
	BTNode * newNode = new BTNode(Order);
	result = BTreeFile.Read(*newNode, recaddr);
	if (result == -1) return NULL;
	newNode->RecAddr = result;
	return newNode;
}
template<class keyType>
int BTree<keyType>::Store(BTNode * thisNode)
{
	return BTreeFile.Write(*thisNode, thisNode->RecAddr);
}

template<class keyType>
template <class rfType>
void BTree<keyType>::InorderTraversal(BTNode* btnode, RecordFile<rfType>& recordFile, int level) {
	rfType type;
	for (int i = 0; i < btnode->NumKeys; ++i) {
		if (level == Height) {
			recordFile.Read(type, btnode->RecAddrs[i]);
			cout << type << endl;
		}
		else
			InorderTraversal(Fetch(btnode->RecAddrs[i]), recordFile, level + 1);
	}
}

template<class keyType>
template <class rfType>
void BTree<keyType>::PrintSorted(RecordFile<rfType>& recordFile) {
	InorderTraversal(&Root, recordFile, 1);
}

#endif