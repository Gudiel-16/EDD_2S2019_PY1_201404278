#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void menu();
void opcionesMenu(int);
void lecturaArchivoCSV();
void guardarConfig(string);
void modificarCapaEspecificaDeCuboNegativo(string);
void modificarTodasLasCapasDeCuboNegativo();
string obtenerRGBnegativo(string);

int config[4]; //guardar la configuracion de la imagen (width ancho)(height alto)(pix width)(pix height)

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
	//para creacion de matriz
	nodoMatriz insertarOrdenadoColumna(nodoMatriz, nodoMatriz);
	nodoMatriz insertarOrdenadoFila(nodoMatriz, nodoMatriz);
	nodoMatriz crearColumna(int);
	nodoMatriz crearFila(int);
	//imprimir
	void imprimirAbajoLuegoSiguiente();
	void imprimirSiguienteLuegoAbajo();
	Matriz(); //constructor
	void insertarElemento(int,int,string); //insertar en matriz


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

	// 1 CASO: COLUMNA NO EXISTA Y FILA NO EXISTA ####################################################################
	if (Nodcolumna==0 && Nodfila==0)
	{
		//printf("CASO1\n");
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
	else if (Nodcolumna == 0 && Nodfila == 1){ // 2 CASO: COLUNMA NO EXISTA Y FILA EXISTA ###########################################
		//printf("CASO2\n");
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
	else if (Nodcolumna == 1 && Nodfila == 0){ // 3 CASO: COLUMNA EXISTA Y LA FILA NO EXISTA ########################################
		//printf("CASO3\n");
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
	else if (Nodcolumna == 1 && Nodfila == 1){ // 4 CASO: COLUMNA Y LA FILA EXISTEN ###############################################
		//printf("CASO4\n");
		bool bandeColum = false;
		bool bandeFil = false;

		//BUSCAR FILA
		nodoMatriz *BuscarFila = this->raiz;
		nodoMatriz *filaa = NULL;
		while (BuscarFila != NULL)
		{
			if (BuscarFila->y == _y)
			{
				filaa = BuscarFila;
				break;
			}
			BuscarFila = BuscarFila->abajo;
		}

		//BUSCAR COLUMNA
		nodoMatriz *BuscarColumn = this->raiz;
		nodoMatriz *columnaa = NULL;
		while (BuscarColumn != NULL)
		{
			if (BuscarColumn->x == _x)
			{
				columnaa = BuscarColumn;
				break;
			}
			BuscarColumn = BuscarColumn->siguiente;
		}		

		//INSERTAMOS NODO DE FORMA ORDENADA EN COLUMNA (nuevo, nodoFila) //temp=nodoFila
		
		bandeColum = false;
		while (true)
		{
			if (filaa->x == nuevo->x) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				filaa->y = nuevo->y;
				filaa->dato = nuevo->dato;
				break;
			}
			else if (filaa->x > nuevo->x){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeColum = true;
				break;
			}
			if (filaa->siguiente != NULL)
			{
				filaa = filaa->siguiente;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}
					
			
		//INSERTAMOS NODO DE FORMA ORDENADA EN FILA (nuevo, nodoColumna) //temp=nodoColumna
		
		bandeFil = false;

		while (true)
		{
			if (columnaa->y == nuevo->y) // SI LA POSICION ES LA MISMA SOBRE ESCRIBO
			{
				columnaa->x = nuevo->x;
				columnaa->dato = nuevo->dato;
				break;
			}
			else if (columnaa->y > nuevo->y){ // TENGO QUE INSERTARLO ANTES DE TEMP
				bandeFil = true;
				break;
			}
			if (columnaa->abajo != NULL)
			{
				columnaa = columnaa->abajo;
			}
			else{ // TENGO QUE INSERTAR NUEVO DESPUES DE TEMP, Y BANDECOLUM SERA FALSE
				break;
			}
		}
		
		//BUSCAR SI YA EXISTE ESA COORDENADA PARA AGREGARLO O NO
		nodoMatriz *buscarcoor = this->raiz;
		bool BuscarCoordenada = false;
		while (buscarcoor != NULL)
		{
			nodoMatriz *buscarcoor2 = buscarcoor->siguiente;
			while (buscarcoor2!=NULL)
			{
				if (buscarcoor2->x == _x && buscarcoor2->y==_y)
				{
					BuscarCoordenada = true;
					break;
				}

				buscarcoor2 = buscarcoor2->siguiente;
			}
			
			buscarcoor = buscarcoor->abajo;
		}

		if (BuscarCoordenada == false)
		{
			if (bandeColum == true) // INSERTARLO ANTES DE TEMPORAL
			{
				nuevo->siguiente = filaa;
				filaa->anterior->siguiente = nuevo;
				nuevo->anterior = filaa->anterior;
				filaa->anterior = nuevo;
			}
			else{
				filaa->siguiente = nuevo;
				nuevo->anterior = filaa;
			}

			if (bandeFil == true) // INSERTARLO ANTES DE TEMPORAL
			{
				nuevo->abajo = columnaa;
				columnaa->arriba->abajo = nuevo;
				nuevo->arriba = columnaa->arriba;
				columnaa->arriba = nuevo;
			}
			else{
				columnaa->abajo = nuevo;
				nuevo->arriba = columnaa;
			}
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
			filas += "(" + to_string(aux->x) + "," + to_string(aux->y) + "," + aux->dato + ")";
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
	NodolistaArchivos(int, string); //constructor

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
	void insertarElemento(int, string); //numero y nombre de capa
	void nuevaMatriz(int, string); //enlazar nodo con matriz
	//recorrido linealizacion
	void linealizacionPorFila();
	void linealizacionPorColumna();
	void imprimir();
};

listaArchivos::listaArchivos()
{
	this->raizArch = new NodolistaArchivos(-1, "raizListaArchivo");
}

// Lista simple que ordena de mayor a menor 
void listaArchivos::insertarElemento(int _indice, string _nomArchivo)
{	
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

//me crea solamente una matriz y se enlaza al nodo de la lista archivos
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
					lectura.open(_archivo, ios::in);

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
					
					//se enlaza mi lista archivos con la matriz creada
					aux->apuntaRaizDeMatriz = a.raiz; //cada nodo de la lista archivos (aux->apuntaRaizDeMatriz) apunta a la raiz de la matriz creada
					break;
				}
				catch (exception e){
					printf("ERROR AL LEER ARCHIVO EN CREACION DE MATRIZ");
				}				
			}

			aux = aux->siguiente;
		}
	}
}

