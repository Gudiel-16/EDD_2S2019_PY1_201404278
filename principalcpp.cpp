#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void menu();
void opcionesMenu(int);

void lecturaArchivoCSV();

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
	nodoMatriz insertarOrdenadoColumna(nodoMatriz, nodoMatriz);
	nodoMatriz insertarOrdenadoFila(nodoMatriz, nodoMatriz);
	nodoMatriz crearColumna(int);
	nodoMatriz crearFila(int);
	void imprimirAbajoLuegoSiguiente();
	void imprimirSiguienteLuegoAbajo();
	Matriz(); //constructor
	void insertarElemento(int,int,string);


};

Matriz::Matriz()
{
	this->raiz = new nodoMatriz(-1,-1,"raiz"); //inicializamos creando raiz

}

void Matriz::insertarElemento(int _x, int _y, string dato)
{
	nodoMatriz *nuevo = new nodoMatriz(_x,_y,dato);
	nodoMatriz *nodoColumn=NULL;
	nodoMatriz *nodoFila=NULL;	

	//BUSCAR FILA
	int banderaBuscarFila = 0;
	nodoMatriz *tempBuscarFila = this->raiz;
	while (tempBuscarFila != NULL)
	{
		if (tempBuscarFila->y == _y)
		{
			banderaBuscarFila = 1;
		}
		tempBuscarFila = tempBuscarFila->abajo;
	}

	//BUSCAR COLUMNA
	int banderaBuscarColumn = 0;
	nodoMatriz *tempBuscarColumn = this->raiz;
	while (tempBuscarColumn != NULL)
	{
		if (tempBuscarColumn->x == _x)
		{
			banderaBuscarColumn = 1;
		}
		tempBuscarColumn = tempBuscarColumn->siguiente;
	}

	int Nodcolumna = banderaBuscarColumn;
	int Nodfila = banderaBuscarFila;

	// 1 CASO: COLUMNA NO EXISTA Y FILA NO EXISTA
	if (Nodcolumna==0 && Nodfila==0)
	{
		// SE CREA COLUMNA
		nodoMatriz *temp1 = this->raiz;
		nodoMatriz *nuevo2 = new nodoMatriz(_x, -1, "COL");
		bool bandeColum = false;
		bool bandeColumIguales = false;
		while (true)
		{
			if (temp1->x==nuevo2->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp1->y = nuevo2->y;
				temp1->dato = nuevo2->dato;
				bandeColumIguales = true;
				break;
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

		if (bandeColumIguales==true)
		{
			nodoColumn = temp1;;
		}
		else{
			nodoColumn = nuevo2;
		}		

		//SE CREA FILA
		nodoMatriz *temp2 = this->raiz;
		nodoMatriz *nuevo3 = new nodoMatriz(-1, _y, "FIL");
		bool bandeFil = false;
		bool bandeFilIguales = false;

		while (true)
		{
			if (temp2->y == nuevo3->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp2->x = nuevo3->x;
				temp2->dato = nuevo3->dato;
				bandeFilIguales = true;
				break;
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

		if (bandeFilIguales==true)
		{
			nodoFila = temp2;
		}
		else{
			nodoFila = nuevo3;
		}

		
		//INSERTAMOS NODO DE FORMA ORDENADA EN COLUMNA (nuevo, nodoFila) //temp=nodoFila
		bandeColum = false;
		bandeColumIguales = false;
		while (true)
		{
			if (nodoFila->x == nuevo->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoFila->y = nuevo->y;
				nodoFila->dato = nuevo->dato;
				bandeColumIguales = true;
				break;
			}
			else if (nodoFila->x > nuevo->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (nodoFila->siguiente != NULL)
			{
				nodoFila = nodoFila->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->siguiente = nodoFila;
			nodoFila->anterior->siguiente = nuevo;
			nuevo->anterior = nodoFila->anterior;
			nodoFila->anterior = nuevo;
		}
		else{
			nodoFila->siguiente = nuevo;
			nuevo->anterior = nodoFila;
		}

		
		//INSERTAMOS NODO DE FORMA ORDENADA EN FILA (nuevo, nodoColumna) //temp=nodoColumna
		bandeFil = false;
		bandeFilIguales = false;

		while (true)
		{
			if (nodoColumn->y == nuevo->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoColumn->x = nuevo->x;
				nodoColumn->dato = nuevo->dato;
				bandeFilIguales = true;
				break;
			}
			else if (nodoColumn->y > nuevo->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (nodoColumn->abajo != NULL)
			{
				nodoColumn = nodoColumn->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->abajo = nodoColumn;
			nodoColumn->arriba->abajo = nuevo;
			nuevo->arriba = nodoColumn->arriba;
			nodoColumn->arriba = nuevo;
		}
		else{
			nodoColumn->abajo = nuevo;
			nuevo->arriba = nodoColumn;
		}
		
	}
	else if (Nodcolumna == 0 && Nodfila == 1){ // 2 CASO: COLUNMA NO EXISTA Y FILA EXISTA
		
		// SE CREA COLUMNA
		nodoMatriz *temp1 = this->raiz;
		nodoMatriz *nuevo2 = new nodoMatriz(_x, -1, "COL");
		bool bandeColum = false;
		bool bandeColumIguales = false;
		while (true)
		{
			if (temp1->x == nuevo2->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp1->y = nuevo2->y;
				temp1->dato = nuevo2->dato;
				bandeColumIguales = true;
				break;
			}
			else if (temp1->x > nuevo2->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (temp1->siguiente != NULL)
			{
				temp1 = temp1->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
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

		if (bandeColumIguales == true)
		{
			nodoColumn = temp1;;
		}
		else{
			nodoColumn = nuevo2;
		}

		//SE CREA FILA
		nodoMatriz *temp2 = this->raiz;
		nodoMatriz *nuevo3 = new nodoMatriz(-1, _y, "FIL");
		bool bandeFil = false;
		bool bandeFilIguales = false;

		while (true)
		{
			if (temp2->y == nuevo3->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp2->x = nuevo3->x;
				temp2->dato = nuevo3->dato;
				bandeFilIguales = true;
				break;
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

		/*if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo3->abajo = temp2;
			temp2->arriba->abajo = nuevo3;
			nuevo3->arriba = temp2->arriba;
			temp2->arriba = nuevo3;
		}
		else{
			temp2->abajo = nuevo3;
			nuevo3->arriba = temp2;
		}*/

		if (bandeFilIguales == true)
		{
			nodoFila = temp2;
		}
		else{
			nodoFila = nuevo3;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN COLUMNA (nuevo, nodoFila) //temp=nodoFila
		bandeColum = false;
		bandeColumIguales = false;
		while (true)
		{
			if (nodoFila->x == nuevo->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoFila->y = nuevo->y;
				nodoFila->dato = nuevo->dato;
				bandeColumIguales = true;
				break;
			}
			else if (nodoFila->x > nuevo->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (nodoFila->siguiente != NULL)
			{
				nodoFila = nodoFila->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->siguiente = nodoFila;
			nodoFila->anterior->siguiente = nuevo;
			nuevo->anterior = nodoFila->anterior;
			nodoFila->anterior = nuevo;
		}
		else{
			nodoFila->siguiente = nuevo;
			nuevo->anterior = nodoFila;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN FILA (nuevo, nodoColumna) //temp=nodoColumna
		bandeFil = false;
		bandeFilIguales = false;

		while (true)
		{
			if (nodoColumn->y == nuevo->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoColumn->x = nuevo->x;
				nodoColumn->dato = nuevo->dato;
				bandeFilIguales = true;
				break;
			}
			else if (nodoColumn->y > nuevo->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (nodoColumn->abajo != NULL)
			{
				nodoColumn = nodoColumn->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->abajo = nodoColumn;
			nodoColumn->arriba->abajo = nuevo;
			nuevo->arriba = nodoColumn->arriba;
			nodoColumn->arriba = nuevo;
		}
		else{
			nodoColumn->abajo = nuevo;
			nuevo->arriba = nodoColumn;
		}

	}
	else if (Nodcolumna == 1 && Nodfila == 0){ // 3 CASO: COLUMNA EXISTA Y LA FILA NO EXISTA
		
		// SE CREA COLUMNA
		nodoMatriz *temp1 = this->raiz;
		nodoMatriz *nuevo2 = new nodoMatriz(_x, -1, "COL");
		bool bandeColum = false;
		bool bandeColumIguales = false;
		while (true)
		{
			if (temp1->x == nuevo2->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp1->y = nuevo2->y;
				temp1->dato = nuevo2->dato;
				bandeColumIguales = true;
				break;
			}
			else if (temp1->x > nuevo2->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (temp1->siguiente != NULL)
			{
				temp1 = temp1->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		//if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		//{
		//	nuevo2->siguiente = temp1;
		//	temp1->anterior->siguiente = nuevo2;
		//	nuevo2->anterior = temp1->anterior;
		//	temp1->anterior = nuevo2;
		//}
		//else{
		//	temp1->siguiente = nuevo2;
		//	nuevo2->anterior = temp1;
		//}

		if (bandeColumIguales == true)
		{
			nodoColumn = temp1;;
		}
		else{
			nodoColumn = nuevo2;
		}

		//SE CREA FILA
		nodoMatriz *temp2 = this->raiz;
		nodoMatriz *nuevo3 = new nodoMatriz(-1, _y, "FIL");
		bool bandeFil = false;
		bool bandeFilIguales = false;

		while (true)
		{
			if (temp2->y == nuevo3->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp2->x = nuevo3->x;
				temp2->dato = nuevo3->dato;
				bandeFilIguales = true;
				break;
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

		if (bandeFilIguales == true)
		{
			nodoFila = temp2;
		}
		else{
			nodoFila = nuevo3;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN COLUMNA (nuevo, nodoFila) //temp=nodoFila
		bandeColum = false;
		bandeColumIguales = false;
		while (true)
		{
			if (nodoFila->x == nuevo->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoFila->y = nuevo->y;
				nodoFila->dato = nuevo->dato;
				bandeColumIguales = true;
				break;
			}
			else if (nodoFila->x > nuevo->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (nodoFila->siguiente != NULL)
			{
				nodoFila = nodoFila->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->siguiente = nodoFila;
			nodoFila->anterior->siguiente = nuevo;
			nuevo->anterior = nodoFila->anterior;
			nodoFila->anterior = nuevo;
		}
		else{
			nodoFila->siguiente = nuevo;
			nuevo->anterior = nodoFila;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN FILA (nuevo, nodoColumna) //temp=nodoColumna
		bandeFil = false;
		bandeFilIguales = false;

		while (true)
		{
			if (nodoColumn->y == nuevo->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoColumn->x = nuevo->x;
				nodoColumn->dato = nuevo->dato;
				bandeFilIguales = true;
				break;
			}
			else if (nodoColumn->y > nuevo->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (nodoColumn->abajo != NULL)
			{
				nodoColumn = nodoColumn->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->abajo = nodoColumn;
			nodoColumn->arriba->abajo = nuevo;
			nuevo->arriba = nodoColumn->arriba;
			nodoColumn->arriba = nuevo;
		}
		else{
			nodoColumn->abajo = nuevo;
			nuevo->arriba = nodoColumn;
		}

	}
	else if (Nodcolumna == 1 && Nodfila == 1){ // 4 CASO: COLUMNA Y LA FILA EXISTEN

		// SE CREA COLUMNA
		nodoMatriz *temp1 = this->raiz;
		nodoMatriz *nuevo2 = new nodoMatriz(_x, -1, "COL");
		bool bandeColum = false;
		bool bandeColumIguales = false;
		while (true)
		{
			if (temp1->x == nuevo2->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp1->y = nuevo2->y;
				temp1->dato = nuevo2->dato;
				bandeColumIguales = true;
				break;
			}
			else if (temp1->x > nuevo2->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (temp1->siguiente != NULL)
			{
				temp1 = temp1->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		/*if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		{
		nuevo2->siguiente = temp1;
		temp1->anterior->siguiente = nuevo2;
		nuevo2->anterior = temp1->anterior;
		temp1->anterior = nuevo2;
		}
		else{
		temp1->siguiente = nuevo2;
		nuevo2->anterior = temp1;
		}*/

		if (bandeColumIguales == true)
		{
			nodoColumn = temp1;;
		}
		else{
			nodoColumn = nuevo2;
		}

		//SE CREA FILA
		nodoMatriz *temp2 = this->raiz;
		nodoMatriz *nuevo3 = new nodoMatriz(-1, _y, "FIL");
		bool bandeFil = false;
		bool bandeFilIguales = false;

		while (true)
		{
			if (temp2->y == nuevo3->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				temp2->x = nuevo3->x;
				temp2->dato = nuevo3->dato;
				bandeFilIguales = true;
				break;
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
		/*
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
		}*/

		if (bandeFilIguales == true)
		{
			nodoFila = temp2;
		}
		else{
			nodoFila = nuevo3;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN COLUMNA (nuevo, nodoFila) //temp=nodoFila
		bandeColum = false;
		bandeColumIguales = false;
		while (true)
		{
			if (nodoFila->x == nuevo->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoFila->y = nuevo->y;
				nodoFila->dato = nuevo->dato;
				bandeColumIguales = true;
				break;
			}
			else if (nodoFila->x > nuevo->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (nodoFila->siguiente != NULL)
			{
				nodoFila = nodoFila->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->siguiente = nodoFila;
			nodoFila->anterior->siguiente = nuevo;
			nuevo->anterior = nodoFila->anterior;
			nodoFila->anterior = nuevo;
		}
		else{
			nodoFila->siguiente = nuevo;
			nuevo->anterior = nodoFila;
		}


		//INSERTAMOS NODO DE FORMA ORDENADA EN FILA (nuevo, nodoColumna) //temp=nodoColumna
		bandeFil = false;
		bandeFilIguales = false;

		while (true)
		{
			if (nodoColumn->y == nuevo->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				nodoColumn->x = nuevo->x;
				nodoColumn->dato = nuevo->dato;
				bandeFilIguales = true;
				break;
			}
			else if (nodoColumn->y > nuevo->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (nodoColumn->abajo != NULL)
			{
				nodoColumn = nodoColumn->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}

		if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
		{
			nuevo->abajo = nodoColumn;
			nodoColumn->arriba->abajo = nuevo;
			nuevo->arriba = nodoColumn->arriba;
			nodoColumn->arriba = nuevo;
		}
		else{
			nodoColumn->abajo = nuevo;
			nuevo->arriba = nodoColumn;
		}

	}
}

void Matriz::imprimirAbajoLuegoSiguiente()
{
	nodoMatriz *temp = this->raiz;
	string filas = "";

	while (temp!=NULL)
	{
		//cout << "X: " << temp->x << " Y: " << temp->y << " Dato: " << temp->dato << endl;
		filas += "(" + to_string(temp->x) + "," + to_string(temp->y) + ")";
		nodoMatriz *aux = temp->siguiente;
		while (aux!=NULL)
		{
			//cout << "X: " << aux->x << " Y: " << aux->y << " Dato: " << aux->dato << endl;
			filas += "(" + to_string(aux->x) + "," + to_string(aux->y) + ")";
			aux = aux->siguiente;
		}
		cout << filas << endl;
		filas = "";
		temp = temp->abajo;
	}
}

void Matriz::imprimirSiguienteLuegoAbajo()
{
	nodoMatriz *temp = this->raiz;

	while (temp != NULL)
	{
		cout << "X: " << temp->x << " Y: " << temp->y << " Dato: " << temp->dato << endl;

		nodoMatriz *aux = temp->abajo;
		while (aux != NULL)
		{
			cout << "X: " << aux->x << " Y: " << aux->y << " Dato: " << aux->dato << endl;
			aux = aux->abajo;
		}
		temp = temp->siguiente;
	}
}


///////////////////////////////////////////////////////// CLASE NODO LISTA ARCHIVOS /////////////////////////////////////////////////////////////////

class NodolistaArchivos
{
public:
	NodolistaArchivos *raizArch;
	NodolistaArchivos *siguiente;
	nodoMatriz *apuntaRaizDeMatriz; //para enlazar el nodo lista archivo a la matriz dispersa
	int indice;
	string nomArchivo;
	NodolistaArchivos(int, string);

};

NodolistaArchivos::NodolistaArchivos(int _indice, string _nomArchivo)
{
	this->siguiente = NULL;
	this->indice = _indice;
	this->nomArchivo = _nomArchivo;
}

///////////////////////////////////////////////////////// CLASE LISTA ARCHIVOS ///////////////////////////////////////////////////////////////////

class listaArchivos
{
public:
	NodolistaArchivos *raizArch;
	listaArchivos();
	void insertarElemento(int, string);
	void nuevaMatriz(int, string);
	void imprimir();
};

listaArchivos::listaArchivos()
{
	this->raizArch = new NodolistaArchivos(-1, "raizListaArchivo");
}

void listaArchivos::insertarElemento(int _indice, string _nomArchivo)
{
	//LISTA SIMPLE QUE ORDENA DE MAYOR A MENOR
	NodolistaArchivos *nuevo = new NodolistaArchivos(_indice, _nomArchivo);
	NodolistaArchivos *temp = this->raizArch;

	bool bandera = false;

	while (true)
	{
		if (temp->siguiente!=NULL)
		{
			if (temp->siguiente->indice<_indice)
			{
				bandera = true; 
				break;
			}
		}
		if (temp->siguiente != NULL)
		{
			temp = temp->siguiente;
		}
		else{
			break;
		}
	}

	if (bandera == true) 
	{
		//si el siguiente es menor, lo inserto antes
		nuevo->siguiente = temp->siguiente; 
		temp->siguiente = nuevo;
	}
	else{
		//lo inserto de ultimo
		temp->siguiente = nuevo;
	}
}

void listaArchivos::nuevaMatriz(int _capa, string _archivo)
{
	Matriz a; //objeto para ingresar a matriz
	NodolistaArchivos *aux = this->raizArch;

	if (aux!=NULL)
	{
		while (aux!=NULL) //mientras sea distinto de NULL ejecuta
		{
			if (aux->indice==_capa) //cuando ya lo encuentra, buscara lo que hay en el archivo y creara una matriz con ese archivo
			{
				try{

					//empezara a leer el archivo.csv
					ifstream lectura;
					lectura.open(aux->nomArchivo, ios::in);

					int linealec = 0;

					for (string linea; getline(lectura, linea);)
					{
						stringstream registro(linea);
						string dato;

						for (int columna = 0; getline(registro, dato, ';'); ++columna)
						{
							a.insertarElemento(columna, linealec, dato); //me ingresa en la matriz con las coordenadas especificas
						}

						linealec++;
					}

					aux->apuntaRaizDeMatriz = a.raiz; //se enlaza mi lista archivos con la matriz creada

				}
				catch (exception e){
					printf("ERROR AL LEER ARCHIVO EN CREACION DE MATRIZ");
				}				
			}

			aux = aux->siguiente;
		}
	}
}

void listaArchivos::imprimir()
{
	NodolistaArchivos *aux = this->raizArch;
	string filas = "";

	if (aux!=NULL)
	{
		while (aux!=NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			cout << aux->indice << " <LISTA ARCHIVO>  " << aux->nomArchivo << endl;
			
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

			while (aux2!=NULL) //me recorre la matriz para abajo
			{
				filas += "(" + to_string(aux2->x) + "," + to_string(aux2->y) + "," + aux2->dato + ")";
				nodoMatriz *aux3 = aux2->siguiente; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

				while (aux3!=NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					filas += "(" + to_string(aux3->x) + "," + to_string(aux3->y) + "," + aux3->dato + ")";
					aux3 = aux3->siguiente;
				}

				cout << filas << endl;
				filas = "";				
				aux2 = aux2->abajo;
			}

			aux = aux->siguiente;
		}

	}
}

void CrearCuboDisperso()
{	
	listaArchivos larch; //objeto para insertar en lista archivos y crear matriz	
	
	int bande = 1; // bandera para que me ayude a insetar
	int capa = 0;
	string archivo="";	

	//me lee el archivo principal
	try{
		ifstream lectura;
		lectura.open("archivo.csv", ios::in);
		int linealec = 0; //me dira en que linea me encuentro

		for (string linea; getline(lectura, linea);)
		{
			stringstream registro(linea);
			string dato;

			for (int columna = 0; getline(registro, dato, ';'); ++columna)
			{
				if (linealec > 0)
				{
					if (bande % 2 == 0) //cada dos datos insertara, para mandar a insertar fila con sus dos columnas
					{
						archivo = dato;
						larch.insertarElemento(capa, archivo); //se inserta a la lista el archivo leido
						larch.nuevaMatriz(capa, archivo); //se crea una matriz para el archivo leido anteriormente
					}
					else{
						capa = atoi(dato.c_str());
					}
					bande++;
				}
			}

			linealec++;
		}
		larch.imprimir();
		
	}
	catch (exception e){
		printf("NO SE ENCONTRO EL ARCHIVO INICIAL");
	}	
}

void menu()
{
	int opcion;
	do{
		printf(" \n -------MENU------ \n");
		printf("1. Insertar Imagen \n");
		printf("2. Seleccionar Imagen \n");
		printf("3. Aplicar Filtros \n");
		printf("4. Edicion Manual \n");
		printf("5. Exportar Imagen \n");
		printf("6. Reportes \n");
		printf("7. Salir \n");

		printf("\n ELIJA UNA OPCION: \n");
		cin >> opcion;
		opcionesMenu(opcion);
	} while (true);
}

void opcionesMenu(int _opcion)
{
	switch (_opcion)
	{
	case 1:
		CrearCuboDisperso();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		exit(0);
	default:
		printf("\n<-!!- OPCION INVALIDA -!!-> \n");
	}
}

int main()
{
	menu();

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
			cout << " " << linealec << " " << columna << " " << dato << endl;
		}

		linealec++;
	}
}
