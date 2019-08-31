#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

void menu();
void opcionesMenu(int);
void lecturaArchivoCSV();
void CrearCuboDisperso();
void copiaCuboDisperso();
void modificarCapaEspecificaDeCuboNegativo(string);
void modificarTodasLasCapasDeCuboNegativo();
bool verificarSiExisteCapa(string);
string obtenerRGBnegativo(string);
void filtroEspejoEjeX();
void filtroEspejoEjeY();
void ReporteGraphvizTodasLasCapas();
void ReporteGraphvizLinealizacionPorFilaTodasCapas();
void ReporteGraphvizLinealizacionPorColumnaTodasCapas();
void guardarConfig(string);
void graphvizEscrituraParaCapa(string, string);
void graphvizEscrituraParaLinealizacion(string, string);
void guardarCuboEnListaDoble(string);

void subMenuFiltros();
void subsubMenuFiltrosOp1();
void subsubMenuFiltrosOp2();

string rgbR(int num);
string rgbG(int num);
string rgbB(int num);
string separarRGB_Y_Extraer_Hexa(string);

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
	this->raiz = new nodoMatriz(-1,-1,"RAIZ"); //inicializamos creando raiz

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
							if ((dato.compare("x") != 0) && (dato.compare("X") != 0)) //no agrega x's
							{
								a.insertarElemento(columna, linealec, dato); //me ingresa en la matriz con las coordenadas especificas
							}
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
		while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
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

///////////////////////////////////////////////////////////////////// PILA /////////////////////////////////////////////////////////////////////////
class NodoPila{

public:
	NodoPila *siguiente;
	string valor;
	NodoPila(string); //constructor
};

NodoPila::NodoPila(string _valor)
{
	this->siguiente = NULL;
	this->valor = _valor;
}

class Pila{
public:
	NodoPila *raizPila;
	Pila(); //constructor
	void insertar(string);
	string eliminar();
	void imprimir();
};

Pila::Pila(){
	this->raizPila = new NodoPila("raizPila");
}

void Pila::insertar(string _valor)
{
	NodoPila *nuevo = new NodoPila(_valor);
	NodoPila *temp = this->raizPila;
	
	//agrego al principio
	if (temp->siguiente == NULL) //cuando aun no hay nodos
	{
		temp->siguiente = nuevo;
	}
	else{
		nuevo->siguiente = temp->siguiente;
		temp->siguiente = nuevo;
	}
}

string Pila::eliminar()
{
	NodoPila *temp = this->raizPila;	
	string dato = "";

	//elimino el primer valor
	if (temp->siguiente!=NULL)
	{
		NodoPila *temp2 = temp->siguiente;

		dato = temp2->valor; //capturo el primer valor

		temp->siguiente = temp2->siguiente;
		temp2->siguiente = NULL;
	}	

	return dato;
}

void Pila::imprimir()
{
	NodoPila *temp = this->raizPila;
	while (temp!=NULL)
	{
		cout << temp->valor << endl;
		
		temp = temp->siguiente;
	}
}

///////////////////////////////////////////////////////////////////// LISTA CIRCULAR /////////////////////////////////////////////////////////////////////////

class nodoFiltro{
public:
	nodoFiltro *anterior;
	nodoFiltro *siguiente;
	NodoCopiaCubo *apuntaCopiaCubo;
	string nomFiltro;
	nodoFiltro(string); //constructor
};

nodoFiltro::nodoFiltro(string _filtro){
	this->anterior = NULL;
	this->siguiente = NULL;
	this->apuntaCopiaCubo = NULL;
	this->nomFiltro = _filtro;
}

class listaDobCircuFiltros{
public:
	nodoFiltro *primero;
	nodoFiltro *ultimo;
	listaDobCircuFiltros();//constructor
	void insertar(string);
	void imprimir();
	void vaciar();
};

listaDobCircuFiltros::listaDobCircuFiltros()
{
	this->primero = NULL;
	this->ultimo = NULL;
}

void listaDobCircuFiltros::insertar(string _filtro)
{
	nodoFiltro *nuevo = new nodoFiltro(_filtro);

	if (this->primero==NULL)//si esta vacia
	{
		this->primero = nuevo;
		this->primero->siguiente = primero;
		nuevo->anterior = ultimo;
		this->ultimo = nuevo;
		
	}else{

		this->ultimo->siguiente = nuevo;
		nuevo->siguiente = primero;
		nuevo->anterior = ultimo;
		this->ultimo = nuevo;
		this->primero->anterior = ultimo;

	}
}