void listaArchivos::linealizacionPorFila()
{
	NodolistaArchivos *aux = this->raizArch->siguiente; //recorrera lista archivos (contiene las capas)
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			cout << "--> CAPA: " << aux->indice  << " ARCHIVO: " << aux->nomArchivo << endl;

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz
			nodoMatriz *aux3 = aux2->siguiente; // se posiciona en (0,-1) de mi matriz
			nodoMatriz *aux4 = aux3->abajo; //se posiciona en (0,0) de mi matriz

			while (aux4 != NULL) //me recorre la matriz para abajo
			{
				filas += "(" + to_string(aux4->x) + "," + to_string(aux4->y) + "," + aux4->dato + ")";
				nodoMatriz *aux5 = aux4->siguiente; //aux4 va hacia abajo, entonces aux5=aux4->siguiente para recorrer toda una fila

				while (aux5 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					filas += "(" + to_string(aux5->x) + "," + to_string(aux5->y) + "," + aux5->dato + ")";
					aux5 = aux5->siguiente;
				}

				cout << filas << endl;
				filas = "";
				aux4 = aux4->abajo;
			}

			aux = aux->siguiente;
		}

	}
}

void listaArchivos::linealizacionPorColumna()
{
	NodolistaArchivos *aux = this->raizArch->siguiente; //recorrera lista archivos (contiene las capas)
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			cout << "--> CAPA: " << aux->indice << " ARCHIVO: " << aux->nomArchivo << endl;

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz
			nodoMatriz *aux3 = aux2->siguiente; // se posiciona en (0,-1) de mi matriz
			nodoMatriz *aux4 = aux3->abajo; //se posiciona en (0,0) de mi matriz

			while (aux4 != NULL) //me recorre la matriz hacia -->
			{
				filas += "(" + to_string(aux4->x) + "," + to_string(aux4->y) + "," + aux4->dato + ")";
				nodoMatriz *aux5 = aux4->abajo; //aux4 va hacia -->, entonces aux5=aux4->abajo para recorrer toda una columna

				while (aux5 != NULL) //me recorre cada columna de la matriz (hacia abajo)
				{
					filas += "(" + to_string(aux5->x) + "," + to_string(aux5->y) + "," + aux5->dato + ")";
					aux5 = aux5->abajo;
				}

				cout << filas << endl;
				filas = "";
				aux4 = aux4->siguiente;
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
			cout << aux->indice << " <->  " << aux->nomArchivo << endl;
			
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

///////////////////////////////////////////////////////// CLASE NODO COPIA CUBO /////////////////////////////////////////////////////////////////

class NodoCopiaCubo
{
public:
	NodoCopiaCubo *raizCopCubo;
	NodoCopiaCubo *siguiente;
	nodoMatriz *apuntaRaizDeMatriz; //para enlazar el nodo lista archivo a la matriz dispersa
	int indice;
	string nomArchivo;
	NodoCopiaCubo(int, string); //constructor

};

NodoCopiaCubo::NodoCopiaCubo(int _indice, string _nomArchivo)
{
	this->siguiente = NULL;
	this->indice = _indice;
	this->nomArchivo = _nomArchivo;
}

class copiaCubo
{
public:
	NodoCopiaCubo *raizCopCubo;
	copiaCubo();
	void insertarElemento(int, string); //numero y nombre de capa
	void nuevaMatriz(int, string); //enlazar nodo con matriz
	void imprimir();
};

copiaCubo::copiaCubo()
{
	this->raizCopCubo = new NodoCopiaCubo(-1, "raizListaCopia");
}

void copiaCubo::imprimir()
{
	NodoCopiaCubo *aux = this->raizCopCubo;
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			cout << aux->indice << " <->  " << aux->nomArchivo << endl;

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				filas += "(" + to_string(aux2->x) + "," + to_string(aux2->y) + "," + aux2->dato + ")";
				nodoMatriz *aux3 = aux2->siguiente; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
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

void copiaCubo::insertarElemento(int _indice, string _archivo)
{
	NodoCopiaCubo *nuevo = new NodoCopiaCubo(_indice, _archivo);
	NodoCopiaCubo *temp = this->raizCopCubo;

	bool bandera = false;

	while (true)
	{
		if (temp->siguiente != NULL)
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

// ------------------------------------> GLOBALES <----------------------------------------

listaArchivos larch; //objeto para insertar en lista archivos y crear matriz
copiaCubo copCubo;

//se crea el cubo, va creando nodo en la la lista archivos y luego creando matriz para cada nodo
void CrearCuboDisperso()
{		
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
						//para archivo de configuracion capa 0 no me lo agregue
						if (capa!=0)
						{
							archivo = dato;							
							larch.insertarElemento(capa, archivo); //se inserta a la lista el archivo leido
							larch.nuevaMatriz(capa, archivo); //se crea una matriz para el archivo leido anteriormente
						}
						else{ //si es la capa 0
							archivo = dato;
							guardarConfig(archivo);
						}
					}
					else{
						capa = atoi(dato.c_str());
					}
					bande++;
				}
			}

			linealec++;
		}
		//larch.imprimir();
		
	}
	catch (exception e){
		printf("NO SE ENCONTRO EL ARCHIVO INICIAL");
	}	
}

