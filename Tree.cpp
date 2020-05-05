#include "Tree.hpp"
#include <queue>
#include <iostream>

using namespace std;

// Destruktor
Tree::~Tree()
{
	//cout << "tree desctructor" << endl;
	delete root; // On sam obrise dalje svoje sinove
	root = nullptr;
}

// LevelOrder obilazak stabla
void Tree::levelOrder() const
{
	//cout << numOfKeys << endl; 
	if (root == nullptr) {
		return;
	}

	Node *curr = root;
	int currLevel = 0, prevLevel = 0;
	queue<Node *> q;
	queue<int> level;

	q.push(curr);
	level.push(currLevel);
	while (!q.empty()) {
		curr = q.front();
		q.pop();

		// Obilazak je za sad samo stampanje, bilo bi dobro da prosledjujemo pokazivac na funkciju koja vrsi obradu
		prevLevel = currLevel;
		currLevel = level.front();
		level.pop();

		if (currLevel > prevLevel)
			cout << endl << curr;
		else
			cout << curr;
		cout << "    ";

		// Dodavanje preostalih validnih sinova u red
		for (int i = 0; i <= curr->numOfKeys; i++) {
			if (curr->pointers[i]) {
				q.push(curr->pointers[i]);
				level.push(currLevel + 1);
			}
		}
	}
	cout << endl << "--------------------------" << endl;
}

// LevelOrder obilazak stabla
void Tree::modifyTree() const
{
	// Ako je prazno stablo nemamo sta da uravnotezavamo
	if (root == nullptr) {
		return;
	}

	Node *curr = root;
	int currLevel = 0, prevLevel = 0;
	queue<Node *> q;
	queue<int> level;

	queue<Node *> sons;
	int numOfSons, numOfKeysInSons;

	q.push(curr);
	level.push(currLevel);
	while (!q.empty()) {
		curr = q.front();
		q.pop();

		// Obilazak je za sad samo stampanje, bilo bi dobro da prosledjujemo pokazivac na funkciju koja vrsi obradu
		prevLevel = currLevel;
		currLevel = level.front();
		level.pop();
		numOfSons = 0;
		numOfKeysInSons = 0;

		// Dodavanje preostalih validnih sinova u red
		for (int i = 0; i <= curr->numOfKeys; i++) {
			if (curr->pointers[i]) {
				q.push(curr->pointers[i]);
				level.push(currLevel + 1);

				sons.push(curr->pointers[i]);
				numOfSons++;
				numOfKeysInSons += curr->pointers[i]->numOfKeys;
			}
		}
		
		if (numOfSons == 0)
			continue;
		int optimalKeysPerSon = numOfKeysInSons / numOfSons + 1;
		cout << "Optimalno: " << optimalKeysPerSon << endl;
		cout << "Broj kljuceva: " << numOfKeysInSons << endl;
		cout << "numOfSons: " << numOfSons << endl;
		
		Node *son;
		while (!sons.empty()) {
			cout << "pokusaj zajam" << endl;
			son = sons.front();
			sons.pop();
			while (son->numOfKeys < optimalKeysPerSon) {
				cout << "zajmi modify" << endl;
				Node *br = nullptr;
				if (!sons.empty()) {
					br = sons.front();
					cout << br << endl;
				}
				else {
					break;
				}
				if (!borrowFromRightBrother(son, son->parent, br)) {
					break;
				}
				else {
					cout << "ok zajmio";
				}
			}
		}
	}
}

// InOrder obilazak stabla
void Tree::inOrder() const
{
	cout << "InOrder traversal: ";
	root->inOrder();
	cout << endl;
}

// Ispis stabla po nivoima
ostream& operator<<(ostream& os, const Tree& tree)
{
	tree.levelOrder();
	return os;
}

// Pomocna metoda za pretragu stabla na zadati kljuc
// Vraca cvor uspesne ili list neuspesne pretrage
Node* Tree::searchKeyUtil(int key) const
{
	Node *curr = root, *prev = nullptr;

	while (curr != nullptr) {
		int pos = lowerBound(curr, key);
		if (curr->keys[pos] == key) {
			return curr;
		}
		else {
			prev = curr;
			curr = curr->pointers[pos + (curr->keys[pos] < key ? 1 : 0)];
		}
	}
	return prev;
}

// Pretraga stabla na zadatki kljuc
bool Tree::searchKey(int key) const
{
	Node *tmp;
	if ((tmp = searchKeyUtil(key)) != nullptr) {
		if (key == tmp->keys[tmp->findPos(key)]) {
			return true;
		}
	}
	return false;
}

// Umetanje kljuca key u stablo
bool Tree::insertKey(int key)
{	
	// Ako kljuc postoji u stablu, ne mozemo da ga umetnemo
	if (searchKey(key))
		return false;	//Dodavanje je neuspesno
	
	numOfKeys++; // Ako se ne nalazi u stablu, znaci da dodajemo novi kljuc

	// Pronalazenje lista u koji se kljuc umece
	Node *destinationNode = searchKeyUtil(key);

	// Ako postoji cvor gde treba umetnuti kljuc
	if (destinationNode != nullptr) {
		insertKeyInNode(destinationNode, key);
	}
	// Ako je stablo prazno
	else {
		root = new Node(m);		// Pravimo novi koren
		root->numOfKeys = 1;	// Trenutno je samo 1 kljuc u korenu
		root->keys[0] = key;	// Na prvo mesto korena upisujemo taj kljuc
		height = 1;				// Visina stabla je 1
	}

	// Postavljanje odgovarajuceg cvora za koren stabla
	// Ako cvor koji je do sada bio koren sada ima oca, znaci da se pocepao
	// To znaci da je stablo poraslo u visinu i potrebno je azurirati koren
	if (root->parent != nullptr) {
		root->isRoot = false;
		root = root->parent;
		root->isRoot = true;

		height++;	// Ako se koren poremetio, visina se povecala
	}

	return true;	// Dodavanje je uspesno
}

// Odradi ovde i brisanje iz korena da ne moras da vodis racuna o tome DONE
// Brisanje kljuca key iz stabla
bool Tree::deleteKey(int key)
{
	// Ako kljuc ne postoji u stablu, ne mozemo da ga obrisemo
	if (!searchKey(key))
		return false;	// Brisanje je neuspesno

	numOfKeys--; // Ako se nalazi u stablu, znaci da brisemo jedan kljuc

	// Pronalazenje cvora iz kog brisemo kljuc
	Node *destinationNode = searchKeyUtil(key);

	// Ako postoji cvor gde treba umetnuti kljuc
	if (destinationNode) {
		deleteKeyFromNode(destinationNode, key);
	}
	// Ako je stablo prazno
	else {
		numOfKeys++;	// Jer smo gore oduzeli kao da smo obrisali
		return false;
	}

	// Postavljanje odgovarajuceg cvora za koren stabla
	// Ako je cvor koji je do sada bio koren sada prazan
	// To znaci da se smanjila visina stabla i potrebno je azurirati koren
	if (root->numOfKeys == 0) {
		Node *oldRoot = root;;
		root = root->pointers[0];
		root->isRoot = true;
		cout << "Novi koren stabla" << endl;
		// Jer smo destruktor cvora lepo napravili
		for (int i = 0; i <= m; i++)
			oldRoot->pointers[i] = nullptr;
		delete oldRoot;

		height--;	// Ako se koren poremetio, visina se smanjila
	}

	return true;	// Brisanje je uspesno
}