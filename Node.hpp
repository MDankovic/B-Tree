#pragma once

#ifndef _NODE_HPP_
#define _NODE_HPP_

#define MAX 11 
#include <iostream>
using namespace std;

class Node
{
public:
	Node() = default;
	explicit Node(int m);
	~Node();
	Node(const Node& other) {
		copyObj(other);
	}

	int getNumOfKeys() const { return numOfKeys; }						// Broj kljuceva u cvoru
	int getM() const { return m; }										// Stepen cvora
	int getMax() const { if (numOfKeys) return keys[numOfKeys - 1]; }	// Maksimalna vrednost kljuca u cvoru
	int getMin() const { if (numOfKeys) return keys[0]; }				// Minimalna vrednost kljuca u cvoru


	friend ostream& operator<<(ostream& os, const Node *node);	// Ispis cvora u obliku [K0|K1|K2|K3]
	friend int binarySearch(Node *node, int key);
	friend int lowerBound(Node *node, int key);
	friend void sort(int *arr, int n);
	
	int findPos(int key) const;		// Pronalazak (moguce) pozicije kljuca key u cvoru

	friend bool insertKeyInNode(Node *node, int key);		// Umetanje kljuca u cvor
	friend bool deleteKeyFromNode(Node *node, int key);		// Brisanje kljuca iz cvora
	friend bool borrowFromRightBrother(Node *node, Node *parent, Node *brother);		// Pozajmljuje kljuc od desnog brata
	friend bool borrowFromLeftBrother(Node *brother, Node *parent, Node *node);			// Pozajmljuje kljuc od levog brata

	void inOrder() const;	// InOrder obilazak podstabla sa korenom u ovom cvoru

private:
	int keys[MAX];				// Kljucevi u cvoru, validni samo do numOfKeys
	Node *pointers[MAX];		// Pokazivaci na sinove
	Node *parent = nullptr;		// Pokazivac na oca
	int numOfKeys = 0;			// Trenutni broj kljuceva u cvoru
	int m = 0;					// Maksimalni stepen grananja

	// Nepotrebno
	bool isRoot = false;
	bool isLeaf = false;

	// Zasto sam ovo radio kad nismo na OOP??!?!?!?
	void copyObj(const Node& other) {
		numOfKeys = other.numOfKeys;
		m = other.m;
		isRoot = other.isRoot;
		isLeaf = other.isLeaf;

		for (int i = 0; i <= numOfKeys; i++) {
			keys[i] = other.keys[i];
			pointers[i] = other.pointers[i];
		}
	}

	// Prijateljska klasa da bi nam bilo lakse da radimo,
	// da ne moramo da koristimo getter i setter metode
	friend class Tree;
};

#endif
