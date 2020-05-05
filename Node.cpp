#include "Node.hpp"
#include <iostream>
#include <stack>

using namespace std;

// Konstruktor koji stvara prazan cvora reda m
Node::Node(int m) : m(m)
{
	for (int i = 0; i <= m; i++) {
		pointers[i] = nullptr;
		keys[i] = 0;
	}
}

// Destruktor
Node::~Node()
{
	//cout << "node desctructor" << endl;
	for (int i = 0; i <= numOfKeys; i++) {
		delete pointers[i];
		pointers[i] = nullptr;
	}
	numOfKeys = 0;
	m = 0;
	parent = nullptr;
}

// Ipis elemenata cvora u odgovarajucoj formi
ostream& operator<<(ostream& os, const Node *node)
{
	// Ako stampamo nullptr samo preskoci
	if (node == nullptr) return os;

	for (int i = 0; i < node->numOfKeys; i++) {
		if (i == 0)
			os << '[';

		os << node->keys[i];

		if (i == node->numOfKeys - 1)
			os << ']';
		else
			os << '|';
	}
	return os;
}

// InOrder obilazak podstabla sa korenom u cvoru this
void Node::inOrder() const
{
	for (int i = 0; i < this->numOfKeys; i++) {
		if(this->pointers[i] != nullptr)
			this->pointers[i]->inOrder();
		cout << this->keys[i] << " ";
	}
	if(this->pointers[this->numOfKeys])
		this->pointers[this->numOfKeys]->inOrder();
}

