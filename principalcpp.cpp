#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

///////////////////////////////////////////////////////// CLASE NODO MATRIZ ///////////////////////////////////////////////////////////////////

class nodoMatriz
{
public:
	nodoMatriz *raiz;
	nodoMatriz *siguiente;
	nodoMatriz *anterior;
	nodoMatriz *abajo;
	nodoMatriz *arriba;
	int x;
	int y;
	string dato;
	nodoMatriz(int,int,string); //constructor

	//nodoMatriz buscarfila(int);
};

nodoMatriz::nodoMatriz(int _x, int _y, string _dato) //constructor
{
	this->siguiente = NULL;
	this->anterior = NULL;
	this->abajo = NULL;
	this->arriba = NULL;
	this->x = _x;
	this->y = _y;
	this->dato = _dato;
}


///////////////////////////////////////////////////////// CLASE MATRIZ ///////////////////////////////////////////////////////////////////

class Matriz
{
public:
	nodoMatriz *raiz; //raiz de mi matriz	
	bool buscarfila(int);
	bool buscarColumna(int);
	nodoMatriz insertarOrdenadoColumna(nodoMatriz, nodoMatriz);
	nodoMatriz insertarOrdenadoFila(nodoMatriz, nodoMatriz);
	nodoMatriz crearColumna(int);
	nodoMatriz crearFila(int);
	Matriz(); //constructor
	void insertarElemento(int,int,string);


};

Matriz::Matriz()
{
	this->raiz = new nodoMatriz(-1,-1,"raiz"); //inicializamos creando raiz

}

bool Matriz::buscarfila(int _y)
{
	bool bandera = false;
	nodoMatriz *temp =this->raiz;
	while (temp!=NULL)
	{
		if (temp->y==_y)
		{
			bandera = true;
		}
		temp = temp->abajo;
	}

	return bandera;
}

bool Matriz::buscarColumna(int _x)
{
	bool bandera = false;
	nodoMatriz *temp = this->raiz;
	while (temp != NULL)
	{
		if (temp->y == _x)
		{
			bandera = true;
		}
		temp = temp->siguiente;
	}

	return bandera;
}

nodoMatriz Matriz::insertarOrdenadoColumna(nodoMatriz nuevo, nodoMatriz raiz)
{
	bool bandera = false;
	nodoMatriz *temp = this->raiz;
	
}

void Matriz::insertarElemento(int _x, int _y, string dato)
{
	nodoMatriz *nuevo = new nodoMatriz(_x,_y,dato);
	nodoMatriz *nodoColumn;
	nodoMatriz *nodoFila;

	bool Nodcolumna =buscarColumna(_x);
	bool Nodfila = buscarfila(_y);

	// 1 CASO: COLUMNA NO EXISTA Y FILA NO EXISTA
	if (Nodcolumna==false && Nodfila==false)
	{
		// SE CREA COLUMNA
		nodoMatriz *temp1 = this->raiz;
		nodoMatriz *nuevo2 = new nodoMatriz(_x, -1, "COL");
		bool bandeColum = false;
		while (true)
		{
			if (temp1->x==nuevo2->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp1->y = nuevo2->y;
				temp1->dato = nuevo2->dato;
			}
			else if (temp1->x > nuevo2->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (temp1->siguiente!=NULL)
			{
				temp1 = temp1->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum==true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo2->siguiente = temp1;
			temp1->anterior->siguiente = nuevo2;
			nuevo2->anterior = temp1->anterior;
			temp1->anterior = nuevo2;
		}
		else{
			temp1->siguiente = nuevo2;
			nuevo2->anterior = temp1;
		}

		nodoColumn = nuevo2;

		//SE CREA FILA
		nodoMatriz *temp2 = this->raiz;
		nodoMatriz *nuevo3 = new nodoMatriz(-1, _y, "FIL");
		bool bandeFil = false;
		while (true)
		{
			if (temp2->y == nuevo3->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp2->x = nuevo3->x;
				temp2->dato = nuevo3->dato;
			}
			else if (temp2->y > nuevo3->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (temp2->abajo != NULL)
			{
				temp2 = temp2->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo3->abajo = temp2;
			temp2->arriba->abajo = nuevo3;
			nuevo3->arriba = temp2->arriba;
			temp2->arriba = nuevo3;
		}
		else{
			temp2->abajo = nuevo3;
			nuevo3->arriba = temp2;
		}

		nodoFila = nuevo3;

	}
	else if (Nodcolumna == false && Nodfila == true){ // 2 CASO: COLUNMA NO EXISTA Y FILA EXISTA

	}
	else if (Nodcolumna == true && Nodfila == false){ // 3 CASO: COLUMNA EXISTA Y LA FILA NO EXISTA

	}
	else if (Nodcolumna == true && Nodfila == true){ // 4 CASO: COLUMNA Y LA FILA EXISTEN

	}
}


int main()
{
	nodoMatriz p1 = nodoMatriz(-1, -1, "s");

	system("pause");
	return 0;
}

void lecturaArchivoCSV()
{
	ifstream lectura;
	lectura.open("archivo.csv", ios::in);

	int linealec = 0;

	for (string linea; getline(lectura, linea);)
	{
		stringstream registro(linea);
		string dato;

		for (int columna = 0; getline(registro, dato, ';'); ++columna)
		{
			cout << "LINEA: " << linealec << " COLUMNA: " << columna << " " << dato << endl;
		}

		linealec++;
	}
}