void listaDobCircuFiltros::imprimir()
{
	if (this->primero!=NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{
			cout << actual->nomFiltro << " <--->  "<< endl;
			if (actual->apuntaCopiaCubo!=NULL)
			{
				NodoCopiaCubo *aux = actual->apuntaCopiaCubo;
				string filas = "";

				if (aux != NULL)
				{
					while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
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
			actual = actual->siguiente;
		} while (actual != primero);
	}	

	
}

void listaDobCircuFiltros::vaciar()
{
	this->primero = NULL;
	this->ultimo = NULL;
}

//////////////////////////////////////////////////////////////////// LISTA PARA LINEALIZACION /////////////////////////////////////////////////////////////////

class nodoLinealizacion{
public:
	nodoLinealizacion *siguiente;
	string dato;
	nodoLinealizacion(string); //constructor
};

nodoLinealizacion::nodoLinealizacion(string _dato)
{
	this->siguiente = NULL;
	this->dato = _dato;
}

class listaLinealizacion{
public:
	nodoLinealizacion *raizListLinealiz;
	nodoLinealizacion *primero;
	nodoLinealizacion *ultimo;
	listaLinealizacion(); //constructor
	void insertar(string);
	string textoGraphviz();
	void imprimir();
	void vaciar();
};

listaLinealizacion::listaLinealizacion()
{
	this->raizListLinealiz = new nodoLinealizacion("INICIO");
	this->primero = NULL;
	this->ultimo = NULL;
}

void listaLinealizacion::insertar(string _dato)
{
	nodoLinealizacion *nuevo = new nodoLinealizacion(_dato);

	if (this->raizListLinealiz->siguiente==NULL) //vacia
	{
		this->raizListLinealiz->siguiente = nuevo;
		this->primero = nuevo;
		this->ultimo = nuevo;
	}
	else{
		this->ultimo->siguiente = nuevo;
		this->ultimo = nuevo;
	}
}

string listaLinealizacion::textoGraphviz()
{
	string cadena = "";
	nodoLinealizacion *temp = this->raizListLinealiz;
	while (temp != NULL)
	{
		if (temp->siguiente!=NULL)
		{
			cadena += temp->dato + "->" + temp->siguiente->dato;
		}
		temp = temp->siguiente;
	}
	return cadena;
}

void listaLinealizacion::imprimir()
{
	nodoLinealizacion *temp = this->raizListLinealiz;
	while (temp!=NULL)
	{
		cout << temp->dato << endl;
		temp = temp->siguiente;
	}
}

void listaLinealizacion::vaciar()
{
	this->raizListLinealiz->siguiente = NULL;
	this->primero = NULL;
	this->ultimo = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------> GLOBALES <----------------------------------------

listaArchivos larch; //(cubo original)
copiaCubo copCubo; // (copia cubo)
Pila miPila; // (ayuda para filtros en ejes)
listaDobCircuFiltros listFiltros;
listaLinealizacion listLinealizacion;

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

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en coordenada (-1,0)

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
								nodoMatriz *aux3 = aux2->siguiente; //se posiciona en el siguiente de la cabecera Y

								nm.insertarElemento(aux3->x, aux3->y, aux3->dato); //me inserta en la matriz

								nodoMatriz *aux4 = aux3->siguiente; //se posiciona en la columa (1)

								while (aux4 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
								{
									nm.insertarElemento(aux4->x, aux4->y, aux4->dato); //me inserta en la matriz
									aux4 = aux4->siguiente;
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
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en (-1,0)

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					nodoMatriz *temp = aux2->siguiente;//pos (0,0)

					while (temp != NULL)
					{
						string a = obtenerRGBnegativo(temp->dato);
						temp->dato = a;
						temp = temp->siguiente;
					}

					aux2 = aux2->abajo;
				}
			}

			aux = aux->siguiente;
		}

	}
}

bool verificarSiExisteCapa(string nomCapa)
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	bool bandera = false;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string a = aux->nomArchivo;
			string b = a.substr(0, a.length() - 4);

			if (b == nomCapa)
			{
				bandera = true;
			}
			aux = aux->siguiente;
		}
	}
	return bandera;
}