void copiaCuboDisperso()
{
	NodolistaArchivos *aux = larch.raizArch->siguiente;//------------------------puede que duplique

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			//capturo el indice y nombre de archivo
			int indi = aux->indice;
			string nom = aux->nomArchivo;
			copCubo.insertarElemento(indi,nom); //lo agrego a la lista copiarcubo

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente->abajo; //se posiciona en coordenada (0,0)

			Matriz nm; //nueva matriz

			//busco indice en la lista copiacubo para que cuando lo encuentre lo enlaze
			NodoCopiaCubo *auxNodoCopia = copCubo.raizCopCubo;

			if (auxNodoCopia != NULL)
			{
				while (auxNodoCopia != NULL) //mientras sea distinto de NULL ejecuta
				{
					if (auxNodoCopia->indice == indi) //cuando ya lo encuentra entra..
					{						
							while (aux2 != NULL) //me recorre la matriz para abajo, aux2 empieza en (0,0)
							{
								nm.insertarElemento(aux2->x, aux2->y, aux2->dato); //me inserta en la matriz

								nodoMatriz *aux3 = aux2->siguiente; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila
								while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
								{
									nm.insertarElemento(aux3->x, aux3->y, aux3->dato); //me inserta en la matriz
									aux3 = aux3->siguiente;
								}
								aux2 = aux2->abajo;
							}							

							//se enlaza mi lista copiacubo con la matriz creada
							auxNodoCopia->apuntaRaizDeMatriz = nm.raiz; //cada nodo de la lista archivos (aux->apuntaRaizDeMatriz) apunta a la raiz de la matriz creada						
					}
					auxNodoCopia = auxNodoCopia->siguiente;
				}
			}			
			aux = aux->siguiente; //aux es de listaarchivos, entonces al avanzar me creara una nueva matriz
		}

	}
}