// O(log n)
int binarySearch(Node *node, int key)
{
	int low = 0;
	int high = node->numOfKeys;
	int mid;

	while (low < high) {
		mid = (low + high) / 2;
		if (node->keys[mid] == key) {
			return mid;
		}
		else if (node->keys[mid] > key) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	return -1;
}

// Debilni sort O(n^2), nije potreban
void sort(int *arr, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			if (arr[i] > arr[j]) {
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

// O(log n)
int lowerBound(Node *node, int key)
{
	int low = 0;
	int high = node->numOfKeys -1;
	int mid;

	while (low < high) {
		mid = (low + high) / 2;
		if (key <= node->keys[mid]) {
			high = mid;
		}
		else {
			low = mid + 1;
		}
	}
	return low;
}

// Pronalazi poziciju prvog veceg ili jednakog elementa u cvoru
// Nakon poziva mora da se proveri da li je kljuc manji od svih, veci od svih, jednak nekom elementu ili ne postoji
int Node::findPos(int key) const
{
	int pos = 0;
	while (pos < numOfKeys && key > keys[pos]) {
		pos++;
	}
	return pos;
}

// Ubacivanje kljuca u zadati cvor, uvek pozivamo za list i to samo ako se kljuc ne nalazi u stablu
bool insertKeyInNode(Node *node, int key)
{
	// Pozicija gde bi se umetnuo kljuc
	int pos = node->findPos(key);

	// Ako ima mesta samo ga dodamo uz pomeranje ostalih
	if (node->numOfKeys < (node->m - 1)) {
		//DEBUG
		//cout << "Umetnut u list" << endl;
		for (int i = node->numOfKeys; i >= pos + 1; i--) {
			node->keys[i] = node->keys[i - 1];
		}
		node->keys[pos] = key;
		node->numOfKeys++;
		return true;
	}

	// Ako ne mozemo samo da ubacimo kljuc, moramo da cepamo cvor

	int *arr = new int[node->m];				// Pomocni niz za preraspodelu kljuceva
	Node **parr = new Node *[node->m + 1];		// Pomocni niz za preraspodelu pokazivaca
	int mid = (node->m + node->m % 2) / 2 - 1;	// Pozicija razdelnog kljuca u pomocnom nizu, posto je indeksiranje od 0, mora ovako
	
	// Ubacivanje elemenata u uredjeni niz
	for (int i = 0; i < node->m - 1; i++)
		arr[i] = node->keys[i];
	arr[node->m - 1] = key;
	sort(arr, node->m);

	//DEBUG
	//cout << endl << "node: " << node << endl << "parent: " << node->parent << endl; // Levi cvor i njegov otac

	// Smestanje kljuceva koji ostaju u levom cvoru
	node->numOfKeys = mid;
	for (int i = 0; i < mid; i++)
		node->keys[i] = arr[i];

	// Kljuc koji saljemo ocu je sada razdelni kljuc za 2 pocepana cvora
	key = arr[mid];

	// Pravimo novi desni cvor
	Node *rightNode = new Node(node->m);

	// Smestanje kljuceva koji idu u novi desni cvor
	for (int i = mid + 1; i < node->m; i++) {
		rightNode->keys[i - (mid + 1)] = arr[i];
	}
	rightNode->numOfKeys = node->m - (mid + 1);
	
	// levi i desni cvor u odnosu na oca cvora node
	Node *l = node;
	Node *r = rightNode;

	// Penjemo se na gore dokle je potrebno i cepamo sve cvorove koje je potrebno iscepati
	// Ako smo iscepali koren, taj slucaj obradjujemo posle ove while petlje
	while (node->parent) {
		node = node->parent;

		// Nova pozicija
		pos = node->findPos(key);
		
		// Ako ima dovoljno mesta
		if (node->numOfKeys < node->m - 1) {
			//DEBUG
			//cout << "Ima mesta u ocu" << endl;

			// Pomeranje kljuceva
			for (int i = node->numOfKeys; i >= pos + 1; i--) { // Nesto prejebo
				node->keys[i] = node->keys[i - 1];
			}
			// Pomeranje pokazivaca
			for (int i = node->numOfKeys + 1; i > pos + 1; i--) {
				node->pointers[i] = node->pointers[i - 1]; // GRESKA mozda
			}
			node->pointers[pos] = l; // Ovo vec vazi
			node->pointers[pos + 1] = r;

			node->numOfKeys++;		// Broj kljuceva se povecao za 1
			node->keys[pos] = key;	// Postavljanje kljuca key na odgovarajucu poziciju

			// Ulancavanje sa ocem pocepanog cvora
			l->parent = node;
			r->parent = node;
			//DEBUG
			//cout << "l: " << l << " -- " << "r: " << r << " p: " << node << endl;

			// Oslobadjanje zauzete memorije
			delete[] arr;
			delete[] parr;
			return true;
		}
		// Ako je potrebno da pocepamo i trenutni cvor
		else {
			//DEBUG
			//cout << "Nema mesta u ocu" << endl;
			// Ubacivanje elemenata u uredjeni niz
			for (int i = 0; i < node->m - 1; i++)
				arr[i] = node->keys[i];

			arr[node->m - 1] = key;
			sort(arr, node->m);

			// Ubacivanje pokazivaca na odgovarajuce mesto, ovako moze i za kljuceve pa onda ne mora sortiranje
			for (int i = 0; i < node->m; i++) {
				if (i <= pos)
					parr[i] = node->pointers[i];
				else
					parr[i + 1] = node->pointers[i];
			}
			parr[pos + 1] = r;

			//DEBUG ispis
			//cout << "parr: ";
			/*
			for (int i = 0; i < node->m + 1; i++) {
				cout << parr[i] << "  ";
			}
			*/
			//cout << endl << "key: " << key << endl;

			// Smestanje kljuceva koji ostaju u levom cvoru
			node->numOfKeys = mid;
			for (int i = 0; i < mid; i++)
				node->keys[i] = arr[i];
			
			// Kljuc koji saljemo ocu je sada razdelni kljuc za 2 pocepana cvora
			key = arr[mid];
			
			// Pravimo novi desni cvor
			// Nema potrebe rightNode da definisemo ponovo, al nmvz
			Node *rightNode = new Node(node->m);

			// Smestanje kljuceva koji idu u novi desni cvor
			for (int i = mid + 1; i < node->m + 1; i++) {
				rightNode->keys[i - (mid + 1)] = arr[i];
			}
			rightNode->numOfKeys = node->m - (mid + 1);
			
			//DEBUG
			//cout << "n: " << node << endl;
			for (int i = 0; i < mid + 1; i++) {
				node->pointers[i] = parr[i];		// Preuzimanje pokazivaca sa odgovarajuce pozicije iz niza parr
				node->pointers[i]->parent = node;	// Ulancavanje sina trenutnog cvora sa ocem
				//DEBUG
				//cout << "np[" << i << "]: " << node->pointers[i] <<  endl;
			}
			//cout << endl;

			//DEBUG
			//cout << "rn: " << rightNode << endl;
			for (int i = mid + 1; i < node->m + 1; i++) {
				rightNode->pointers[i - (mid + 1)] = parr[i];				// Preuzimanje pokazivaca sa odgovarajuce pozicije iz niza parr
				rightNode->pointers[i - (mid + 1)]->parent = rightNode;		// Ulancavanje sina trenutnog cvora sa ocem
				//DEBUG
				//cout << "rnp[" << i - (mid + 1) << "]: " << rightNode->pointers[i - (mid + 1)] << endl;
			}
			//cout << endl;

			// Sada l i r postavljamo na trenutni node i rightNode
			l = node;
			r = rightNode;
		}
	}

	// Ako smo dosli do ovog dela to znaci da u korenu nije bilo mesta i da je on pocepan
	node->isRoot = false;

	// Potrebno je da napravimo novi cvor koji ce biti koren
	Node *newRoot = new Node(node->m);

	// Postavimo mu odgovarajuce parametre
	newRoot->isRoot = true;
	newRoot->keys[0] = key;
	newRoot->numOfKeys = 1;
	newRoot->pointers[0] = l;
	newRoot->pointers[1] = r;

	// Ulancamo sinove novog korena sa njim
	l->parent = newRoot;
	r->parent = newRoot;

	// Oslobadjanje zauzete memorije
	delete[] arr;
	delete[] parr;
	return true;
}

// Brisanje kljuca iz zadatok cvora
bool deleteKeyFromNode(Node *node, int key)
{
	int min = (node->m + node->m % 2) / 2 - 1;

	// Pozicija u cvoru sa koje brisemo kljuc
	int pos = node->findPos(key);

	// Ispis kljuca koji brisemo
	cout << "Brisemo kljuc: " << key << endl;

	int notInLeaft = 0;
	// Ako nismo u listu, zamenimo kljuc sa svojim sledbenikom i onda ga obrisimo sa te pozicije
	if (node->pointers[0] != nullptr) {
		cout << "Nije u listu, nalazi se u unutrasnjem cvoru: " << node << endl;
		Node *tmp = node->pointers[pos + 1];

		// Trazimo list u kome je sledbenik kljuca key
		while (tmp->pointers[0] != nullptr) {
			tmp = tmp->pointers[0];
			notInLeaft = 3;
		}

		int p1 = tmp->keys[0];
		tmp->keys[0] = node->keys[pos];
		node->keys[pos] = p1;

		pos = 0;		// Sigurno je na poziciji 0
		node = tmp;		// Cvor iz koga brisemo je sada odgovarajuci list stabla
		cout << "Brisemo odavde: " << node << endl;
		
		if (notInLeaft == 3) {
			notInLeaft = 0; // Direktan potomak
			//cout << "Nije direktan potomak" << endl;
		}
		else {
			notInLeaft = 1;	// Nije direktan potomak
			//cout << "Direktan potomak" << endl;
		}

		// Posto smo cvor zamenili sledbenikom, na findPos moramo da dodamo 1
	}

	// Sada smo sigurno u listu

	// MOZE I DA SE OBRISE PA SVAKAKO U PETLJI DA SE RADI
	// Ako ostaje dovoljno cvorova nakon brisanja, proveri uslov za koren
	if (node->numOfKeys > min || node->isRoot) {
		for (int i = pos; i < node->numOfKeys - 1; i++) {
			node->keys[i] = node->keys[i + 1];
		}
		node->numOfKeys--;
		return true;
	}
	

	// Ako nakon uklanjanja kljuca ne ostaje dovoljno kljuceva u cvoru
	Node *parent;		// Otac cvora iz kog brisemo
	Node *brother;		// Brat (levi/desni)
	int dividingPos;	// Pozicija razdelnog kljuca izmedju node i brother u ocu

	while (node->parent) {
		pos = node->findPos(key);
		parent = node->parent;
		//cout << "pos->find: " << pos << endl;
		// Pomeranje ako nismo bili u listu
		if (notInLeaft == 1) {
			//cout << "Dodao 1 na findPos" << endl;
			dividingPos = parent->findPos(key) + 1;
			notInLeaft = 0;
			//cout << "nil pos" << endl;
		}
		else {
			//cout << "NIJE dodao 1 na findPos" << endl;
			dividingPos = parent->findPos(key);
		}
		//cout << "divpos->parent->find: " << dividingPos << endl;
		// Ako ostaje dovoljno kljuceva
		if (node->numOfKeys > min || node->isRoot) {
			cout << "U listu ostaje dovoljno kljuceva" << endl;
			for (int i = pos; i < node->numOfKeys - 1; i++) {
				node->keys[i] = node->keys[i + 1];
			}
			for (int i = pos + 1; i < node->numOfKeys; i++) {
				node->pointers[i] = node->pointers[i + 1];
			}
			node->numOfKeys--;
			return true;
		}

		// Pozajmica od desnog brata
		cout << "pointer number: " << dividingPos + 1 << endl;
		cout << "Otac: " << parent << endl;
		for (int i = 0; i <= parent->numOfKeys; i++) {
			cout << parent->pointers[i] << " -> p: " << parent->pointers[i]->parent << endl;
		}
		cout << endl;
		if (dividingPos < parent->numOfKeys && parent->pointers[dividingPos + 1]->numOfKeys > min) { // Nisam skroz desni sin i desni brat ima dovoljno
			brother = parent->pointers[dividingPos + 1];		// Desni brat

			cout << "Zajmimo od desnog brata -> Desni brat: " << brother << endl;
			int brotherKey = brother->keys[0];					// Kljuc koji pozajmljujemo
			Node *brotherFirstPointer = brother->pointers[0];	// Njegov levi pokazivac
			// Pomeranje kljuceva u desnom bratu za jedno mesto levo
			for (int i = 0; i < brother->numOfKeys - 1; i++)
				brother->keys[i] = brother->keys[i + 1];
			for (int i = 0; i < brother->numOfKeys; i++)
				brother->pointers[i] = brother->pointers[i + 1];
			brother->numOfKeys--;	// Broj kljuceva u bratu se smanjio za 1
			if(brotherFirstPointer) brotherFirstPointer->parent = node;	// Postavljanje odgovarajuceg cvora oca

			// Pomeranje kljuceva u originalnom cvoru za 1 mesto u levo da bi na poslednju poziciju smestili pozajmljeni kljuc
			for (int i = pos; i < node->numOfKeys - 1; i++)
				node->keys[i] = node->keys[i + 1];
			for (int i = pos + 1; i < node->numOfKeys; i++)	// Ako smo spajali cvor, spojili smo ga u levi cvor, pa gubimo samo desni pokazivac
				node->pointers[i] = node->pointers[i + 1];

			node->pointers[node->numOfKeys] = brotherFirstPointer;
			node->keys[node->numOfKeys - 1] = parent->keys[dividingPos];
			parent->keys[dividingPos] = brotherKey;

			return true;	// Kraj, uspeli smo da pozajmimo od desnog brata
		}
		// Pozajmica od levog brata
		else if (dividingPos > 0 && parent->pointers[dividingPos - 1]->numOfKeys > min) {	// Nisam skroz levi sin i levi brat ima dovoljno
			cout << "Zajmimo od levog brata" << endl;
			brother = parent->pointers[dividingPos - 1];							// Levi brat
			int brotherKey = brother->keys[brother->numOfKeys - 1];					// Kljuc koji pozajmljujemo
			Node *brotherLastPointer = brother->pointers[brother->numOfKeys];		// Njegov levi pokazivac
			// Ne pomeramo kljuceve jer samo brisemo zadnji
			brother->numOfKeys--;	// Broj kljuceva u bratu se smanjio za 1
			if(brotherLastPointer) brotherLastPointer->parent = node;	// Postavljanje odgovarajuceg cvora oca

			// Pomeranje kljuceva u originalnom cvoru za 1 mesto u desno da bi na poslednju poziciju smestili pozajmljeni kljuc
			for (int i = pos; i > 1; i--)
				node->keys[i] = node->keys[i - 1];
			for (int i = pos + 1; i > 0; i--)	// Ako smo spajali cvor, spojili smo ga u levi cvor, pa gubimo samo desni pokazivac
				node->pointers[i] = node->pointers[i - 1];

			node->pointers[0] = brotherLastPointer;
			node->keys[0] = parent->keys[dividingPos - 1];
			parent->keys[dividingPos - 1] = brotherKey;
			return true;	// Kraj, uspeli smo da pozajmimo od levog brata
		}

		////// TODO ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// Spajanje sa desnim bratom
		else if (dividingPos < parent->numOfKeys) {
			//DEBUG ispis
			cout << "Spajamo sa desnim bratom -> ";
			brother = parent->pointers[dividingPos + 1];		// Desni brat
			cout << "Desni brat: " << brother << endl;

			// Ne koristim
			int *arr = new int[node->m];				// Pomocni niz za preraspodelu kljuceva
			Node **parr = new Node *[node->m + 1];		// Pomocni niz za preraspodelu pokazivaca
			
			// Skupimo kljuceve tako da izgubimo kljuc koji se brise
			// Pomeranje kljuceva u originalnom cvoru za 1 mesto u levo da bi na poslednju poziciju smestili pozajmljeni kljuc
			for (int i = pos; i < node->numOfKeys - 1; i++)
				node->keys[i] = node->keys[i + 1];
			for (int i = pos + 1; i < node->numOfKeys; i++)	// Ako smo spajali cvor, spojili smo ga u levi cvor, pa gubimo samo desni pokazivac
				node->pointers[i] = node->pointers[i + 1];

			// Prebacujemo kljuc iz cvora oca na kraj niza kljuceva u nasem cvoru
			node->keys[node->numOfKeys - 1] = parent->keys[dividingPos]; // prebacujemo kljuc iz oca

			key = parent->keys[dividingPos];	// Ovaj kljuc u sledecoj iteraciji brisemo iz oca

			cout << "Prebacen kljuc iz oca: " << key << endl;
			// Prepisivanje kljuceva iz desnog cvora u levi cvor
			for (int i = node->numOfKeys; i < node->numOfKeys + brother->numOfKeys; i++) {
				node->keys[i] = brother->keys[i - node->numOfKeys];
				cout << "Prebacen kljuc iz brata: " << brother->keys[i - node->numOfKeys] << endl;
			}
			// Prepisivanje pokazivaca iz desnog cvora u levi cvor
			for (int i = node->numOfKeys; i <= node->numOfKeys + brother->numOfKeys; i++) {
				node->pointers[i] = brother->pointers[i - (node->numOfKeys)];
				// Postavljamo novog oca za sinove cvora brother 
				if (brother->pointers[i - (node->numOfKeys)] != nullptr) brother->pointers[i - (node->numOfKeys)]->parent = node;
			}

			for (int i = 0; i <= node->m; i++) {
				brother->pointers[i] = nullptr;	// Da slucajno ne bi obrisali ostatak cvora
			}

			node->numOfKeys += brother->numOfKeys;
			delete brother;		// Brisemo ovog iz kog smo sve pokupili
			cout << "Novi cvor: " << node << endl;
			delete[] arr;
			delete[] parr;
		}
		// Spajanje sa levim bratom
		else {
			// Radimo spajanje sa levim
			//DEBUG ispis
			cout << "Spajamo sa levim bratom -> ";
			brother = parent->pointers[dividingPos - 1];		// levi brat
			cout << "Levi brat: " << brother << endl;

			// Ne koristim
			int *arr = new int[node->m];				// Pomocni niz za preraspodelu kljuceva
			Node **parr = new Node *[node->m + 1];		// Pomocni niz za preraspodelu pokazivaca

			// Skupimo kljuceve tako da izgubimo kljuc koji se brise
			// Pomeranje kljuceva u originalnom cvoru za 1 mesto u levo da bi na poslednju poziciju smestili pozajmljeni kljuc
			for (int i = pos; i < node->numOfKeys - 1; i++)
				node->keys[i] = node->keys[i + 1];
			for (int i = pos + 1; i < node->numOfKeys; i++)	// Ako smo spajali cvor, spojili smo ga u levi cvor, pa gubimo samo desni pokazivac
				node->pointers[i] = node->pointers[i + 1];
			//node->numOfKeys--;	// Smanji broj kljuceva
			
			Node *tmp;
			tmp = node;
			node = brother;
			brother = tmp;

			// Prebacujemo kljuc iz cvora oca na kraj niza kljuceva u nasem cvoru
			node->keys[node->numOfKeys] = parent->keys[dividingPos - 1]; // prebacujemo kljuc iz oca

			key = parent->keys[dividingPos - 1];	// Ovaj kljuc u sledecoj iteraciji brisemo iz oca

			cout << "Prebacen kljuc iz oca: " << key << endl;
			// Prepisivanje kljuceva iz desnog cvora u levi cvor
			// Ovde u uslovu +- 1 proveri, ali sve radi kako treba jer sam posle to ispeglao
			for (int i = node->numOfKeys + 1; i < node->numOfKeys + brother->numOfKeys; i++) {
				node->keys[i] = brother->keys[i - (node->numOfKeys + 1)];
				cout << "Prebacen kljuc iz brata: " << brother->keys[i - (node->numOfKeys + 1)] << endl;
			}
			// Prepisivanje pokazivaca iz desnog cvora u levi cvor
			for (int i = node->numOfKeys + 1; i <= node->numOfKeys + 1 + brother->numOfKeys; i++) {
				node->pointers[i] = brother->pointers[i - (node->numOfKeys + 1)];
				// Postavljamo novog oca za sinove cvora brother 
				if (brother->pointers[i - (node->numOfKeys + 1)] != nullptr) brother->pointers[i - (node->numOfKeys + 1)]->parent = node;
			}

			for (int i = 0; i <= node->m; i++) {
				brother->pointers[i] = nullptr;	// Da slucajno ne bi obrisali ostatak cvora
			}

			node->numOfKeys += brother->numOfKeys;
			delete brother;		// Brisemo ovog iz kog smo sve pokupili
			cout << "Novi cvor: " << node << endl;
			delete[] arr;
			delete[] parr;
		}
		node = parent;
	}

	// Ovde samo obrisemo kljuc iz korena ako je bilo potrebe
	// A u metodi klase Tree postavljamo novi koren stabla
	pos = node->findPos(key);
	// Za sad samo ako ostaje dovoljno u korenu
	for (int i = pos; i < node->numOfKeys - 1; i++) {
		node->keys[i] = node->keys[i + 1];
		cout << "k - " << node->keys[i] << endl;
	}
	cout << endl;
	for (int i = pos + 1; i < node->numOfKeys; i++) {
		node->pointers[i] = node->pointers[i + 1];
		cout << "Sin od korena: " << node->pointers[i] << endl;
	}
	node->numOfKeys--; // Broj cvorova u korenu se smanjio za 1
	
	return true;
}

// Pozajmljuje jedan kljuc od desnog brata (preko oca)
bool borrowFromRightBrother(Node *node, Node *parent, Node *brother)
{
	int min = (node->m + node->m % 2) / 2 - 1;

	if (brother != nullptr && brother->numOfKeys > min) {		// Nisam skroz levi sin i desni brat ima dovoljno
		cout << "Zajmimo od desnog brata -> Desni brat: " << brother << endl;
		int dividingPos = parent->findPos(node->keys[node->numOfKeys - 1]);

		int brotherKey = brother->keys[0];					// Kljuc koji pozajmljujemo
		Node *brotherFirstPointer = brother->pointers[0];	// Njegov levi pokazivac
		// Pomeranje kljuceva u desnom bratu za jedno mesto levo
		for (int i = 0; i < brother->numOfKeys - 1; i++)
			brother->keys[i] = brother->keys[i + 1];
		for (int i = 0; i < brother->numOfKeys; i++)
			brother->pointers[i] = brother->pointers[i + 1];
		brother->numOfKeys--;	// Broj kljuceva u bratu se smanjio za 1
		if (brotherFirstPointer) brotherFirstPointer->parent = node;	// Postavljanje odgovarajuceg cvora oca
		
		node->pointers[node->numOfKeys + 1] = brotherFirstPointer;	// Prabacivanje pokazivaca
		node->keys[node->numOfKeys] = parent->keys[dividingPos];	// Razdelni iz oca na kraj naseg cvora
		parent->keys[dividingPos] = brotherKey;						// Iz brata kljuc u oca
		node->numOfKeys++;											// Broj kljuceva se povecao za 1

		return true;	// Kraj, uspeli smo da pozajmimo od desnog brata
	}
	return false;
}

// Pozajmljuje jedan kljuc od levog brata (preko oca)
bool borrowFromLeftBrother(Node *brother, Node *parent, Node *node)
{
	int min = (node->m + node->m % 2) / 2 - 1;

	if (brother != nullptr && brother->numOfKeys > min) {						// Nisam skroz levi sin i levi brat ima dovoljno
		int dividingPos = parent->findPos(node->keys[node->numOfKeys - 1]);
		dividingPos--;

		cout << "Zajmimo od levog brata -> Levi brat: " << brother << endl;
		int brotherKey = brother->keys[brother->numOfKeys - 1];					// Kljuc koji pozajmljujemo
		Node *brotherLastPointer = brother->pointers[brother->numOfKeys];		// Njegov levi pokazivac

		brother->numOfKeys--;	// Broj kljuceva u bratu se smanjio za 1

		if (brotherLastPointer) brotherLastPointer->parent = node;	// Postavljanje odgovarajuceg cvora oca


		// Pomeranje kljuceva u originalnom cvoru za 1 mesto u desno da bi na prvu poziciju smestili pozajmljeni kljuc
		for (int i = node->numOfKeys; i > 0; i--)
			node->keys[i] = node->keys[i - 1];
		for (int i = node->numOfKeys + 1; i > 0; i--)	// Ako smo spajali cvor, spojili smo ga u levi cvor, pa gubimo samo desni pokazivac
			node->pointers[i] = node->pointers[i - 1];

		node->pointers[0] = brotherLastPointer;			// Prabacivanje pokazivaca
		node->keys[0] = parent->keys[dividingPos];		// Razdelni iz oca na kraj naseg cvora
		parent->keys[dividingPos] = brotherKey;			// Iz brata kljuc u oca
		node->numOfKeys++;								// Broj kljuceva se povecao za 1

		return true;	// Kraj, uspeli smo da pozajmimo od levog brata
		
	}
	return false;
}