void modificarTodasLasCapasDeCuboNegativo()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en (-1,0)

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				nodoMatriz *temp = aux2->siguiente;//pos (0,0)

				while (temp!=NULL)
				{
					string a = obtenerRGBnegativo(temp->dato);
					temp->dato = a;
					temp = temp->siguiente;
				}

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

void filtroEspejoEjeX()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	miPila.raizPila->siguiente = NULL; //limpio Pila

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente->abajo; //se posiciona en (0,0)

			while (aux2 != NULL) //me recorre la matriz para abajo
			{				
				nodoMatriz *aux3 = aux2; //(primer recorrido) para que empieze en columna 0 hacia -->
				nodoMatriz *aux4 = aux2; //(segundo recorrido) para que empieze en columna 0 hacia -->

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					miPila.insertar(aux3->dato); //inserto en pila
					aux3 = aux3->siguiente;
				}

				while (aux4 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					string ob = miPila.eliminar(); //elimino y extraigo en pila
					aux4->dato = ob; //modifico en matriz
					aux4 = aux4->siguiente;
				}

				aux2 = aux2->abajo;
			}
			aux = aux->siguiente;
		}

	}
}

void filtroEspejoEjeY()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	miPila.raizPila->siguiente = NULL; //limpio Pila

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente->abajo; //se posiciona en (0,0)

			while (aux2 != NULL) //me recorre la matriz para -->
			{
				nodoMatriz *aux3 = aux2; //(primer recorrido) para que empieze en columna 0 hacia bajo
				nodoMatriz *aux4 = aux2; //(segundo recorrido) para que empieze en columna 0 hacia bajo

				while (aux3 != NULL) //me recorre cada columna de la matriz 
				{
					miPila.insertar(aux3->dato); //inserto en pila
					aux3 = aux3->abajo;
				}

				while (aux4 != NULL) //me recorre cada columna de la matriz 
				{
					string ob = miPila.eliminar(); //elimino y extraigo en pila
					aux4->dato = ob; //modifico en matriz
					aux4 = aux4->abajo;
				}

				aux2 = aux2->siguiente;
			}
			aux = aux->siguiente;
		}

	}
}