void modificarCapaEspecificaDeCuboNegativo(string nomCapa)
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string a = aux->nomArchivo;
			string b = a.substr(0, a.length()-4);

			if (b==nomCapa)
			{
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente->abajo; //se posiciona en (0,0)

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					if (aux2->dato!="x")
					{
						cout << aux2->dato << endl;
						string a = obtenerRGBnegativo(aux2->dato);
						aux2->dato = a;
					}
					nodoMatriz *aux3 = aux2->siguiente; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

					while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
					{
						if (aux3->dato != "x")
						{
							cout << aux3->dato << endl;
							string a = obtenerRGBnegativo(aux3->dato);
							aux3->dato = a;
						}
						aux3 = aux3->siguiente;
					}

					//cout << filas << endl;
					filas = "";
					aux2 = aux2->abajo;
				}
			}		

			aux = aux->siguiente;
		}

	}
}

void modificarTodasLasCapasDeCuboNegativo()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente->abajo; //se posiciona en (0,0)

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				if (aux2->dato != "x")
				{
					cout << aux2->dato << endl;
					string a = obtenerRGBnegativo(aux2->dato);
					aux2->dato = a;
				}
				nodoMatriz *aux3 = aux2->siguiente; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					if (aux3->dato != "x")
					{
						cout << aux3->dato << endl;
						string a = obtenerRGBnegativo(aux3->dato);
						aux3->dato = a;
					}
					aux3 = aux3->siguiente;
				}

				//cout << filas << endl;
				filas = "";
				aux2 = aux2->abajo;
			}			

			aux = aux->siguiente;
		}

	}
}

string obtenerRGBnegativo(string _rgb)
{
	string d = _rgb;
	int pos = d.find("-"); //primer guion
	string e = d.substr(0, pos); //obtiene rgbR
	string f = d.substr(pos + 1, d.length() - 1);//resto de cadena
	int pos2 = f.find("-");
	string g = f.substr(0, pos2);//obtiene rgbG
	string h = f.substr(pos2 + 1, f.length() - 1);//obtiene rgbB

	//obtengo numeros por separado
	int rgb1 = atoi(e.c_str());
	int rgb2 = atoi(g.c_str());
	int rgb3 = atoi(h.c_str());

	int op1 = 255 - rgb1;
	int op2 = 255 - rgb2;
	int op3 = 255 - rgb3;

	string dev = to_string(op1) + "-" + to_string(op2) + "-" + to_string(op3);

	return dev;
}


void guardarConfig(string _archivo)
{
	int bande = 1;
	int linealec = 0; //me dira en que linea me encuentro
	int tam = 0;
	string archivo = "";
	try{
		//empezara a leer el archivo.csv
		ifstream lectura;
		lectura.open(_archivo, ios::in);

		int linealec = 0;

		for (string linea; getline(lectura, linea);)
		{
			stringstream registro(linea);
			string dato;

			for (int columna = 0; getline(registro, dato, ';'); ++columna)
			{
				if (linealec > 0)
				{
					if (bande % 2 == 0) //cada dos datos insertara, para mandar guardar tamanio de imagen
					{
						int a = linealec - 1; //guardara en el indice (linea que va menos 1)
						tam = atoi(dato.c_str()); //convierte el valor a entero
						config[a] = tam; //lo asigna al vector
					}
					bande++;
				}
			}
			linealec++;
		}
		cout << config[0] << config[1] << config[2] << config[3] << endl;
	}
	catch (exception e){
		printf("ERROR AL LEER ARCHIVO EN CREACION DE MATRIZ");
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
		larch.imprimir();
		break;
	case 2:
		larch.raizArch->siguiente = NULL; //limpiar lista
		break;
	case 3:
		copiaCuboDisperso();
		break;
	case 4:
		//modificarCapaEspecificaDeCubo("boots");
		modificarTodasLasCapasDeCuboNegativo();
		break;
	case 5:
		copCubo.imprimir();
		break;
	case 6:
		printf("\n-----------------\n\n");
		//larch.linealizacionPorColumna();
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
