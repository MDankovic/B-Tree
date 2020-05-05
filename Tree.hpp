#pragma once

#ifndef _TREE_HPP_
#define _TREE_HPP_

#include "Node.hpp"

using namespace std;

/*
	Cuvamo osnovne podatke u stablu i pokazivac na koren
	Ova klasa je prijateljska klasa klasi Node, da bi iz metoda
	ove klase bez ogranicenja mogli da pristupamo clanovima klase Node
*/
class Tree
{
public:
	Tree(int m) : m(m) {}		// Stvaranje praznog stabla sa zadatim redom m
	Tree(const Tree&) = delete;	// Zabrana kopiranja
	~Tree();					// Dovrsi destruktor retardu glupi da bude nerekurzivan

	// Trivijalne getter funkcije
	int getHeight() const { return height; }		// Visina stabla
	int getNumOfKeys() const { return numOfKeys; }	// Broj umetnutih kljuceva u stablo
	int getM() const { return m; }					// Red stabla
	const Node *getRoot() const { return root; }	// Pokazivac na koren stabla

	// Osnovne operacije nad B stablom
	bool searchKey(int key) const;
	bool insertKey(int key);
	bool deleteKey(int key);

	// Obilasci stabla
	void levelOrder() const;
	void inOrder() const;

	// Uravnotezavanje stabla
	void modifyTree() const;
	
	// Ispis stabla
	friend ostream& operator<<(ostream& os, const Tree& tree);
	friend ostream& operator<<(ostream& os, const Tree* tree) { return os << *tree; }

private:
	Node *root = nullptr;	// Koren stabla
	int height = 0;			// Visina stabla
	int numOfKeys = 0;		// Broj kljuceva u stablu
	int numOfNodes = 0;		// Broj cvorova u stablu
	int m;					// Red stabla

	// Pomocna funkcija za pretragu stabla na zadati kljuc
	Node* searchKeyUtil(int key) const;	
};



#endif