void ReporteGraphvizTodasLasCapas()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	string cadena = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			cadena = "";
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv

			//nodos que me serviran para los pasos correspondientes
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz (-1,-1)
			nodoMatriz *aux3 = aux->apuntaRaizDeMatriz;
			nodoMatriz *aux4 = aux->apuntaRaizDeMatriz->siguiente; //se posiciona en (-1,0)
			nodoMatriz *aux5 = aux->apuntaRaizDeMatriz->siguiente;
			nodoMatriz *aux6 = aux->apuntaRaizDeMatriz;
			nodoMatriz *aux7 = aux->apuntaRaizDeMatriz->siguiente->abajo; //me posiciono en (0,0)

			//-->PASO 1: recororo toda mi cabecera Y para crear los label de la cabecera Y
			while (aux2 != NULL) 
			{
				cadena += "\"(" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "\"" + "[ label = " + "\"" + "(" + to_string(aux2->x) + "," + to_string(aux2->y) + ") " + aux2->dato + "\"" + ", width = 1.5, style = filled, fillcolor = firebrick1, group =" + to_string(aux2->x) + "] \n";
				aux2 = aux2->abajo;
			}

			//-->PASO 2: recorro cabecera Y para hacer las relaciones de los nodos (anterior se crean los label) aqui se hacen las relaciones (solo entre cabecera Y)
			while (aux3!=NULL) 
			{
				if (aux3->abajo!=NULL)
				{
					cadena += "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->abajo->x) + "," + to_string(aux3->abajo->y) + ")" + "\""+"\n";
					cadena += "\"(" + to_string(aux3->abajo->x) + "," + to_string(aux3->abajo->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\""+"\n";
				}
				aux3 = aux3->abajo;
			}

			//-->PASO 3: recororo toda mi cabecera X para crear los label de la cabecera X
			while (aux4 != NULL) 
			{
				cadena += "\"(" + to_string(aux4->x) + "," + to_string(aux4->y) + ")" + "\"" + "[ label = " + "\"" + "(" + to_string(aux4->x) + "," + to_string(aux4->y) + ") " + aux4->dato + "\"" + ", width = 1.5, style = filled, fillcolor = firebrick1, group =" + to_string(aux4->x) + "] \n";
				aux4 = aux4->siguiente;
			}

			//-->PASO 4: recorro cabecera X para hacer las relaciones de los nodos (anterior se crean los label) aqui se hacen las relaciones (solo entre cabecera X)
			while (aux5 != NULL) 
			{				
				cadena += "\"(" + to_string(aux5->x) + "," + to_string(aux5->y) + ")" + "\"" + "->" + "\"(" + to_string(aux5->anterior->x) + "," + to_string(aux5->anterior->y) + ")" + "\"" + "\n";
				cadena += "\"(" + to_string(aux5->anterior->x) + "," + to_string(aux5->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux5->x) + "," + to_string(aux5->y) + ")" + "\"" + "\n";
				
				aux5 = aux5->siguiente;
			}

			//-->PASO 5: aqui hago que mis filas se elineen segun la columna que le corresponde (group)
			cadena += "{rank=same;";

			while (aux6 != NULL) 
			{				
				cadena += "\"(" + to_string(aux6->x) + "," + to_string(aux6->y) + ")" + "\"" + ";";
				
				aux6 = aux6->siguiente;
			}
			cadena += "} \n";

			//-->PASO 6: recorro mi matriz desde (0,0) hacia abajo
			while (aux7!=NULL)
			{
				//(PASO 7,8,9,10 se ejecuta cada vez que aux7 da una vuelta)
				//-->PASO 7
				nodoMatriz *aux8 = aux7;
				nodoMatriz *aux9 = aux7;
				nodoMatriz *aux10 = aux7->anterior; // aux7->anterior para que me alinee la fila con la cabecera en Y (para que tome en cuenta la cabecera Y )

				//-->PASO 8: recororo toda mi fila en X para crear los label
				while (aux8!=NULL) 
				{
					cadena += "\"(" + to_string(aux8->x) + "," + to_string(aux8->y) + ")" + "\"" + "[ label = " + "\"" + "(" + to_string(aux8->x) + "," + to_string(aux8->y) + ") " + aux8->dato + "\"" + ", width = 1.5, style = filled, group =" + to_string(aux8->x) + "] \n";
					
					aux8 = aux8->siguiente;
				}
				//-->PASO 9: ya creados los label anteriormente hago las relaciones entre nodos
				while (aux9!=NULL)
				{
					cadena += "\"(" + to_string(aux9->x) + "," + to_string(aux9->y) + ")" + "\"" + "->" + "\"(" + to_string(aux9->anterior->x) + "," + to_string(aux9->anterior->y) + ")" + "\"" + "\n";
					cadena += "\"(" + to_string(aux9->anterior->x) + "," + to_string(aux9->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux9->x) + "," + to_string(aux9->y) + ")" + "\"" + "\n";
					cadena += "\"(" + to_string(aux9->arriba->x) + "," + to_string(aux9->arriba->y) + ")" + "\"" + "->" + "\"(" + to_string(aux9->x) + "," + to_string(aux9->y) + ")" + "\"" + "\n";
					cadena += "\"(" + to_string(aux9->x) + "," + to_string(aux9->y) + ")" + "\"" + "->" + "\"(" + to_string(aux9->arriba->x) + "," + to_string(aux9->arriba->y) + ")" + "\"" + "\n";
					
					aux9 = aux9->siguiente;
				}

				//-->PASO 10: aqui hago que mis filas se elineen segun la columna que le corresponde (group)
				cadena += "{rank=same;";

				while (aux10 != NULL) // recorro X para hacer la linealizacion de nodos
				{
					cadena += "\"(" + to_string(aux10->x) + "," + to_string(aux10->y) + ")" + "\"" + ";";					
					aux10 = aux10->siguiente;
				}
				cadena += "} \n";

				aux7 = aux7->abajo;
			}

			graphvizEscrituraParaCapa(nomCap, cadena);
			aux = aux->siguiente;
		}

	}
}

void ReporteGraphvizLinealizacionPorFilaTodasCapas()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente; 
	string cadena = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la lista en este sentido -> -> ->
		{
			cadena = "";
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv
			nomCap += "LinealizacionPorFila";

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //(-1,0)

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				nodoMatriz *aux3 = aux2->siguiente; //

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{		
					cadena = "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ") " + aux3->dato + "\"";
					listLinealizacion.insertar(cadena);
					aux3 = aux3->siguiente;
				}

				aux2 = aux2->abajo;
			}
			string cad = listLinealizacion.textoGraphviz();
			graphvizEscrituraParaLinealizacion(nomCap, cad);
			listLinealizacion.vaciar();
			aux = aux->siguiente;
		}
	}
}

