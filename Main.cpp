#include <iostream>
#include <fstream>
#include "Tree.hpp"

using namespace std;


int menu()
{
	cout << "MENI:" << endl;
	cout << "1. Red stabla" << endl;
	cout << "2. Ubaci u stablo" << endl;
	cout << "3. Obrisi iz stabla" << endl;
	cout << "4. Uredi stablo" << endl;
	cout << "5. Ispisi stablo" << endl;
	cout << "6. Kraj" << endl;

	int option;
	cin >> option;
	return option;
}



int main()
{
/*
	int option = 0;
	Tree *t = nullptr;

	while (option != 6) {
		option = menu();
		switch (option)
		{
		case 1:
			delete t;
			int m;
			cout << "Unesite red stabla: ";
			cin >> m;
			t = new Tree(m);
			break;
		case 2:
			int k;
			cout << "Kljuc: ";
			cin >> k;
			if (t->insertKey(k)) {
				cout << "Uspesno umetanje" << endl;
			}
			else {
				cout << "Kljuc se vec nalazi u stablu" << endl;
			}
			break;
		case 3:
			int kk;
			cout << "Kljuc: ";
			cin >> k;
			if (t->deleteKey(k)) {
				cout << "Uspesno brisanje" << endl;
			}
			else {
				cout << "Kljuc se ne nalazi u stablu" << endl;
			}
			break;
		case 4:
			//UREDI
			break;
		case 5:
			t->levelOrder();
			cout << "------------------------" << endl << endl;
			break;
		case 6:
			// Kraj rada
			delete t;
			cout << endl << endl << "BYE BYE";
			break;
		default:
			cout << "Nepostojeca opcija" << endl;
			break;
		}
	}

*/
	// TEST PRIMER koji pokriva sve slucajeve umetanja i brisanja
	// Dodavanje u list gde ima dovoljno mesta, cepanje lista, cepanje na vise nivoa, povecanje visine
	// Uklanjanje iz lista gde ostaje dovoljono kljuceva, uklanjanje iz unutrasnjeg cvora, pozajmica od desnog brata,
	// pozajmica od levog brata, spajanje sa desnim bratom, spajanje sa levim bratom

	// 95 66 40 42 50 61 26 7 14 31 25 1 78 55 15
	int arr[10000];
	ifstream file;
	int size = 0;

	file.open("numbers_10_unique.txt");
	if (file.is_open())
		while (!file.eof())
			file >> arr[size++];
	else
		exit(2);
	file.close();

	Tree *t = new Tree(5);	// Najbolji prikaz je za red 5, ali moze i za 4 tu se vidi kako prebacuje u list
	for (int i = 0; i < size; i++) {
		t->insertKey(arr[i]);
	}

	t->insertKey(0);
	t->insertKey(5);
	t->insertKey(45);
	t->insertKey(20);
	t->insertKey(21);
	t->insertKey(47);

	t->inOrder();	// InOrder obilazak, ispis treba da bude sortiran niz kljuceva

	// Stampamo broj kljuceva i visinu stabla
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;

	// Neuspesna brisanja
	t->deleteKey(691);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	
	// Brisanja nakon kojih ce se videti promene
	t->deleteKey(1);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(78);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(5);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	
	t->modifyTree();
	cout << "Modifikovanje stabla" << endl;
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	cout << "+-+-+-+-+-+-" << endl << endl;

	t->deleteKey(7);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(14);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(15);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(31);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(26);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(61);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(25);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(0);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(45);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	
	// 78 je neuspesno brisanje jer taj kljuc ne postoji u stablu
	t->deleteKey(78);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(95);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(47);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(55);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(66);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(21);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(20);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(40);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	t->deleteKey(50);
	cout << t << endl << endl;
	cout << "Broj kljuceva: " << t->getNumOfKeys() << endl << "Visina stabla: " << t->getHeight() << endl << endl;
	delete t;
	

	return 0;
}