void ReporteGraphvizLinealizacionPorColumnaTodasCapas()
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	string cadena = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la lista en este sentido -> -> ->
		{
			cadena = "";
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv
			nomCap += "LinealizacionPorColumna";

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente; //(0,-1)

			while (aux2 != NULL) //me recorre la matriz para -> ->
			{
				nodoMatriz *aux3 = aux2->abajo; //

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					cadena = "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ") " + aux3->dato + "\"";
					listLinealizacion.insertar(cadena);
					aux3 = aux3->abajo;
				}

				aux2 = aux2->siguiente;
			}
			string cad = listLinealizacion.textoGraphviz();
			graphvizEscrituraParaLinealizacion(nomCap, cad);
			listLinealizacion.vaciar();
			aux = aux->siguiente;
		}
	}
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

void graphvizEscrituraParaCapa(string _nomCapa, string _texto)
{
	ofstream archivo;
	archivo.open(_nomCapa+".dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph G { \n";
	archivo << "node [shape=box]; \n";
	archivo << "graph [splines=ortho, nodesep=0.5]; \n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	char* charr;
	string dott = "dot -Tpng " + _nomCapa + ".dot -o " + _nomCapa + ".png";
	charr = (char *)dott.c_str();
	system(charr);
	//system("nohup display ruta_y_nombre_de_imagen_generada.png &");
}

void graphvizEscrituraParaLinealizacion(string _nomCapa, string _texto)
{
	ofstream archivo;
	archivo.open(_nomCapa + ".dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph G { \n";
	archivo << "rankdir=\"LR\" \n";
	archivo << "node [shape = record,height=.1,width=0.9]; \n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	char* charr;
	string dott = "dot -Tpng " + _nomCapa + ".dot -o " + _nomCapa + ".png";
	charr = (char *)dott.c_str();
	system(charr);
}

void guardarCuboEnListaDoble(string _filtro)
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;

		do
		{
			if (actual->nomFiltro==_filtro) //cuando encuentre el nombre del filtro
			{
				if (actual->apuntaCopiaCubo==NULL) // si aun no hay cubo en ese nodo
				{
					actual->apuntaCopiaCubo = copCubo.raizCopCubo; //se enlaza o guarda en el nodo el cubo
					break;
				}
				else{
					printf("\n--> YA APLICO ESTE FILTRO A LA IMAGEN!\n");
				}
				
			}
			actual = actual->siguiente;

		} while (actual != listFiltros.primero);
	}
}

void generarHTML()
{
	string cadena = "";
	cadena += "<!DOCTYPE html> \n";
	cadena += "<head> \n";
	cadena += "<link rel = \"stylesheet\" href = \"si.css\"> \n";
	cadena += "</head> \n";
	cadena += "<body> \n";
	cadena += "<div class = \"canvas\"> \n";

	int image_width = config[0];
	int image_height = config[1];
	int pixel_width = config[2];
	int pixel_height = config[3];

	int op = image_width * image_height;

	for (int i = 0; i < op; i++)
	{
		cadena += "<div class=\"pixel\"></div> \n";
	}

	cadena += "</div> \n";
	cadena += "</body> \n";
	cadena += "</html> \n";

	ofstream archivo;
	archivo.open("prueba.html", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	archivo << cadena;

	archivo.close();
}

void generarCSS()
{
	string cadena = "";
	cadena += ".canvas { \n";
	cadena += "background: #bed10f; \n";

	int image_width = config[0];
	int image_height = config[1];
	int pixel_width = config[2];
	int pixel_height = config[3];
	int op = image_width * pixel_width;

	cadena += "width: " + to_string(op) + "px; \n";
	cadena += "height: " + to_string(op) + "px; \n";
	cadena += " } \n";
	cadena += ".pixel{ \n";
	cadena += "width: " + to_string(pixel_width) + "px; \n";
	cadena += "height: " + to_string(pixel_height) + "px; \n";
	cadena += "float:left; \n";
	cadena += "} \n";
	
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la lista en este sentido -> -> ->
		{
			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //(-1,0)

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				nodoMatriz *aux3 = aux2->siguiente; //

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					//FORMULA: (FILAACTUAL*TAMANIOPIXEL)+(COLUMNAACTUAL+1)
					int filaActual = 0;
					int tamPixel = 0;
					int columnaActual = 0;
					int columnaActualn = 0;
					int op1 = 0;
					int op2 = 0;
					filaActual = aux3->y; //FILA ACTUAL
					tamPixel = config[0]; //TAMANIOPIXEL
					columnaActual = aux3->x;
					columnaActualn = columnaActual + 1;//COLUMNAACTUAL +1
					op1 = tamPixel*filaActual; // (FILAACTUAL*TAMANIOPIXEL)
					op2 = op1 + columnaActualn; // (FILAACTUAL*TAMANIOPIXEL)+(COLUMNAACTUAL+1)
					string hexad = separarRGB_Y_Extraer_Hexa(aux3->dato);
					cadena += ".pixel:nth-child(" + to_string(op2) + "){ \n";
					cadena += "background: " + hexad + "; \n";
					cadena += "} \n\n";

					aux3 = aux3->siguiente;
				}

				aux2 = aux2->abajo;
			}
			aux = aux->siguiente;
		}
	}

	ofstream archivo;
	archivo.open("si.css", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	archivo << cadena;

	archivo.close();

}

///////////////////////////////////////////////////////// CONVERSION RGB A HEXA ///////////////////////////////////////////////////////////////

string separarRGB_Y_Extraer_Hexa(string cad)
{
	string d = cad;
	int pos = d.find("-"); //primer guion
	string e = d.substr(0, pos); //obtiene rgbR
	string f = d.substr(pos + 1, d.length() - 1);//resto de cadena
	int pos2 = f.find("-");
	string g = f.substr(0, pos2);//obtiene rgbG
	string h = f.substr(pos2 + 1, f.length() - 1);//obtiene rgbB

	int rgb1 = atoi(e.c_str());
	int rgb2 = atoi(g.c_str());
	int rgb3 = atoi(h.c_str());

	string a = rgbR(rgb1);
	string b = rgbG(rgb2);
	string c = rgbR(rgb3);

	string hexa = "#";
	hexa += a;
	hexa += b;
	hexa += c;

	return hexa;
}

int devolverValor(double num)
{
	double a, b;
	double aux;
	a = 1;
	b = 1;
	aux = 1;

	while (true)
	{
		aux = a / b;

		if (aux<num) //si aux es menor que el numero recibido
		{
			a++;
		}
		else if (aux>num){ //si aux es mayor que el numero recibido
			a--;
			b++;
		}

		if (aux == num) // si ya encontro la division se sale
		{
			break;
		}
	}

	while (b < 10) //mientras b (denominador) sea < 10
	{
		a = a * 2;
		b = b * 2;
	}

	return (int)a;

}

string rgbR(int num)
{
	string cadena = "";
	double a = 16;
	int b = num / 16; //valor exacto 
	double c = num / a; //valor con decimal
	double d = c - b; //residuo
	int e = devolverValor(d);

	if (b == 10)
	{
		cadena += "A";
	}
	else if (b == 11){
		cadena += "B";
	}
	else if (b == 12){
		cadena += "B";
	}
	else if (b == 13){
		cadena += "D";
	}
	else if (b == 14){
		cadena += "E";
	}
	else if (b == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(b);
	}

	if (e == 10)
	{
		cadena += "A";
	}
	else if (e == 11){
		cadena += "B";
	}
	else if (e == 12){
		cadena += "B";
	}
	else if (e == 13){
		cadena += "D";
	}
	else if (e == 14){
		cadena += "E";
	}
	else if (e == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(e);
	}

	return cadena;

}

string rgbG(int num)
{
	string cadena = "";
	double a = 16;
	int b = num / 16; //valor exacto 
	double c = num / a; //valor con decimal
	double d = c - b; //residuo
	int e = devolverValor(d);

	if (b == 10)
	{
		cadena += "A";
	}
	else if (b == 11){
		cadena += "B";
	}
	else if (b == 12){
		cadena += "B";
	}
	else if (b == 13){
		cadena += "D";
	}
	else if (b == 14){
		cadena += "E";
	}
	else if (b == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(b);
	}

	if (e == 10)
	{
		cadena += "A";
	}
	else if (e == 11){
		cadena += "B";
	}
	else if (e == 12){
		cadena += "B";
	}
	else if (e == 13){
		cadena += "D";
	}
	else if (e == 14){
		cadena += "E";
	}
	else if (e == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(e);
	}

	return cadena;
}

string rgbB(int num)
{
	string cadena = "";
	double a = 16;
	int b = num / 16; //valor exacto 
	double c = num / a; //valor con decimal
	double d = c - b; //residuo
	int e = devolverValor(d);

	if (b == 10)
	{
		cadena += "A";
	}
	else if (b == 11){
		cadena += "B";
	}
	else if (b == 12){
		cadena += "B";
	}
	else if (b == 13){
		cadena += "D";
	}
	else if (b == 14){
		cadena += "E";
	}
	else if (b == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(b);
	}

	if (e == 10)
	{
		cadena += "A";
	}
	else if (e == 11){
		cadena += "B";
	}
	else if (e == 12){
		cadena += "B";
	}
	else if (e == 13){
		cadena += "D";
	}
	else if (e == 14){
		cadena += "E";
	}
	else if (e == 15){
		cadena += "F";
	}
	else{
		cadena += to_string(e);
	}

	return cadena;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

		printf("\n--> ELIJA UNA OPCION: \n");
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
		//larch.imprimir();
		copiaCuboDisperso();
		copCubo.imprimir();
		filtroEspejoEjeX();
		copCubo.imprimir();
		break;
	case 2:
		//listFiltros.imprimir();
		//larch.raizArch->siguiente = NULL; //limpiar lista
		break;
	case 3:
		subMenuFiltros();
		break;
	case 4:
		break;
	case 5:
		generarHTML();
		generarCSS();
		break;
	case 6:
		//ReporteGraphvizTodasLasCapas();
		//ReporteGraphvizLinealizacionPorFilaTodasCapas();
		//ReporteGraphvizLinealizacionPorColumnaTodasCapas();
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

/////////////////////////////////////////////////////////////// SUB MENU FILTROS //////////////////////////////////////////////////////////////////////////////

void subMenuFiltros()
{
	string op1;
	printf(" \n -------FILTROS------ \n");
	printf("1. Filtro a Imagen Completa \n");
	printf("2. Filtro a Una Capa en Especifico \n");
	printf("\n ELIJA UNA OPCION: \n");
	cin >> op1;
	if (op1.compare("1")==0)
	{
		subsubMenuFiltrosOp1();
	}
	else if (op1.compare("2") == 0){
		subsubMenuFiltrosOp2();
	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
}

void subsubMenuFiltrosOp1() // Imagen completa
{
	string op2;
	printf(" \n -------TIPOS DE FILTROS------ \n");
	printf("1. Negative (Negativo) \n");
	printf("2. Grayscale (Escala De Grises) \n");
	printf("3. X-Mirror (Espejo en eje X) \n");
	printf("4. Y-Mirror (Espejo en eje Y) \n");
	printf("5. Double Mirror (Espejo En Ambos Ejes) \n");
	printf("\n--> ELIJA UNA OPCION: \n");
	cin >> op2;
	if (op2.compare("1") == 0)
	{
		modificarTodasLasCapasDeCuboNegativo();
		listFiltros.insertar("NegativoEnTodasCapas"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("NegativoEnTodasCapas"); //busca el nodo y lo enlaza al cubo
	}
	else if (op2.compare("2") == 0)
	{
		
	}
	else if (op2.compare("3") == 0)
	{

	}
	else if (op2.compare("4") == 0)
	{

	}
	else if (op2.compare("5") == 0)
	{

	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
	
}

void subsubMenuFiltrosOp2() // A capa especifica
{
	string op2;
	printf(" \n -------TIPOS DE FILTROS------ \n");
	printf("1. Negative (Negativo) \n");
	printf("2. Grayscale (Escala De Grises) \n");
	printf("\n--> ELIJA UNA OPCION: \n");
	cin >> op2;
	if (op2.compare("1") == 0)
	{
		string ncap;
		cin >> ncap;
		if (verificarSiExisteCapa(ncap)==true)
		{
			modificarCapaEspecificaDeCuboNegativo(ncap);
			listFiltros.insertar("NegativoEnCapa"+ncap); //se guarda primero en la lista doble
			guardarCuboEnListaDoble("NegativoEnCapa" + ncap); //busca el nodo y lo enlaza al cubo
		}
		else{
			printf("\n--> NO EXISTE LA CAPA \n");
		}
	}
	else if (op2.compare("2") == 0)
	{

	}	
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
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
