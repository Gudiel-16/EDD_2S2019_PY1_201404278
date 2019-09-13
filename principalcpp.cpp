#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <direct.h>


using namespace std;

void menu();
void opcionesMenu(string);
void lecturaArchivoCSV();
void CrearCuboDisperso(string);
void copiaCuboDisperso();
void modificarCapaEspecificaDeCuboNegativo(string);
void modificarTodasLasCapasDeCuboNegativo();
bool verificarSiExisteCapa(string);
string obtenerRGBnegativo(string);
string obtenerRGBgrayscale(string);
void filtroEspejoEjeX();
void filtroEspejoEjeY();
void ReporteGraphvizTodasLasCapasListaFiltros(string);
void ReporteGraphvizCapaEspecificaListaFiltros(string, int);
void ReporteGraphvizLinealizacionPorFilaTodasCapas();
void ReporteGraphvizLinealizacionPorColumnaTodasCapas();
void guardarConfig(string);
void graphvizEscrituraParaCapa(string, string);
void graphvizEscrituraParaCapaDesdeArbol(string, string, string);
void graphvizEscrituraParaLinealizacion(string, string);
void graphvizEscrituraParaLinealizacionDesdeArbol(string,string,string);
void graphvizEscrituraParaRecorrido(string, string);
void guardarCuboEnListaDoble(string);

void subMenuInsertarImagen();
void subMenuSeleccionarImagen();
void subMenuFiltros();
void subsubMenuFiltrosOp1();
void subsubMenuFiltrosOp2();
void subMenuEdicionManual();
void subMenuEdicionManualOp1();
void subMenuEdicionManualOp2();
void subMenuReportes();
void subMenuReportesOp2();
void subMenuReporteOp3();
void subMenuReporteOp4();
void subMenuReporteOp5();
void subMenuExportarImagen();

string rgbR(int num);
string rgbG(int num);
string rgbB(int num);
string separarRGB_Y_Extraer_Hexa(string);

int config[4]; //guardar la configuracion de la imagen (width ancho)(height alto)(pix width)(pix height
int arrDimImage[2] = {0}; //numero de pixeles para html
int arrDimImageCss[4] = {0,0,0,0};
string nombreDeImagen = "";
string nombreFiltroActual = "";
bool banderaMosaico = false;

void guardarImageOriginal(string);

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
	bool buscarCoordenadasXY(string,int,int);
	void modificarCoordenadaXY(string, int, int,string);
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
						
						for (int columna = 0; getline(registro, dato, ','); ++columna)
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

bool listaArchivos::buscarCoordenadasXY(string capa,int x, int y)
{
	NodolistaArchivos *aux = this->raizArch->siguiente;
	bool encontrado = false;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv	

			if (nomCap.compare(capa)==0)
			{
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					nodoMatriz *aux3 = aux2; //aux2 va hacia abajo, entonces aux3=aux2 para recorrer toda una fila

					while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
					{
						if ((aux3->x==x)&&(aux3->y==y))
						{
							encontrado = true;
						}
						aux3 = aux3->siguiente;
					}

					aux2 = aux2->abajo;
				}
			}		

			aux = aux->siguiente;
		}

	}
	return encontrado;
}

void listaArchivos::modificarCoordenadaXY(string capa, int x, int y, string rgb)
{
	NodolistaArchivos *aux = this->raizArch->siguiente;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv	

			if (nomCap.compare(capa) == 0)
			{
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					nodoMatriz *aux3 = aux2; //aux2 va hacia abajo, entonces aux3=aux2 para recorrer toda una fila

					while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
					{
						if ((aux3->x == x) && (aux3->y == y))
						{
							aux3->dato = rgb;
						}
						aux3 = aux3->siguiente;
					}

					aux2 = aux2->abajo;
				}
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
	int obtenerCoordenadaYmayorDeCapa(string);
	void vaciar();
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

void copiaCubo::vaciar()
{
	this->raizCopCubo->siguiente = NULL;
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

int copiaCubo::obtenerCoordenadaYmayorDeCapa(string nomCapa)
{
	NodoCopiaCubo *aux = this->raizCopCubo->siguiente;
	int yyy = 0;

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
		{
			string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
			string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv	
			if (nomCap.compare(nomCapa)==0)
			{
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					yyy = aux2->y;
					aux2 = aux2->abajo;
				}

				break;
			}	
			aux = aux->siguiente;
		}
	}
	return yyy;
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

///////////////////////////////////////////////////////////////////// LISTA CIRCULAR (FILTROS) /////////////////////////////////////////////////////////////////

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
	string filtrosAplicados();
	string reporteFiltros();
	bool existeFiltro(string);
	bool existeCapaEnFiltroEspecifico(string,int);
	bool existeCoordEnCapaDeFiltroEspecifico(string,string,int,int);
	void modificarCoordendasXY(string, string, int, int, string);
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

string listaDobCircuFiltros::filtrosAplicados()
{
	string cadena = "";
	int cont = 1;

	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		cadena = "\n-->FILTROS YA APLICADOS:\n";

		do
		{
			cadena += to_string(cont) + ". " + actual->nomFiltro + "\n";
			cont++;
			actual = actual->siguiente;
		} while (actual != primero);
	}
	else{
		cadena = "\n-->AUN NO HAY FILTROS APLICADOS! \n";
	}
	return cadena;
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

string listaDobCircuFiltros::reporteFiltros()
{
	string cadena = "";
	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{
			cadena += "\"" + actual->nomFiltro + "\" -> \"" + actual->siguiente->nomFiltro + "\" [dir=both]; \n";			

			actual = actual->siguiente;
		} while (actual != primero);
	}
	return cadena;
}

bool listaDobCircuFiltros::existeFiltro(string nomF)
{
	bool encontrado = false;

	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{
			if (actual->nomFiltro.compare(nomF)==0)
			{
				encontrado = true;
			}

			actual = actual->siguiente;
		} while (actual != primero);
	}

	return encontrado;
}

bool listaDobCircuFiltros::existeCapaEnFiltroEspecifico(string nomF, int cap)
{
	bool encontrado = false;

	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{
			if (actual->nomFiltro.compare(nomF)==0)
			{
				if (actual->apuntaCopiaCubo != NULL)
				{
					NodoCopiaCubo *aux = actual->apuntaCopiaCubo;

					if (aux != NULL)
					{
						while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
						{
							if (aux->indice==cap)
							{
								encontrado = true;
							}
							aux = aux->siguiente;
						}
					}
				}
			}
			
			actual = actual->siguiente;
		} while (actual != primero);
	}

	return encontrado;
}

bool listaDobCircuFiltros::existeCoordEnCapaDeFiltroEspecifico(string nomf, string nomcap, int _x, int _y)
{
	bool encontrado = false;

	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{			
			if (actual->nomFiltro.compare(nomf)==0) //cuando encuentre nombre de filtro
			{
				if (actual->apuntaCopiaCubo != NULL)
				{
					NodoCopiaCubo *aux = actual->apuntaCopiaCubo;

					if (aux != NULL)
					{
						while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
						{
							string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
							string nomCapp = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv	

							if (nomCapp.compare(nomcap)==0) //si encuentra la capa
							{
								nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

								while (aux2 != NULL) //me recorre la matriz para abajo
								{
									nodoMatriz *aux3 = aux2; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

									while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
									{
										if (aux3->x==_x && aux3->y==_y) //si encuentra las coordenadas
										{
											encontrado = true;
										}

										aux3 = aux3->siguiente;
									}

									aux2 = aux2->abajo;
								}
							}

							aux = aux->siguiente;
						}

					}

				}
			}

			actual = actual->siguiente;
		} while (actual != primero);
	}

	return encontrado;
}

void listaDobCircuFiltros::modificarCoordendasXY(string nomf, string nomcap, int _x, int _y, string rgb)
{
	if (this->primero != NULL)
	{
		nodoFiltro *actual = this->primero;
		do
		{
			if (actual->nomFiltro.compare(nomf) == 0) //cuando encuentre nombre de filtro
			{
				if (actual->apuntaCopiaCubo != NULL)
				{
					NodoCopiaCubo *aux = actual->apuntaCopiaCubo;

					if (aux != NULL)
					{
						while (aux != NULL) //me recorre la lista copia cubo en este sentido -> -> ->
						{
							string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
							string nomCapp = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv	

							if (nomCapp.compare(nomcap) == 0) //si encuentra la capa
							{
								nodoMatriz *aux2 = aux->apuntaRaizDeMatriz; //se posiciona en la raiz de la matriz

								while (aux2 != NULL) //me recorre la matriz para abajo
								{
									nodoMatriz *aux3 = aux2; //aux2 va hacia abajo, entonces aux3=aux2->siguiente para recorrer toda una fila

									while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
									{
										if (aux3->x == _x && aux3->y == _y) //si encuentra las coordenadas
										{
											aux3->dato = rgb;
										}

										aux3 = aux3->siguiente;
									}

									aux2 = aux2->abajo;
								}
							}

							aux = aux->siguiente;
						}

					}

				}
			}

			actual = actual->siguiente;
		} while (actual != primero);
	}
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

//////////////////////////////////////////////////////////////////// IMAGE ORIGINAL /////////////////////////////////////////////////////////////////

class nodoImageOriginal{
public:
	nodoImageOriginal *anterior;
	nodoImageOriginal *siguiente;
	NodolistaArchivos *apuntaCuboOriginal;
	string nomImage;
	nodoImageOriginal(string); //constructor
};

nodoImageOriginal::nodoImageOriginal(string nomImag)
{
	this->anterior = NULL;
	this->siguiente = NULL;
	this->apuntaCuboOriginal = NULL;
	this->nomImage = nomImag;
}

class imageOriginal{
public:
	nodoImageOriginal *primero;
	nodoImageOriginal *ultimo;
	imageOriginal();//constructor
	void insertar(string);
	void imprimir();
	void vaciar();
};

imageOriginal::imageOriginal()
{
	this->primero = NULL;
	this->ultimo = NULL;
}

void imageOriginal::insertar(string nomImag)
{
	nodoImageOriginal *nuevo = new nodoImageOriginal(nomImag);

	if (this->primero == NULL)//si esta vacia
	{
		this->primero = nuevo;
		this->primero->siguiente = primero;
		nuevo->anterior = ultimo;
		this->ultimo = nuevo;

	}
	else{

		this->ultimo->siguiente = nuevo;
		nuevo->siguiente = primero;
		nuevo->anterior = ultimo;
		this->ultimo = nuevo;
		this->primero->anterior = ultimo;

	}
}

void imageOriginal::imprimir()
{
	if (this->primero != NULL)
	{
		nodoImageOriginal *actual = this->primero;
		do
		{
			cout << actual->nomImage << " <--->  " << endl;
			if (actual->apuntaCuboOriginal != NULL)
			{
				NodolistaArchivos *aux = actual->apuntaCuboOriginal;
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

void imageOriginal::vaciar()
{
	this->primero = NULL;
	this->ultimo = NULL;
}

//////////////////////////////////////////////////////////////////// ARBOL BINARIO /////////////////////////////////////////////////////////////////

class nodoArbol{
public:
	nodoArbol *derecho;
	nodoArbol *izquierdo;
	NodolistaArchivos *cuboOriginal;
	string nomImg;
	int imgWidth;
	int imgHeight;
	int pixWidth;
	int pixHeight;
	nodoArbol(string, int, int, int, int);
};

nodoArbol::nodoArbol(string n, int iw, int ih, int pw, int ph)
{
	this->derecho = NULL;
	this->izquierdo = NULL;
	this->cuboOriginal = NULL;
	this->nomImg = n;
	this->imgWidth = iw;
	this->imgHeight = ih;
	this->pixWidth = pw;
	this->pixHeight = ph;
}

//////////////////////////////////////////////////////////////////// DIMENSION COLLAGE /////////////////////////////////////////////////////////////////
class nodoDimensioCollage{
public:
	nodoDimensioCollage *siguiente;
	string nomFilt;
	int pixHTML;
	int wcss;
	int hcss;
	int pwcss;
	int phcss;
	nodoDimensioCollage(string,int,int,int,int,int);

};

nodoDimensioCollage::nodoDimensioCollage(string a, int b, int c, int d, int e, int f)
{
	this->siguiente = NULL;
	this->nomFilt = a;
	this->pixHTML = b;
	this->wcss = c;
	this->hcss = d;
	this->pwcss = e;
	this->phcss = f;
}

class DimensionCollage{
public:
	nodoDimensioCollage *raiz;
	DimensionCollage();
	void insertar(string,int,int,int,int,int);
	void vaciar();
	int extraerPixHTML(string);
	int extraerwcss(string);
	int extraerhcss(string);
	int extraerpwcss(string);
	int extraerphcss(string);

};

DimensionCollage::DimensionCollage(){
	this->raiz = new nodoDimensioCollage("RAIZ",-1,-1,-1,-1,-1);
}

void DimensionCollage::insertar(string a, int b, int c, int d, int e, int f)
{
	nodoDimensioCollage *nuevo = new nodoDimensioCollage(a, b, c, d, e, f);

	if (this->raiz->siguiente==NULL) //si no hay nodos
	{
		this->raiz->siguiente = nuevo;
	}
	else{
		nuevo->siguiente = this->raiz->siguiente;
		this->raiz->siguiente = nuevo;
	}
}

void DimensionCollage::vaciar()
{
	this->raiz->siguiente = NULL;
}

int DimensionCollage::extraerPixHTML(string a)
{
	int ret = 0;

	if (this->raiz->siguiente!=NULL)
	{
		nodoDimensioCollage *temp = this->raiz;

		while (temp!=NULL)
		{
			if (temp->nomFilt.compare(a)==0) //cuando lo encuentre
			{
				ret = temp->pixHTML;
			}
			temp = temp->siguiente;
		}
	}
	return ret;
}

int DimensionCollage::extraerwcss(string a)
{
	int ret = 0;

	if (this->raiz->siguiente != NULL)
	{
		nodoDimensioCollage *temp = this->raiz;

		while (temp != NULL)
		{
			if (temp->nomFilt.compare(a) == 0) //cuando lo encuentre
			{
				ret = temp->wcss;
			}
			temp = temp->siguiente;
		}
	}
	return ret;
}

int DimensionCollage::extraerhcss(string a)
{
	int ret = 0;

	if (this->raiz->siguiente != NULL)
	{
		nodoDimensioCollage *temp = this->raiz;

		while (temp != NULL)
		{
			if (temp->nomFilt.compare(a) == 0) //cuando lo encuentre
			{
				ret = temp->hcss;
			}
			temp = temp->siguiente;
		}
	}
	return ret;
}

int DimensionCollage::extraerpwcss(string a)
{
	int ret = 0;

	if (this->raiz->siguiente != NULL)
	{
		nodoDimensioCollage *temp = this->raiz;

		while (temp != NULL)
		{
			if (temp->nomFilt.compare(a) == 0) //cuando lo encuentre
			{
				ret = temp->pwcss;
			}
			temp = temp->siguiente;
		}
	}
	return ret;
}

int DimensionCollage::extraerphcss(string a)
{
	int ret = 0;

	if (this->raiz->siguiente != NULL)
	{
		nodoDimensioCollage *temp = this->raiz;

		while (temp != NULL)
		{
			if (temp->nomFilt.compare(a) == 0) //cuando lo encuentre
			{
				ret = temp->phcss;
			}
			temp = temp->siguiente;
		}
	}
	return ret;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------------------------> GLOBALES <----------------------------------------

listaArchivos larch; //(cubo original)
copiaCubo copCubo; // (copia cubo)
Pila miPila; // (ayuda para filtros en ejes)
listaDobCircuFiltros listFiltros;
listaLinealizacion listLinealizacion;
nodoArbol *arbolImg;
imageOriginal listImageOriginal; //obsoleto
DimensionCollage listDimensionCollage;

//////////////////////////////////////////////////////////////////// OPERACIONES EN ARBOL /////////////////////////////////////////////////////////////////

void insertarEnArbol(nodoArbol *&arbolImg, string nomimg, int iw, int ih, int pw, int ph)
{	
	if (arbolImg==NULL)
	{
		nodoArbol *nuevo = new nodoArbol(nomimg, iw, ih, pw, ph);
		arbolImg = nuevo;
	}
	else{
		string nombre = arbolImg->nomImg;
		if ((nomimg<arbolImg->nomImg) == 1)
		{
			insertarEnArbol(arbolImg->izquierdo,nomimg,iw,ih,pw,ph);
		}
		else{
			insertarEnArbol(arbolImg->derecho, nomimg, iw, ih, pw, ph);
		}
	}
}

void mostrarArbol(nodoArbol *&arbolImg, int contador)
{
	if (arbolImg==NULL)
	{
		return;
	}
	else{
		mostrarArbol(arbolImg->derecho, contador + 1);
		for (int i = 0; i < contador; i++)
		{
			cout << "   ";
		}
		cout << arbolImg->nomImg << endl;
		mostrarArbol(arbolImg->izquierdo, contador + 1);
	}
}

void mostrarArbol2(nodoArbol *&arbolImg)
{
	if (arbolImg == NULL)
	{
		return;
	}
	else{
		mostrarArbol2(arbolImg->derecho);
		
		cout << "\n"  << arbolImg->nomImg << "\n" << endl;

		NodolistaArchivos *aux = arbolImg->cuboOriginal;
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

		mostrarArbol2(arbolImg->izquierdo);
	}
}

bool buscarEnArbol(nodoArbol *&arbolImg, string nomIMG)
{
	if (arbolImg == NULL)
	{
		return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG))==0){
		return true;
	}
	else if ((nomIMG<arbolImg->nomImg)==1){
		return buscarEnArbol(arbolImg->izquierdo,nomIMG);
	}
	else{
		return buscarEnArbol(arbolImg->derecho, nomIMG);
	}

}

bool buscarCapaEnArbol(nodoArbol *&arbolImg, string nomIMG, int cap)
{
	if (arbolImg == NULL)
	{
		return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
		bool bandee = false;
		NodolistaArchivos *aux = arbolImg->cuboOriginal;
		if (aux != NULL)
		{
			while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
			{
				if (aux->indice==cap)
				{
					bandee= true;
				}
				aux = aux->siguiente;
			}
		}
		return bandee;
	}
	else if ((nomIMG<arbolImg->nomImg) == 1){
		return buscarEnArbol(arbolImg->izquierdo, nomIMG);
	}
	else{
		return buscarEnArbol(arbolImg->derecho, nomIMG);
	}
}

void seleccionarImagen(nodoArbol *&arbolImg, string nomIMG)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
		larch.raizArch->siguiente = arbolImg->cuboOriginal;
		config[0] = arbolImg->imgWidth;
		config[1] = arbolImg->imgHeight;
		config[2] = arbolImg->pixWidth;
		config[3] = arbolImg->pixHeight;

	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		seleccionarImagen(arbolImg->izquierdo, nomIMG);
	}
	else{
		seleccionarImagen(arbolImg->derecho, nomIMG);
	}
}

void enlazarImagenAlInsertar(nodoArbol *&arbolImg, string nomIMG)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
		arbolImg->cuboOriginal = larch.raizArch->siguiente; //enlazamos o guardamos el cubo en el nodo del arbol
	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		enlazarImagenAlInsertar(arbolImg->izquierdo, nomIMG);
	}
	else{
		enlazarImagenAlInsertar(arbolImg->derecho, nomIMG);
	}
}

int contadorMostMenu = 0;
void InordenParaMostrarEnMenu(nodoArbol *arbolImg)
{
	string cadena = "";
	
	if (arbolImg==NULL)
	{
		//printf("\nAUN NO HAY IMAGENES EN EL SISTEMA!\n");
	}
	else{
		InordenParaMostrarEnMenu(arbolImg->izquierdo);
		cout << to_string(contadorMostMenu+=1) << ". " << arbolImg->nomImg << endl;
		InordenParaMostrarEnMenu(arbolImg->derecho);
	}

}

void guardarImageOriginal(string nomImagee)
{
	if (listImageOriginal.primero != NULL)
	{
		nodoImageOriginal *actual = listImageOriginal.primero;

		do
		{
			if (actual->nomImage == nomImagee) //cuando encuentre el nombre del filtro
			{
				if (actual->apuntaCuboOriginal == NULL) // si aun no hay cubo en ese nodo
				{
					NodolistaArchivos *nuevo = larch.raizArch->siguiente;
					actual->apuntaCuboOriginal = nuevo; //se enlaza o guarda en el nodo el cubo
					break;
				}
				else{
					printf("\n--> YA APLICO ESTE FILTRO A LA IMAGEN!\n");
				}

			}
			actual = actual->siguiente;

		} while (actual != listImageOriginal.primero);
	}
} 

void reporteCapaEspecificaDeCuboDeArbol(nodoArbol *&arbolImg, string nomIMG, int cap)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
			
		NodolistaArchivos *aux = arbolImg->cuboOriginal;
		string grafo = "";

		if (aux != NULL)
		{
			while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
			{
				if (aux->indice==cap)
				{
					grafo = "";
					string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
					string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv			

					int filas = copCubo.obtenerCoordenadaYmayorDeCapa(nomCap) + 1; //numero de filas, +1 por la cabecera

					//para cabecera Y
					nodoMatriz *auxCabY = aux->apuntaRaizDeMatriz;
					nodoMatriz *auxCabY2 = aux->apuntaRaizDeMatriz;

					while (auxCabY != NULL)
					{
						int op = filas - auxCabY->y;
						if (auxCabY->y == -1)
						{
							op = filas + 1;
						}

						if (auxCabY->x == -1 && auxCabY->y == -1)
						{
							grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data> RAIZ }\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
						}
						else{
							grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data>" + auxCabY->dato + " (" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")}" + "\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
						}
						auxCabY = auxCabY->abajo;
					}

					//relacion entre cabecera Y
					while (auxCabY2 != NULL)
					{
						if (auxCabY2->abajo != NULL)
						{
							grafo += "\"(" + to_string(auxCabY2->x) + "," + to_string(auxCabY2->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabY2->abajo->x) + "," + to_string(auxCabY2->abajo->y) + ")" + "\" [dir=both]; \n";
						}
						auxCabY2 = auxCabY2->abajo;
					}

					//para cabecera X
					nodoMatriz *auxCabX = aux->apuntaRaizDeMatriz->siguiente;
					nodoMatriz *auxCabX2 = aux->apuntaRaizDeMatriz->siguiente;

					while (auxCabX != NULL)
					{
						int op = filas - auxCabX->y;
						if (auxCabX->y == -1)
						{
							op = filas + 1;
						}
						grafo += "\"(" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "\"" + "[label=\"{<data>" + auxCabX->dato + " (" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "}\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabX->x * 3) + "," + to_string(op) + "!\"];\n";
						auxCabX = auxCabX->siguiente;
					}

					//relacion entre cabecera X
					while (auxCabX2 != NULL)
					{
						grafo += "\"(" + to_string(auxCabX2->anterior->x) + "," + to_string(auxCabX2->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabX2->x) + "," + to_string(auxCabX2->y) + ")" + "\" [dir=both]; \n";
						auxCabX2 = auxCabX2->siguiente;
					}

					//Para nodos
					nodoMatriz *auxNod = aux->apuntaRaizDeMatriz->abajo;

					while (auxNod != NULL)
					{
						//creando nodos
						nodoMatriz *aux2 = auxNod->siguiente;
						nodoMatriz *aux3 = auxNod->siguiente;

						while (aux2 != NULL)
						{
							int op = filas - aux2->y;
							grafo += "\"(" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "\"" + "[label=\"{<data>" + aux2->dato + " (" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "}\" width = 1.5 pos=\"" + to_string(aux2->x * 3) + "," + to_string(op) + "!\"];\n";
							aux2 = aux2->siguiente;
						}

						//haciendo relacion entre nodos
						while (aux3 != NULL)
						{
							grafo += "\"(" + to_string(aux3->anterior->x) + "," + to_string(aux3->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\" [dir=both]; \n";
							grafo += "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->arriba->x) + "," + to_string(aux3->arriba->y) + ")" + "\" [dir=both]; \n";

							aux3 = aux3->siguiente;
						}

						auxNod = auxNod->abajo;
					}
					string nomGuardar = "REPORT_CAPA_ESPECIFICA_" + nomIMG + "_CAPA_NUMERO_" + to_string(cap);
					graphvizEscrituraParaCapaDesdeArbol(nomGuardar, grafo, nomIMG);
				}				

				aux = aux->siguiente;
			}

		}

	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		reporteCapaEspecificaDeCuboDeArbol(arbolImg->izquierdo, nomIMG,cap);
	}
	else{
		reporteCapaEspecificaDeCuboDeArbol(arbolImg->derecho, nomIMG,cap);
	}
}

void reporteTodasLasCapasDeCuboDeArbol(nodoArbol *&arbolImg, string nomIMG)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){

		NodolistaArchivos *aux = arbolImg->cuboOriginal;
		string grafo = "";

		if (aux != NULL)
		{
			while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
			{
				grafo = "";
				string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
				string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv			

				int filas = copCubo.obtenerCoordenadaYmayorDeCapa(nomCap) + 1; //numero de filas, +1 por la cabecera

				//para cabecera Y
				nodoMatriz *auxCabY = aux->apuntaRaizDeMatriz;
				nodoMatriz *auxCabY2 = aux->apuntaRaizDeMatriz;

				while (auxCabY != NULL)
				{
					int op = filas - auxCabY->y;
					if (auxCabY->y == -1)
					{
						op = filas + 1;
					}

					if (auxCabY->x == -1 && auxCabY->y == -1)
					{
						grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data> RAIZ }\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
					}
					else{
						grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data>" + auxCabY->dato + " (" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")}" + "\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
					}
					auxCabY = auxCabY->abajo;
				}

				//relacion entre cabecera Y
				while (auxCabY2 != NULL)
				{
					if (auxCabY2->abajo != NULL)
					{
						grafo += "\"(" + to_string(auxCabY2->x) + "," + to_string(auxCabY2->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabY2->abajo->x) + "," + to_string(auxCabY2->abajo->y) + ")" + "\" [dir=both]; \n";
					}
					auxCabY2 = auxCabY2->abajo;
				}

				//para cabecera X
				nodoMatriz *auxCabX = aux->apuntaRaizDeMatriz->siguiente;
				nodoMatriz *auxCabX2 = aux->apuntaRaizDeMatriz->siguiente;

				while (auxCabX != NULL)
				{
					int op = filas - auxCabX->y;
					if (auxCabX->y == -1)
					{
						op = filas + 1;
					}
					grafo += "\"(" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "\"" + "[label=\"{<data>" + auxCabX->dato + " (" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "}\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabX->x * 3) + "," + to_string(op) + "!\"];\n";
					auxCabX = auxCabX->siguiente;
				}

				//relacion entre cabecera X
				while (auxCabX2 != NULL)
				{
					grafo += "\"(" + to_string(auxCabX2->anterior->x) + "," + to_string(auxCabX2->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabX2->x) + "," + to_string(auxCabX2->y) + ")" + "\" [dir=both]; \n";
					auxCabX2 = auxCabX2->siguiente;
				}

				//Para nodos
				nodoMatriz *auxNod = aux->apuntaRaizDeMatriz->abajo;

				while (auxNod != NULL)
				{
					//creando nodos
					nodoMatriz *aux2 = auxNod->siguiente;
					nodoMatriz *aux3 = auxNod->siguiente;

					while (aux2 != NULL)
					{
						int op = filas - aux2->y;
						grafo += "\"(" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "\"" + "[label=\"{<data>" + aux2->dato + " (" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "}\" width = 1.5 pos=\"" + to_string(aux2->x * 3) + "," + to_string(op) + "!\"];\n";
						aux2 = aux2->siguiente;
					}

					//haciendo relacion entre nodos
					while (aux3 != NULL)
					{
						grafo += "\"(" + to_string(aux3->anterior->x) + "," + to_string(aux3->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\" [dir=both]; \n";
						grafo += "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->arriba->x) + "," + to_string(aux3->arriba->y) + ")" + "\" [dir=both]; \n";

						aux3 = aux3->siguiente;
					}

					auxNod = auxNod->abajo;
				}
				string nomGuardar = "REPORT_CAPAS_" + nomIMG + "_CAPA_" + nomCap;
				graphvizEscrituraParaCapaDesdeArbol(nomGuardar, grafo,nomIMG);

				aux = aux->siguiente;
			}
		}
	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		reporteTodasLasCapasDeCuboDeArbol(arbolImg->izquierdo, nomIMG);
	}
	else{
		reporteTodasLasCapasDeCuboDeArbol(arbolImg->derecho, nomIMG);
	}
}

void reporteLinealizacionPorColumnaDeCuboDeArbol(nodoArbol *&arbolImg, string nomIMG, int cap)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
		NodolistaArchivos *aux = arbolImg->cuboOriginal;
		string cadena = "";

		if (aux != NULL)
		{
			while (aux != NULL) //me recorre la lista en este sentido -> -> ->
			{
				if (aux->indice==cap)
				{
					cadena = "";
					string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
					string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv

					nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente; //(-1,0)

					while (aux2 != NULL) //me recorre la matriz para abajo
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
					string nomGuardar = "REPORT_LINEALIZACION_POR_COLUMNA_" + nomIMG + "_CAPA_NUMERO_" + to_string(cap);
					graphvizEscrituraParaLinealizacionDesdeArbol(nomGuardar, cad, nomIMG);
					listLinealizacion.vaciar();
				}
				aux = aux->siguiente;
			}
		}
	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		reporteLinealizacionPorColumnaDeCuboDeArbol(arbolImg->izquierdo, nomIMG, cap);
	}
	else{
		reporteLinealizacionPorColumnaDeCuboDeArbol(arbolImg->derecho, nomIMG, cap);
	}
}

void reporteLinealizacionPorFilaDeCuboDeArbol(nodoArbol *&arbolImg, string nomIMG, int cap)
{
	if (arbolImg == NULL)
	{
		//return false;
	}
	else if ((arbolImg->nomImg.compare(nomIMG)) == 0){
		
		NodolistaArchivos *aux = arbolImg->cuboOriginal;
		string cadena = "";

		if (aux != NULL)
		{
			while (aux != NULL) //me recorre la lista en este sentido -> -> ->
			{
				if (aux->indice==cap)
				{
					cadena = "";
					string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
					string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv

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
					string nomGuardar = "REPORT_LINEALIZACION_POR_FILA_" + nomIMG + "_CAPA_NUMERO_" + to_string(cap);
					graphvizEscrituraParaLinealizacionDesdeArbol(nomGuardar, cad, nomIMG);
					listLinealizacion.vaciar();
				}
				aux = aux->siguiente;
			}
		}

	}
	else if (((nomIMG)<(arbolImg->nomImg)) == 1){
		reporteLinealizacionPorFilaDeCuboDeArbol(arbolImg->izquierdo, nomIMG, cap);
	}
	else{
		reporteLinealizacionPorFilaDeCuboDeArbol(arbolImg->derecho, nomIMG, cap);
	}
}

void reporteInorder(nodoArbol *arbolImg)
{
	string cadena = "";

	if (arbolImg == NULL)
	{
		//printf("\nAUN NO HAY IMAGENES EN EL SISTEMA!\n");
	}
	else{
		reporteInorder(arbolImg->izquierdo);
		string cadena = "\""+ arbolImg->nomImg + ", W: " + to_string(arbolImg->imgWidth) + ", H: " + to_string(arbolImg->imgHeight) + ", PW: " + to_string(arbolImg->pixWidth) + "px, PH: " + to_string(arbolImg->pixHeight) + "px\"" ;
		listLinealizacion.insertar(cadena);
		reporteInorder(arbolImg->derecho);
	}
}

void reportePreorder(nodoArbol *arbolImg)
{
	string cadena = "";

	if (arbolImg == NULL)
	{
		//printf("\nAUN NO HAY IMAGENES EN EL SISTEMA!\n");
	}
	else{
		string cadena = "\"" + arbolImg->nomImg + ", W: " + to_string(arbolImg->imgWidth) + ", H: " + to_string(arbolImg->imgHeight) + ", PW: " + to_string(arbolImg->pixWidth) + "px, PH: " + to_string(arbolImg->pixHeight) + "px\"";
		listLinealizacion.insertar(cadena);
		reportePreorder(arbolImg->izquierdo);		
		reportePreorder(arbolImg->derecho);
	}
}

void reportePosorder(nodoArbol *arbolImg)
{
	string cadena = "";

	if (arbolImg == NULL)
	{
		//printf("\nAUN NO HAY IMAGENES EN EL SISTEMA!\n");
	}
	else{		
		reportePosorder(arbolImg->izquierdo);
		reportePosorder(arbolImg->derecho);
		string cadena = "\"" + arbolImg->nomImg + ", W: " + to_string(arbolImg->imgWidth) + ", H: " + to_string(arbolImg->imgHeight) + ", PW: " + to_string(arbolImg->pixWidth) + "px, PH: " + to_string(arbolImg->pixHeight) + "px\"";
		listLinealizacion.insertar(cadena);
	}
}

string cadrep = "";
void reporteArbol(nodoArbol *arbolImg)
{
	if (arbolImg == NULL)
	{
		return;
	}
	else{
		cout << arbolImg->nomImg << endl;
		if (arbolImg->izquierdo != NULL)
		{
			cadrep += "\"" + arbolImg->nomImg + " \n W: " + to_string(arbolImg->imgWidth) + ", H: " + to_string(arbolImg->imgHeight) + " \n PW: " + to_string(arbolImg->pixWidth) + "px, PH: " + to_string(arbolImg->pixHeight) + "px\"" + "->" + "\"" + arbolImg->izquierdo->nomImg + " \n W: " + to_string(arbolImg->izquierdo->imgWidth) + ", H: " + to_string(arbolImg->izquierdo->imgHeight) + " \n PW: " + to_string(arbolImg->izquierdo->pixWidth) + "px, PH: " + to_string(arbolImg->izquierdo->pixHeight) + "px\"" + "\n";
		}
		if (arbolImg->derecho!=NULL)
		{
			cadrep += "\"" + arbolImg->nomImg + " \n W: " + to_string(arbolImg->imgWidth) + ", H: " + to_string(arbolImg->imgHeight) + " \n PW: " + to_string(arbolImg->pixWidth) + "px, PH: " + to_string(arbolImg->pixHeight) + "px\"" + "->" + "\"" + arbolImg->derecho->nomImg + " \n W: " + to_string(arbolImg->derecho->imgWidth) + ", H: " + to_string(arbolImg->derecho->imgHeight) + " \n PW: " + to_string(arbolImg->derecho->pixWidth) + "px, PH: " + to_string(arbolImg->derecho->pixHeight) + "px\"" + "\n";
		}		
		reporteArbol(arbolImg->derecho);
		reporteArbol(arbolImg->izquierdo);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//se crea el cubo, va creando nodo en la la lista archivos y luego creando matriz para cada nodo
void CrearCuboDisperso(string _archivoleer)
{	
	larch.raizArch->siguiente = NULL; //limpiamos nuestro cubo original

	int bande = 1; // bandera para que me ayude a insetar
	int capa = 0;
	string archivo="";	
	
	//me lee el archivo principal
	try{
		ifstream lectura;
		lectura.open(_archivoleer, ios::in);
		int linealec = 0; //me dira en que linea me encuentro

		for (string linea; getline(lectura, linea);)
		{
			stringstream registro(linea);
			string dato;

			for (int columna = 0; getline(registro, dato, ','); ++columna)
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
		
		if (larch.raizArch->siguiente!=NULL)
		{
			//inserto y enlazo en arbol
			insertarEnArbol(arbolImg, nombreDeImagen, config[0], config[1], config[2], config[3]);
			enlazarImagenAlInsertar(arbolImg, nombreDeImagen);
			//listImageOriginal.insertar(nombreDeImagen);
			//guardarImageOriginal(nombreDeImagen);
		}
		
		
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

void collage(int xRep, int yRep)
{
	NodolistaArchivos *aux = larch.raizArch->siguiente;//

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			//capturo el indice y nombre de archivo
			int indi = aux->indice;
			string nom = aux->nomArchivo;
			copCubo.insertarElemento(indi, nom); //lo agrego a la lista copiarcubo

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
						while (aux2 != NULL) //me recorre la matriz para abajo, aux2 empieza en (-1,0)
						{
							nodoMatriz *aux3 = aux2->siguiente; //se posiciona en el siguiente de la cabecera Y

							while (aux3!=NULL)
							{
								nm.insertarElemento(aux3->x, aux3->y, aux3->dato); //me inserta el valor de la matriz actual

								int posX = aux3->x;
								int posY = aux3->y;
								int tamimageParaX = config[0];
								int tamimageParaY = config[1];

								//Si es mayor que uno modificara (repeticiones en x)
								if (xRep>1)
								{
									// se crea 
									for (int i = 0; i < xRep-1; i++)
									{
										posX = posX + tamimageParaX;
										nm.insertarElemento(posX,aux3->y,aux3->dato);
									}
								}
								//Si es mayor que uno modificara (repeticiones en y)
								if (yRep>1)
								{
									for (int i = 0; i < yRep-1; i++)
									{
										posY = posY + tamimageParaY;
										nm.insertarElemento(aux3->x,posY,aux3->dato);										
										
										// Por cada repeticion en y, se crean las repeticiones en x en esa fila y
										if (xRep>1)
										{
											int posXx = aux3->x;
											for (int k = 0; k < xRep - 1; k++)
											{
												posXx = posXx + tamimageParaX;
												nm.insertarElemento(posXx, posY, aux3->dato);
											}
										}

									}
								}

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

void mosaico()
{
	NodolistaArchivos *aux = larch.raizArch->siguiente;//

	int xRep = config[0]; // image width
	int yRep = config[1]; // image height

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			//capturo el indice y nombre de archivo
			int indi = aux->indice;
			string nom = aux->nomArchivo;
			copCubo.insertarElemento(indi, nom); //lo agrego a la lista copiarcubo

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
						while (aux2 != NULL) //me recorre la matriz para abajo, aux2 empieza en (-1,0)
						{
							nodoMatriz *aux3 = aux2->siguiente; //se posiciona en el siguiente de la cabecera Y

							while (aux3 != NULL)
							{
								nm.insertarElemento(aux3->x, aux3->y, aux3->dato); //me inserta el valor de la matriz actual

								int posX = aux3->x;
								int posY = aux3->y;
								int tamimageParaX = config[0];
								int tamimageParaY = config[0];

								//Si es mayor que uno modificara (repeticiones en x)
								if (xRep>1)
								{
									// se crea 
									for (int i = 0; i < xRep - 1; i++)
									{
										posX = posX + tamimageParaX;
										nm.insertarElemento(posX, aux3->y, aux3->dato);
									}
								}
								//Si es mayor que uno modificara (repeticiones en y)
								if (yRep>1)
								{
									for (int i = 0; i < yRep - 1; i++)
									{
										posY = posY + tamimageParaY;
										nm.insertarElemento(aux3->x, posY, aux3->dato);

										// Por cada repeticion en y, se crean las repeticiones en x en esa fila y
										if (xRep>1)
										{
											int posXx = aux3->x;
											for (int k = 0; k < xRep - 1; k++)
											{
												posXx = posXx + tamimageParaX;
												nm.insertarElemento(posXx, posY, aux3->dato);
											}
										}

									}
								}

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

void modificarCapaEspecificaDeCuboGrayScale(string nomCapa)
{
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	string filas = "";

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string a = aux->nomArchivo;
			string b = a.substr(0, a.length() - 4);

			if (b == nomCapa)
			{
				nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en (-1,0)

				while (aux2 != NULL) //me recorre la matriz para abajo
				{
					nodoMatriz *temp = aux2->siguiente;//pos (0,0)

					while (temp != NULL)
					{
						string a = obtenerRGBgrayscale(temp->dato);
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

void modificarTodasLasCapasDeCuboGrayScale()
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

				while (temp != NULL)
				{
					string a = obtenerRGBgrayscale(temp->dato);
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

string obtenerRGBgrayscale(string _rgb)
{
	string d = _rgb;
	int pos = d.find("-"); //primer guion
	string e = d.substr(0, pos); //obtiene rgbR
	string f = d.substr(pos + 1, d.length() - 1);//resto de cadena
	int pos2 = f.find("-");
	string g = f.substr(0, pos2);//obtiene rgbG
	string h = f.substr(pos2 + 1, f.length() - 1);//obtiene rgbB

	//obtengo numeros por separado
	double rgb1 = atof(e.c_str());
	double rgb2 = atof(g.c_str());
	double rgb3 = atof(h.c_str());

	double op1 = 0.3 * rgb1;
	double op2 = 0.59 * rgb2;
	double op3 = 0.11 * rgb3;
	double op4 = op1 + op2 + op3;
	int op5 = (int)op4;

	string dev = to_string(op5) + "-" + to_string(op5) + "-" + to_string(op5);

	return dev;
}

void filtroEspejoEjeX()
{
	//FORMULA: (TAMIMAGEN - COLUMNAACTUAL) -1

	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	miPila.raizPila->siguiente = NULL; //limpio Pila

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string a = aux->nomArchivo;
			string b = a.substr(0, a.length() - 4);
			
			Matriz nm;

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en (-1,0)
			NodoCopiaCubo *auxbuscar = copCubo.raizCopCubo->siguiente;

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				nodoMatriz *aux3 = aux2->siguiente; //(primer recorrido) para que empieze en columna 0 hacia -->

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					int tamimagen = config[0]; 
					int columActual = aux3->x;
					int op1 = tamimagen - columActual;
					int op2 = op1 - 1;
					nm.insertarElemento(op2,aux3->y,aux3->dato);
					aux3 = aux3->siguiente;
				}				

				aux2 = aux2->abajo;
			}

			if (auxbuscar != NULL) //BUSCAR Y MODIFICAR CAPA
			{
				while (auxbuscar != NULL) //me recorre la listaArchivos en este sentido -> -> ->
				{
					string c = auxbuscar->nomArchivo;
					string d = c.substr(0, c.length() - 4);
					if (b == d) // cuando encuentra la misma capa...
					{
						auxbuscar->apuntaRaizDeMatriz = nm.raiz; //se enlaza la nueva capa modificada donde estaba la otra
					}

					auxbuscar = auxbuscar->siguiente;
				}
			}
			aux = aux->siguiente;
		}

	}
}

void filtroEspejoEjeY()
{
	//FORMULA: (TAMIMAGEN - FILAACTUAL) -1
	
	NodoCopiaCubo *aux = copCubo.raizCopCubo->siguiente;
	miPila.raizPila->siguiente = NULL; //limpio Pila

	if (aux != NULL)
	{
		while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
		{
			string a = aux->nomArchivo;
			string b = a.substr(0, a.length() - 4);

			Matriz nm;

			nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->abajo; //se posiciona en (-1,0)
			NodoCopiaCubo *auxbuscar = copCubo.raizCopCubo->siguiente;

			while (aux2 != NULL) //me recorre la matriz para abajo
			{
				nodoMatriz *aux3 = aux2->siguiente; //(primer recorrido) para que empieze en columna 0 hacia -->

				while (aux3 != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
				{
					int tamimagen = config[0];
					int filaActual = aux3->y;
					int op1 = tamimagen - filaActual;
					int op2 = op1 - 1;
					nm.insertarElemento(aux3->x, op2, aux3->dato);
					aux3 = aux3->siguiente;
				}
				aux2 = aux2->abajo;
			}

			if (auxbuscar != NULL) //BUSCAR Y MODIFICAR CAPA
			{
				while (auxbuscar != NULL) //me recorre la listaArchivos en este sentido -> -> ->
				{
					string c = auxbuscar->nomArchivo;
					string d = c.substr(0, c.length() - 4);
					if (b == d) // cuando encuentra la misma capa...
					{
						auxbuscar->apuntaRaizDeMatriz = nm.raiz; //se enlaza la nueva capa modificada donde estaba la otra
					}

					auxbuscar = auxbuscar->siguiente;
				}
			}
			aux = aux->siguiente;
		}

	}
}

void ReporteGraphvizTodasLasCapasListaFiltros(string nomF)
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;
		do
		{
			if (actual->nomFiltro.compare(nomF)==0)
			{
				if (actual->apuntaCopiaCubo != NULL)
				{
					NodoCopiaCubo *aux = actual->apuntaCopiaCubo->siguiente;
					string grafo = "";

					if (aux != NULL)
					{
						while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
						{
							grafo = "";
							string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
							string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv			

							int filas = copCubo.obtenerCoordenadaYmayorDeCapa(nomCap) + 1; //numero de filas, +1 por la cabecera

							//para cabecera Y
							nodoMatriz *auxCabY = aux->apuntaRaizDeMatriz;
							nodoMatriz *auxCabY2 = aux->apuntaRaizDeMatriz;

							while (auxCabY != NULL)
							{
								int op = filas - auxCabY->y;
								if (auxCabY->y == -1)
								{
									op = filas + 1;
								}

								if (auxCabY->x == -1 && auxCabY->y == -1)
								{
									grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data> RAIZ }\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
								}
								else{
									grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data>" + auxCabY->dato + " (" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")}" + "\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
								}
								auxCabY = auxCabY->abajo;
							}

							//relacion entre cabecera Y
							while (auxCabY2 != NULL)
							{
								if (auxCabY2->abajo != NULL)
								{
									grafo += "\"(" + to_string(auxCabY2->x) + "," + to_string(auxCabY2->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabY2->abajo->x) + "," + to_string(auxCabY2->abajo->y) + ")" + "\" [dir=both]; \n";
								}
								auxCabY2 = auxCabY2->abajo;
							}

							//para cabecera X
							nodoMatriz *auxCabX = aux->apuntaRaizDeMatriz->siguiente;
							nodoMatriz *auxCabX2 = aux->apuntaRaizDeMatriz->siguiente;

							while (auxCabX != NULL)
							{
								int op = filas - auxCabX->y;
								if (auxCabX->y == -1)
								{
									op = filas + 1;
								}
								grafo += "\"(" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "\"" + "[label=\"{<data>" + auxCabX->dato + " (" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "}\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabX->x * 3) + "," + to_string(op) + "!\"];\n";
								auxCabX = auxCabX->siguiente;
							}

							//relacion entre cabecera X
							while (auxCabX2 != NULL)
							{
								grafo += "\"(" + to_string(auxCabX2->anterior->x) + "," + to_string(auxCabX2->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabX2->x) + "," + to_string(auxCabX2->y) + ")" + "\" [dir=both]; \n";
								auxCabX2 = auxCabX2->siguiente;
							}

							//Para nodos
							nodoMatriz *auxNod = aux->apuntaRaizDeMatriz->abajo;

							while (auxNod != NULL)
							{
								//creando nodos
								nodoMatriz *aux2 = auxNod->siguiente;
								nodoMatriz *aux3 = auxNod->siguiente;

								while (aux2 != NULL)
								{
									int op = filas - aux2->y;
									grafo += "\"(" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "\"" + "[label=\"{<data>" + aux2->dato + " (" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "}\" width = 1.5 pos=\"" + to_string(aux2->x * 3) + "," + to_string(op) + "!\"];\n";
									aux2 = aux2->siguiente;
								}

								//haciendo relacion entre nodos
								while (aux3 != NULL)
								{
									grafo += "\"(" + to_string(aux3->anterior->x) + "," + to_string(aux3->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\" [dir=both]; \n";
									grafo += "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->arriba->x) + "," + to_string(aux3->arriba->y) + ")" + "\" [dir=both]; \n";

									aux3 = aux3->siguiente;
								}

								auxNod = auxNod->abajo;
							}
							string nomGuardar = "REPORT_CAPAS_" + nombreDeImagen + "_FILTRO_" + nomF + "_CAPA_" + nomCap;
							graphvizEscrituraParaCapa(nomGuardar, grafo);
							aux = aux->siguiente;
						}

					}
				}
			}
			
			actual = actual->siguiente;
		} while (actual != listFiltros.primero);
	}
		
}

void ReporteGraphvizCapaEspecificaListaFiltros(string nomF, int cap)
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;
		do
		{
			if (actual->nomFiltro.compare(nomF) == 0)
			{
				if (actual->apuntaCopiaCubo != NULL)
				{
					NodoCopiaCubo *aux = actual->apuntaCopiaCubo->siguiente;
					string grafo = "";

					if (aux != NULL)
					{
						while (aux != NULL) //me recorre la listaArchivos en este sentido -> -> ->
						{
							if (aux->indice==cap)
							{
								grafo = "";
								string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
								string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv			

								int filas = copCubo.obtenerCoordenadaYmayorDeCapa(nomCap) + 1; //numero de filas, +1 por la cabecera

								//para cabecera Y
								nodoMatriz *auxCabY = aux->apuntaRaizDeMatriz;
								nodoMatriz *auxCabY2 = aux->apuntaRaizDeMatriz;

								while (auxCabY != NULL)
								{
									int op = filas - auxCabY->y;
									if (auxCabY->y == -1)
									{
										op = filas + 1;
									}

									if (auxCabY->x == -1 && auxCabY->y == -1)
									{
										grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data> RAIZ }\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
									}
									else{
										grafo += "\"(" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")" + "\"" + "[label=\"{<data>" + auxCabY->dato + " (" + to_string(auxCabY->x) + "," + to_string(auxCabY->y) + ")}" + "\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabY->x) + "," + to_string(op) + "!\"];\n";
									}
									auxCabY = auxCabY->abajo;
								}

								//relacion entre cabecera Y
								while (auxCabY2 != NULL)
								{
									if (auxCabY2->abajo != NULL)
									{
										grafo += "\"(" + to_string(auxCabY2->x) + "," + to_string(auxCabY2->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabY2->abajo->x) + "," + to_string(auxCabY2->abajo->y) + ")" + "\" [dir=both]; \n";
									}
									auxCabY2 = auxCabY2->abajo;
								}

								//para cabecera X
								nodoMatriz *auxCabX = aux->apuntaRaizDeMatriz->siguiente;
								nodoMatriz *auxCabX2 = aux->apuntaRaizDeMatriz->siguiente;

								while (auxCabX != NULL)
								{
									int op = filas - auxCabX->y;
									if (auxCabX->y == -1)
									{
										op = filas + 1;
									}
									grafo += "\"(" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "\"" + "[label=\"{<data>" + auxCabX->dato + " (" + to_string(auxCabX->x) + "," + to_string(auxCabX->y) + ")" + "}\" ,  style = filled, fillcolor = firebrick1, width = 1.5 pos=\"" + to_string(auxCabX->x * 3) + "," + to_string(op) + "!\"];\n";
									auxCabX = auxCabX->siguiente;
								}

								//relacion entre cabecera X
								while (auxCabX2 != NULL)
								{
									grafo += "\"(" + to_string(auxCabX2->anterior->x) + "," + to_string(auxCabX2->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(auxCabX2->x) + "," + to_string(auxCabX2->y) + ")" + "\" [dir=both]; \n";
									auxCabX2 = auxCabX2->siguiente;
								}

								//Para nodos
								nodoMatriz *auxNod = aux->apuntaRaizDeMatriz->abajo;

								while (auxNod != NULL)
								{
									//creando nodos
									nodoMatriz *aux2 = auxNod->siguiente;
									nodoMatriz *aux3 = auxNod->siguiente;

									while (aux2 != NULL)
									{
										int op = filas - aux2->y;
										grafo += "\"(" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "\"" + "[label=\"{<data>" + aux2->dato + " (" + to_string(aux2->x) + "," + to_string(aux2->y) + ")" + "}\" width = 1.5 pos=\"" + to_string(aux2->x * 3) + "," + to_string(op) + "!\"];\n";
										aux2 = aux2->siguiente;
									}

									//haciendo relacion entre nodos
									while (aux3 != NULL)
									{
										grafo += "\"(" + to_string(aux3->anterior->x) + "," + to_string(aux3->anterior->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\" [dir=both]; \n";
										grafo += "\"(" + to_string(aux3->x) + "," + to_string(aux3->y) + ")" + "\"" + "->" + "\"(" + to_string(aux3->arriba->x) + "," + to_string(aux3->arriba->y) + ")" + "\" [dir=both]; \n";

										aux3 = aux3->siguiente;
									}

									auxNod = auxNod->abajo;
								}
								string nomGuardar = "REPORT_CAPAS_" + nombreDeImagen + "_FILTRO_" + nomF + "_CAPA_" + nomCap;
								graphvizEscrituraParaCapa(nomGuardar, grafo);
							}
							
							aux = aux->siguiente;
						}

					}
				}
			}

			actual = actual->siguiente;
		} while (actual != listFiltros.primero);
	}
}

void ReporteGraphvizLinealizacionPorFilaTodasCapas()
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;
		do
		{
			string nomFilt = actual->nomFiltro;

			if (actual->apuntaCopiaCubo != NULL)
			{
				NodoCopiaCubo *aux = actual->apuntaCopiaCubo->siguiente;
				string cadena = "";

				if (aux != NULL)
				{
					while (aux != NULL) //me recorre la lista en este sentido -> -> ->
					{
						cadena = "";
						string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
						string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv

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
						string nomGuardar = "REPORT_LINEALIZACION_POR_FILA_" + nombreDeImagen + "_FILTRO_" + nomFilt + "_CAPA_" + nomCap;
						graphvizEscrituraParaLinealizacion(nomGuardar, cad);
						listLinealizacion.vaciar();
						aux = aux->siguiente;
					}
				}
				
			}
			actual = actual->siguiente;
		} while (actual != listFiltros.primero);
	}

}

void ReporteGraphvizLinealizacionPorColumnaTodasCapas()
{

	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;
		do
		{
			string nomFilt = actual->nomFiltro;

			if (actual->apuntaCopiaCubo != NULL)
			{
				NodoCopiaCubo *aux = actual->apuntaCopiaCubo->siguiente;
				string cadena = "";

				if (aux != NULL)
				{
					while (aux != NULL) //me recorre la lista en este sentido -> -> ->
					{
						cadena = "";
						string nomCapExt = aux->nomArchivo; //captura el nombre de la capa
						string nomCap = nomCapExt.substr(0, nomCapExt.length() - 4); //le quito el .csv

						nodoMatriz *aux2 = aux->apuntaRaizDeMatriz->siguiente; //(-1,0)

						while (aux2 != NULL) //me recorre la matriz para abajo
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
						string nomGuardar = "REPORT_LINEALIZACION_POR_COLUMNA_" + nombreDeImagen + "_FILTRO_" + nomFilt + "_CAPA_" + nomCap;
						graphvizEscrituraParaLinealizacion(nomGuardar, cad);
						listLinealizacion.vaciar();
						aux = aux->siguiente;
					}
				}

			}
			actual = actual->siguiente;
		} while (actual != listFiltros.primero);
	}
}

void guardarConfig(string _archivo)
{
	int bande = 1;
	int linealec = 0; //me dira en que linea me encuentro
	int tam = 0;
	string conf = "";
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

			for (int columna = 0; getline(registro, dato, ','); ++columna)
			{
				if (linealec > 0)
				{
					if (bande % 2 == 0) //cada dos datos insertara, para mandar guardar tamanio de imagen
					{
						int a = linealec - 1; //guardara en el indice (linea que va menos 1)
						tam = atoi(dato.c_str()); //convierte el valor a entero
						
						if (strstr(conf.c_str(), "mage_width")) // no pongo i, por si en el archivo viene con mayuscula
						{
							config[0] = tam; //lo asigna al vector
						}
						else if (strstr(conf.c_str(), "mage_height")){
							config[1] = tam;
						}
						else if (strstr(conf.c_str(), "ixel_width")){
							config[2] = tam;
						}
						else if (strstr(conf.c_str(), "ixel_height")){
							config[3] = tam;
						}						
					}
					else{
						conf = dato;
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

void graphvizEscrituraParaCapa(string _nomGuardar, string _texto)
{
	//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
	string rut = "Exports/" + nombreDeImagen;
	char* rutt = (char *)rut.c_str();
	int a = _mkdir(rutt);

	ofstream archivo;
	archivo.open("Exports/" + nombreDeImagen + "/" + _nomGuardar + ".dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph g \n";
	archivo << "{\nnode[shape=record];\n";
	archivo << "graph[pencolor=transparent];\n";
	archivo << "rankdir=LR;\n";
	archivo << "node [style=filled];\n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	char* charr;
	string dott = "neato -Tjpg Exports/" + nombreDeImagen + "/" + _nomGuardar + ".dot -o Exports/" + nombreDeImagen + "/" + _nomGuardar + ".jpg";
	charr = (char *)dott.c_str();
	system(charr);
	string im = "start Exports/" + nombreDeImagen + "/" + _nomGuardar + ".jpg";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaCapaDesdeArbol(string _nomGuardar, string _texto, string nomImg)
{
	//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
	string rut = "Exports/" + nomImg;
	char* rutt = (char *)rut.c_str();
	int a = _mkdir(rutt);

	ofstream archivo;
	archivo.open("Exports/" + nomImg + "/" + _nomGuardar + ".dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph g \n";
	archivo << "{\nnode[shape=record];\n";
	archivo << "graph[pencolor=transparent];\n";
	archivo << "rankdir=LR;\n";
	archivo << "node [style=filled];\n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	char* charr;
	string dott = "neato -Tjpg Exports/" + nomImg + "/" + _nomGuardar + ".dot -o Exports/" + nomImg + "/" + _nomGuardar + ".jpg";
	charr = (char *)dott.c_str();
	system(charr);
	string im = "start Exports/" + nomImg + "/" + _nomGuardar + ".jpg";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaLinealizacion(string _nomGuardar, string _texto)
{
	//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
	string rut = "Exports/" + nombreDeImagen;
	char* rutt = (char *)rut.c_str();
	int a = _mkdir(rutt);

	ofstream archivo;
	archivo.open("Exports/" + nombreDeImagen + "/" + _nomGuardar + ".dot", ios::out);

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
	string dott = "dot -Tpng Exports/" + nombreDeImagen + "/" + _nomGuardar + ".dot -o Exports/" + nombreDeImagen + "/" + _nomGuardar + ".png";
	charr = (char *)dott.c_str();
	system(charr);
	string im = "start Exports/" + nombreDeImagen + "/" + _nomGuardar + ".png";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaLinealizacionDesdeArbol(string _nomGuardar, string _texto, string nomImg)
{
	//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
	string rut = "Exports/" + nomImg;
	char* rutt = (char *)rut.c_str();
	int a = _mkdir(rutt);

	ofstream archivo;
	archivo.open("Exports/" + nomImg + "/" + _nomGuardar + ".dot", ios::out);

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
	string dott = "dot -Tpng Exports/" + nomImg + "/" + _nomGuardar + ".dot -o Exports/" + nomImg + "/" + _nomGuardar + ".png";
	charr = (char *)dott.c_str();
	system(charr);
	string im = "start Exports/" + nomImg + "/" + _nomGuardar + ".png";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaRecorrido(string nomRec, string _texto)
{
	ofstream archivo;
	archivo.open(nomRec + ".dot", ios::out);

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
	string dott = "dot -Tpng " + nomRec + ".dot -o " +  nomRec + ".png";
	charr = (char *)dott.c_str();
	system(charr);
	string im ="start " + nomRec+".jpg";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaArbol(string nomRec, string _texto)
{
	ofstream archivo;
	archivo.open(nomRec + ".dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph G { \n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	char* charr;
	string dott = "dot -Tpng " + nomRec + ".dot -o " + nomRec + ".png";
	charr = (char *)dott.c_str();
	system(charr);
	string im ="start " + nomRec + ".png";
	char* charr2 = (char *)im.c_str();
	system(charr2);
}

void graphvizEscrituraParaListaDobleFiltros(string _texto)
{
	ofstream archivo;
	archivo.open("FILTROS.dot", ios::out);

	if (archivo.fail()){
		printf("error");
	}

	string est = _texto;
	archivo << "digraph g \n";
	archivo << "{\nnode[shape=record];\n";
	archivo << "graph[pencolor=transparent];\n";
	archivo << "rankdir=LR;\n";
	archivo << "node [style=filled];\n";
	archivo << est;
	archivo << "\n }";
	archivo.close();

	system("dot -Tjpg FILTROS.dot -o FILTROS.jpg");
	system("start FILTROS.jpg");
}

void guardarCuboEnListaDoble(string _filtro)
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;

		do
		{
			if (actual->nomFiltro.compare(_filtro)==0) //cuando encuentre el nombre del filtro
			{
				if (actual->apuntaCopiaCubo==NULL) // si aun no hay cubo en ese nodo
				{
					actual->apuntaCopiaCubo = copCubo.raizCopCubo->siguiente; //se enlaza o guarda en el nodo el cubo
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

void copiarFiltroACuboCopia(string filtro)
{
	if (listFiltros.primero != NULL)
	{
		nodoFiltro *actual = listFiltros.primero;

		do
		{
			if (actual->nomFiltro.compare(filtro)==0) //cuando encuentre el nombre del filtro
			{
					copCubo.raizCopCubo->siguiente=actual->apuntaCopiaCubo; //se enlaza o guarda en el nodo el cubo
					break;
			}
			actual = actual->siguiente;

		} while (actual != listFiltros.primero);
	}
}

void generarHTML()
{
	int dim = arrDimImage[0];

	if (banderaMosaico==true) // si es mosaico
	{
		string cadena = "";
		cadena += "<!DOCTYPE html> \n";
		cadena += "<head> \n";
		cadena += "<link rel = \"stylesheet\" href = \""+ nombreDeImagen + "_" + nombreFiltroActual +".css\"> \n";
		cadena += "</head> \n";
		cadena += "<body> \n";
		cadena += "<div class = \"canvasPrincipal\"> \n";

		int image_width = config[0];
		int image_height = config[1];

		int op = image_width * image_height;
		int op2 = image_width * image_height;
		
		for (int i = 1; i <= op; i++)
		{
			cadena += "<div class = \"canvas"+ to_string(i) +"\"> \n";

			for (int j = 0; j < op2; j++)
			{
				cadena += "<div class=\"pixel\"></div> \n";
			}

			cadena += "</div> \n";
		}

		cadena += "</div> \n";

		cadena += "</body> \n";
		cadena += "</html> \n";

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int a = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".html", ios::out);
		
		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}
	else if (dim!=0) //si es diferente a 0 es porque es collage 
	{
		string cadena = "";
		cadena += "<!DOCTYPE html> \n";
		cadena += "<head> \n";
		cadena += "<link rel = \"stylesheet\" href = \"" + nombreDeImagen + "_" + nombreFiltroActual + ".css\"> \n";
		cadena += "</head> \n";
		cadena += "<body> \n";
		cadena += "<div class = \"canvas\"> \n";

		int image_width = config[0];
		int image_height = config[1];
		int pixel_width = config[2];
		int pixel_height = config[3];

		int op = image_width * image_height;

		for (int i = 0; i < dim; i++)
		{
			cadena += "<div class=\"pixel\"></div> \n";
		}

		cadena += "</div> \n";
		cadena += "</body> \n";
		cadena += "</html> \n";

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int a = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".html", ios::out);

		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}
	else{
		string cadena = "";
		cadena += "<!DOCTYPE html> \n";
		cadena += "<head> \n";
		cadena += "<link rel = \"stylesheet\" href = \"" + nombreDeImagen + "_" + nombreFiltroActual + ".css\"> \n";
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

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int a = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".html", ios::out);

		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}
	
}

void generarCSS()
{
	int aa = arrDimImage[0]; //para saber si es collage (me modifica los dos arreglos a la vez si es collage, arrDimImage y arrDimImageCss)

	if (banderaMosaico==true)
	{
		string cadena = "";
		cadena += "body{ \n";
		cadena += "height: 100vh; \n";
		cadena += "display: flex; \n";
		cadena += "justify-content: center; \n";
		cadena += "align-items: center; } \n";

		cadena += ".canvasPrincipal { \n";
		cadena += "background: #A5A5A5; \n";

		int image_width = config[0];
		int image_height = config[1];
		int pixel_width = config[2];
		int pixel_height = config[3];
		int op = image_width * pixel_width;
		int op2 = image_height*pixel_height;

		cadena += "width: " + to_string(op) + "px; \n";
		cadena += "height: " + to_string(op2) + "px; \n";		
		cadena += " } \n";

		int canv = image_width*image_height;
		for (int i = 1; i <= canv; i++)
		{
			cadena += ".canvas"+ to_string(i) + " { \n";
			cadena += "width: "+ to_string(pixel_width) +"px; \n";
			cadena += "height: " + to_string(pixel_height) +"px;  \n";
			cadena += "float:left; } \n";
		}

		double a = (double)pixel_width;
		double b = (double)image_width;
		double pix = a / b;

		cadena += ".pixel{ \n";
		cadena += "width: "+ to_string(pix) +"px; \n";
		cadena += "height: " + to_string(pix) + "px; \n";
		cadena += "float:left; \n";
		cadena += "} \n";

		//GENERA CADA PIXEL DE LA MATRIZ, SE JALAN DESDE MATRIZ ORIGINAL PORQUE LA COPIA SE MODIFICA EL TAMANIO
		NodolistaArchivos *aux = larch.raizArch->siguiente;

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

		//PARA GENERAR EL COLOR DE FONDO DE LOS .CANVAS
		NodolistaArchivos *auxc = larch.raizArch->siguiente;

		if (auxc != NULL)
		{
			while (auxc != NULL) //me recorre la lista en este sentido -> -> ->
			{
				nodoMatriz *aux2c = auxc->apuntaRaizDeMatriz->abajo; //(-1,0)

				while (aux2c != NULL) //me recorre la matriz para abajo
				{
					nodoMatriz *aux3c = aux2c->siguiente; //

					while (aux3c != NULL) //me recorre cada fila de la matriz en este sentido -> -> ->
					{
						//FORMULA: (FILAACTUAL*TAMANIOPIXEL)+(COLUMNAACTUAL+1)
						int filaActual = 0;
						int tamPixel = 0;
						int columnaActual = 0;
						int columnaActualn = 0;
						int op1 = 0;
						int op2 = 0;
						filaActual = aux3c->y; //FILA ACTUAL
						tamPixel = config[0]; //TAMANIOPIXEL
						columnaActual = aux3c->x;
						columnaActualn = columnaActual + 1;//COLUMNAACTUAL +1
						op1 = tamPixel*filaActual; // (FILAACTUAL*TAMANIOPIXEL)
						op2 = op1 + columnaActualn; // (FILAACTUAL*TAMANIOPIXEL)+(COLUMNAACTUAL+1)
						string hexad = separarRGB_Y_Extraer_Hexa(aux3c->dato);
						cadena += ".canvas" + to_string(op2) + "{ \n";
						cadena += "background: " + hexad + "; \n";
						cadena += "} \n\n";

						aux3c = aux3c->siguiente;
					}

					aux2c = aux2c->abajo;
				}
				auxc = auxc->siguiente;
			}
		}

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int am = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".css", ios::out);

		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}
	else if (aa!=0) //si es distinto a 0 es collage
	{
		string cadena = "";

		cadena += "body{ \n";
		cadena += "height: 100vh; \n";
		cadena += "display: flex; \n";
		cadena += "justify-content: center; \n";
		cadena += "align-items: center; } \n";

		cadena += "body{ \n";
		cadena += "height: 100vh; \n";
		cadena += "display: flex; \n";
		cadena += "justify - content: center; \n";
		cadena += "align - items: center; } \n";

		cadena += ".canvas { \n";
		cadena += "background: #E3E3E3; \n";
		
		int image_width = arrDimImageCss[0];
		int image_height = arrDimImageCss[1];
		int pixel_width = arrDimImageCss[2];
		int pixel_height = arrDimImageCss[3];
		int op = image_width * pixel_width;
		int op2 = image_height*pixel_height;

		cadena += "width: " + to_string(op) + "px; \n";
		cadena += "height: " + to_string(op2) + "px; \n";
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
						tamPixel = arrDimImageCss[0]; //TAMANIOPIXEL
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

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int am = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".css", ios::out);

		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}
	else{
		string cadena = "";

		cadena += "body{ \n";
		cadena += "height: 100vh; \n";
		cadena += "display: flex; \n";
		cadena += "justify-content: center; \n";
		cadena += "align-items: center; } \n";

		cadena += ".canvas { \n";
		cadena += "background: #E3E3E3; \n";

		int image_width = config[0];
		int image_height = config[1];
		int pixel_width = config[2];
		int pixel_height = config[3];
		int op = image_width * pixel_width;
		int op2 = image_height*pixel_height;

		cadena += "width: " + to_string(op) + "px; \n";
		cadena += "height: " + to_string(op2) + "px; \n";
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

		//creando carpeta con nombre de imagen (Si ya existe directorio no lo crea de nuevo)
		string rut = "Exports/" + nombreDeImagen;
		char* rutt = (char *)rut.c_str();
		int am = _mkdir(rutt);

		ofstream archivo;
		archivo.open("Exports/" + nombreDeImagen + "/" + nombreDeImagen + "_" + nombreFiltroActual + ".css", ios::out);

		if (archivo.fail()){
			printf("error");
		}

		archivo << cadena;

		archivo.close();
	}	

}

void generarHTMLCOLLAGE(int _x, int _y)
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
	int op2 = op*_x;
	int op3 = op2* _y;

	for (int i = 0; i < op3; i++)
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

void generarCSSCOLLAGE(int _x, int _y)
{
	string cadena = "";
	cadena += ".canvas { \n";
	cadena += "background: #bed10f; \n";

	//configuracion de imagen
	int image_width = config[0];
	int image_height = config[1];
	int pixel_width = config[2];
	int pixel_height = config[3];

	//dimensiones de imagen para css
	int op_ = image_width * pixel_width;
	int op2_ = op_*_x;
	int op3_ = op_*_y;
	
	//numero de pixeles
	int op4 = image_width*image_height;//ej. 16x16
	int op5 = op4*_x; //16x16x2
	int op6 = op5*_y; //16x16x2x3
	int op7 = op6 / _y; //(16x16x2x3)/3

	cadena += "width: " + to_string(op2_) + "px; \n";
	cadena += "height: " + to_string(op3_) + "px; \n";
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
					//FORMULA1: (FILAACTUAL*TAMANIOPIXEL)+(COLUMNAACTUAL+1)
					int filaActual = aux3->y; //FILA ACTUAL
					int tamimage = config[0]; //TAMANIMAGE
					int columnaActual = aux3->x;
					int columnaActualn = columnaActual + 1;//COLUMNAACTUAL +1
					int op1 = tamimage*_x*filaActual; // (FILAACTUAL*TAMANIOIMAGE) EJ. CON X=3 Y IMAGE=16, EL ANCHO SERA 16*3=48
					int op2 = op1 + columnaActualn; // (FILAACTUAL*TAMANIOIMAGE)+(COLUMNAACTUAL+1)

					string hexad = separarRGB_Y_Extraer_Hexa(aux3->dato);
					cadena += ".pixel:nth-child(" + to_string(op2) + "){ \n";
					cadena += "background: " + hexad + "; \n";
					cadena += "} \n\n";

					//PARTE 1: SI X>1 ME CREA LAS REPETICIONES EN X
					for (int i = 0; i < _x-1; i++)
					{
						op2 = op2 + tamimage;
						string hexad = separarRGB_Y_Extraer_Hexa(aux3->dato);
						cadena += ".pixel:nth-child(" + to_string(op2) + "){ \n";
						cadena += "background: " + hexad + "; \n";
						cadena += "} \n\n";
					}	

					//PARTE 2: SI Y TIENE UN TAMANIO DE 2, ME CREA LAS REPETICIONES EN Y
					//FORMULA= TAMIMAGEN*FILAACTUAL*X + OP7 + 1 + COLUMNAACUTAL
					int opaux = columnaActual;
					for (int i = 0; i < _x; i++)
					{
						int form1 = tamimage*filaActual;
						int form2 = form1*_x;
						int form3 = form2 + op7;
						int form4 = form3 + 1;
						int form5 = form4 + opaux;
						string hexad = separarRGB_Y_Extraer_Hexa(aux3->dato);
						cadena += ".pixel:nth-child(" + to_string(form5) + "){ \n";
						cadena += "background: " + hexad + "; \n";
						cadena += "} \n\n";
						opaux = opaux + tamimage;
					}

					//PARTE 2: PARA CUANDO Y>2
					//FORMULA= TAMIMAGEN*FILAACTUAL*X + OP7 + 1 + COLUMNAACUTAL
					if (_y>2)
					{
						int aa = op7 + op7; //empezara a crear repeticiones a partir de la tercera fila
						while (aa<op6)
						{							
							int opaux = columnaActual;//columna actual
							for (int i = 0; i < _x; i++)
							{
								int form1 = tamimage*filaActual;
								int form2 = form1*_x;
								int form3 = form2 + aa;
								int form4 = form3 + 1;
								int form5 = form4 + opaux;
								string hexad = separarRGB_Y_Extraer_Hexa(aux3->dato);
								cadena += ".pixel:nth-child(" + to_string(form5) + "){ \n";
								cadena += "background: " + hexad + "; \n";
								cadena += "} \n\n";
								opaux = opaux + tamimage;
							}
							aa = aa + op7;
						}
					}						

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
	string opcion;
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

void opcionesMenu(string _opcion)
{	
	if (_opcion.compare("1")==0)
	{
		subMenuInsertarImagen();
	}
	else if (_opcion.compare("2") == 0){
		subMenuSeleccionarImagen();
	}
	else if (_opcion.compare("3") == 0){
		subMenuFiltros();
	}
	else if (_opcion.compare("4") == 0){
		subMenuEdicionManual();
	}
	else if (_opcion.compare("5") == 0){
		subMenuExportarImagen();
		//generarHTML();
		//generarCSS();
		////restaurando datos
		//arrDimImage[0] = 0;
		//banderaMosaico = false;
		//copCubo.raizCopCubo->siguiente = NULL;
		//copiaCuboDisperso();
	}
	else if (_opcion.compare("6") == 0){
		subMenuReportes();
		//ReporteGraphvizTodasLasCapas();
		//ReporteGraphvizLinealizacionPorFilaTodasCapas();
		//ReporteGraphvizLinealizacionPorColumnaTodasCapas();
	}
	else if (_opcion.compare("7") == 0){
		exit(0);
	}
	else{
		printf("\n<-!!- OPCION INVALIDA -!!-> \n");
	}
}

int main()
{
	menu();

	/*string a = "Mosaico";
	if (strstr(a.c_str(),"Mosaico"))
	{
		printf("yes");
	}*/


	//int contador = 0;
	//insertarEnArbol(arbolImg,"ma",16,16,30,30);
	//insertarEnArbol(arbolImg, "ro", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "gu", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "ab", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "cw", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "na", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "st", 16, 16, 30, 30);
	//insertarEnArbol(arbolImg, "aa", 16, 16, 30, 30);
	//mostrarArbol(arbolImg, contador);
	//printf("\n");

	//reporteArbol(arbolImg);
	//graphvizEscrituraParaArbol("ARBOL",cadrep);


	system("pause");
	return 0;
}

///////////////////////////////////////////////////////////// SUB MENU INSERTAR IMAGEN ///////////////////////////////////////////////////////////////////////

void subMenuInsertarImagen()
{
	string a;
	printf(" \n--> INGRESE NOMBRE DE ARCHIVO PRINCIPAL (SIN EXTENSION)\n");
	cin >> a;
	nombreDeImagen = a;
	nombreFiltroActual = "Original";
	a += ".csv";	
	CrearCuboDisperso(a);
	listDimensionCollage.vaciar();
	copCubo.vaciar();
	listFiltros.vaciar();
	copiaCuboDisperso();

}

///////////////////////////////////////////////////////////// SUB MENU SELECCIONAR IMAGEN ///////////////////////////////////////////////////////////////////////

void subMenuSeleccionarImagen()
{
	if (arbolImg==NULL)
	{
		printf("\nAUN NO HAY IMAGENES EN EL SISTEMA!\n");
	}
	else{
		string a;
		contadorMostMenu = 0;
		printf(" \n-------IMAGENES CARGADAS EN EL SISTEMA-------\n");
		InordenParaMostrarEnMenu(arbolImg);
		printf(" \n--> INGRESE NOMBRE DE IMAGEN A SELECCIONAR\n");
		cin >> a;
		if ((buscarEnArbol(arbolImg, a))==true)
		{
			nombreDeImagen = a;
			larch.raizArch->siguiente = NULL;
			seleccionarImagen(arbolImg, a);
			listDimensionCollage.vaciar();
			copCubo.vaciar();
			listFiltros.vaciar();
			copiaCuboDisperso();
		}else{
			printf(" \n-------NO EXISTE ESA IMAGEN-------\n");
		}
		
	}
	
}

/////////////////////////////////////////////////////////////// SUB MENU FILTROS //////////////////////////////////////////////////////////////////////////////

void subMenuFiltros()
{
	cout << listFiltros.filtrosAplicados() << endl;
	string op1;
	printf(" \n -------FILTROS------ \n");
	printf("1. FILTRO A IMAGEN COMPLETA \n");
	printf("2. FILTRO A UNA CAPA EN ESPECIFICO \n");
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
	printf("6. Collage \n");
	printf("7. Mosaic (Mosaico) \n");
	printf("\n--> ELIJA UNA OPCION: \n");
	cin >> op2;
	if (op2.compare("1") == 0)
	{
		modificarTodasLasCapasDeCuboNegativo();
		listFiltros.insertar("Negativo_En_Todas_Las_Capas"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("Negativo_En_Todas_Las_Capas"); //busca el nodo y lo enlaza al cubo
		nombreFiltroActual = "Negativo_En_Todas_Las_Capas";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else if (op2.compare("2") == 0)
	{
		modificarTodasLasCapasDeCuboGrayScale();
		listFiltros.insertar("GrayScale_En_Todas_Las_Capas"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("GrayScale_En_Todas_Las_Capas"); //busca el nodo y lo enlaza al cubo
		nombreFiltroActual = "GrayScale_En_Todas_Las_Capas";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else if (op2.compare("3") == 0)
	{
		filtroEspejoEjeX();
		listFiltros.insertar("Espejo_En_Eje_X"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("Espejo_En_Eje_X"); //busca el nodo y lo enlaza al cubo
		nombreFiltroActual = "Espejo_En_Eje_X";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else if (op2.compare("4") == 0)
	{
		filtroEspejoEjeY();
		listFiltros.insertar("Espejo_En_Eje_Y"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("Espejo_En_Eje_Y"); //busca el nodo y lo enlaza al cubo
		nombreFiltroActual = "Espejo_En_Eje_Y";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else if (op2.compare("5") == 0)
	{
		filtroEspejoEjeY();
		filtroEspejoEjeX();
		listFiltros.insertar("Espejo_En_Ambos_Ejex"); //se guarda primero en la lista doble
		guardarCuboEnListaDoble("Espejo_En_Ambos_Ejex"); //busca el nodo y lo enlaza al cubo
		nombreFiltroActual = "Espejo_En_Ambos_Ejex";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else if (op2.compare("6") == 0)
	{
		int ingx;
		int ingy;
		printf("\n--> INGRESE REPETICIONES EN X: \n");
		cin >> ingx;
		printf("\n--> INGRESE REPETICIONES EN Y: \n");
		cin >> ingy;
		//me modifica el cubo
		collage(ingx,ingy);
		//redimensiona la imagen para crear pixeles en HTML
		int image_width = config[0];
		int image_height = config[1];
		int pixel_width = config[2];
		int pixel_height = config[3];
		int opp = image_width * image_height;
		int opp2 = opp*ingx;
		int opp3 = opp2*ingy;
		//arrDimImage[0] = opp3;
		//redimensiona para ancho alto de imagen, ancho alto de pixel CSS
		int opp4 = image_width*ingx;
		int opp5 = image_height * ingy;
		//arrDimImageCss[0] = opp4;
		//arrDimImageCss[1] = opp5;
		//arrDimImageCss[2] = pixel_width;
		//arrDimImageCss[3] = pixel_height;
		//me guarda en la lista doble
		string a = "Collage_" + to_string(ingx) + "x" + to_string(ingy);
		listFiltros.insertar(a);
		guardarCuboEnListaDoble(a); //busca el nodo y lo enlaza al cubo
		//nombreFiltroActual = a;
		listDimensionCollage.insertar(a, opp3, opp4, opp5, pixel_width, pixel_height);
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
		
	}
	else if (op2.compare("7") == 0)
	{
		mosaico();
		listFiltros.insertar("Mosaico");
		guardarCuboEnListaDoble("Mosaico"); //busca el nodo y lo enlaza al cubo
		//banderaMosaico = true;
		//nombreFiltroActual = "Mosaico";
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
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
		printf("\n--> INGRESE NOMBRE DE CAPA: \n");
		cin >> ncap;
		if (verificarSiExisteCapa(ncap)==true)
		{
			modificarCapaEspecificaDeCuboNegativo(ncap);
			listFiltros.insertar("Negativo_En_Capa_"+ncap); //se guarda primero en la lista doble
			guardarCuboEnListaDoble("Negativo_En_Capa_" + ncap); //busca el nodo y lo enlaza al cubo
			nombreFiltroActual = "Negativo_En_Capa_" + ncap;
		}
		else{
			printf("\n--> NO EXISTE LA CAPA \n");
		}
	}
	else if (op2.compare("2") == 0)
	{
		string ncap;
		printf("\n--> INGRESE NOMBRE DE CAPA: \n");
		cin >> ncap;
		if (verificarSiExisteCapa(ncap) == true)
		{
			modificarCapaEspecificaDeCuboGrayScale(ncap);
			listFiltros.insertar("GrayScale_En_Capa_" + ncap); //se guarda primero en la lista doble
			guardarCuboEnListaDoble("GrayScale_En_Capa_" + ncap); //busca el nodo y lo enlaza al cubo
			nombreFiltroActual = "GrayScale_En_Capa_" + ncap;
		}
		else{
			printf("\n--> NO EXISTE LA CAPA \n");
		}
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

/////////////////////////////////////////////////////////////// SUB MENU EDICION MANUAL //////////////////////////////////////////////////////////////////////////////

void subMenuEdicionManual()
{
	printf(" \n -------REPORTES------ \n");
	printf("1. IMAGEN ORIGINAL \n");
	printf("2. FILTROS \n");\
	printf("\n ELIJA UNA OPCION: \n");
	string op1;

	cin >> op1;
	if (op1.compare("1") == 0)
	{
		subMenuEdicionManualOp1();
	}
	else if (op1.compare("2") == 0){
		subMenuEdicionManualOp2();
	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
}

void subMenuEdicionManualOp1()
{
	string nc;
	printf("\n INGRESE NOMBRE DE CAPA: \n");
	cin >> nc;
	int op1;
	printf("\n INGRESE COORDENADA X: \n");
	cin >> op1;
	int op2;
	printf("\n INGRESE COORDENADA Y: \n");
	cin >> op2;
	string rrggbb;
	printf("\n INGRESE COLOR FORMATO RGB: \n");
	cin >> rrggbb;
	if (larch.buscarCoordenadasXY(nc,op1,op2)==true)
	{
		larch.modificarCoordenadaXY(nc, op1, op2, rrggbb);
		//copio el cubo con la coordenada ya modificada
		//copCubo.raizCopCubo->siguiente = NULL;
		//copiaCuboDisperso();
		//string a = "Edic_Man_Orig_Coord_" + to_string(op1) + "," + to_string(op2);
		//listFiltros.insertar(a);
		//guardarCuboEnListaDoble(a); //busca el nodo y lo enlaza al cubo
		//nombreFiltroActual = a;

	}
	else{
		printf("\n--> CAPA O COORDENADAS X,Y INEXISTENTES! \n");
	}
}

void subMenuEdicionManualOp2()
{
	cout << listFiltros.filtrosAplicados() << endl;

	string nf;
	printf("\n INGRESE NOMBRE DE FILTRO: \n");
	cin >> nf;
	string nc;
	printf("\n INGRESE NOMBRE DE CAPA: \n");
	cin >> nc;
	int op1;
	printf("\n INGRESE COORDENADA X: \n");
	cin >> op1;
	int op2;
	printf("\n INGRESE COORDENADA Y: \n");
	cin >> op2;
	string rrggbb;
	printf("\n INGRESE COLOR FORMATO RGB: \n");
	cin >> rrggbb;
	if (listFiltros.existeCoordEnCapaDeFiltroEspecifico(nf,nc,op1,op2) == true)
	{
		listFiltros.modificarCoordendasXY(nf, nc, op1, op2, rrggbb);
	}
	else{
		printf("\n--> FILTRO, CAPA O COORDENADAS X,Y INEXISTENTES! \n");
	}
		

}

/////////////////////////////////////////////////////////////// SUB MENU REPORTES //////////////////////////////////////////////////////////////////////////////

void subMenuReportes()
{
	printf(" \n -------REPORTES------ \n");
	printf("1. REPORTE DE IMAGENES IMPORTADAS \n");
	printf("2. REPORTE DE CAPA DE IMAGEN \n");
	printf("3. REPORTE DE MATRIZ LINEAL \n");
	printf("4. REPORTE DE RECORRIDO \n");
	printf("5. REPORTE DE FILTROS \n");
	printf("\n ELIJA UNA OPCION: \n");
	string op1;

	cin >> op1;
	if (op1.compare("1") == 0)
	{
		cadrep = "";
		reporteArbol(arbolImg);
		graphvizEscrituraParaArbol("ARBOL", cadrep);
	}
	else if (op1.compare("2") == 0){
		subMenuReportesOp2();
	}
	else if (op1.compare("3") == 0){
		subMenuReporteOp3();
	}
	else if (op1.compare("4") == 0){
		subMenuReporteOp4();
	}
	else if (op1.compare("5") == 0){
		subMenuReporteOp5();
	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
}

void subMenuReportesOp2()
{
	string nomim;
	printf(" \n-------IMAGENES CARGADAS EN EL SISTEMA-------\n");
	contadorMostMenu = 0;
	InordenParaMostrarEnMenu(arbolImg);
	printf(" \n--> INGRESE NOMBRE DE IMAGEN \n");
	cin >> nomim;
	if ((buscarEnArbol(arbolImg, nomim)) == true) //si existe el nombre de imagen
	{
		string opp;
		printf(" \n -------OPCIONES------ \n");
		printf("1. TODAS LAS CAPAS \n");
		printf("2. CAPA ESPECIFICA \n");
		printf(" \n--> ELIJA UNA OPCION \n");
		cin >> opp;

		if (opp.compare("1") == 0)
		{
			reporteTodasLasCapasDeCuboDeArbol(arbolImg,nomim);
		}
		else if (opp.compare("2") == 0){

			int numcap;
			bool bandeCap = true;
			do
			{
				printf(" \n--> INGRESE NUMERO DE CAPA \n");
				cin >> numcap;

				if (buscarCapaEnArbol(arbolImg, nomim, numcap) == true) //si existe la capa ingresada
				{
					reporteCapaEspecificaDeCuboDeArbol(arbolImg, nomim, numcap);
					bandeCap = false;
				}
				else{
					printf(" \n-------NO EXISTE LA CAPA-------\n");
				}

			} while (bandeCap != false);

		}
		else{
			printf(" \n-------OPCION INVALIDA-------\n");
		}		
	}
	else{
		printf(" \n-------NO EXISTE ESA IMAGEN-------\n");
	}
}

void subMenuReporteOp3()
{
	string nomim;
	printf(" \n-------IMAGENES CARGADAS EN EL SISTEMA-------\n");
	contadorMostMenu = 0;
	InordenParaMostrarEnMenu(arbolImg);
	printf(" \n--> INGRESE NOMBRE DE IMAGEN \n");
	cin >> nomim;
	if ((buscarEnArbol(arbolImg, nomim)) == true) //si existe el nombre de imagen
	{
		int numcap;
		bool bandeCap = true;
		do
		{
			printf(" \n--> INGRESE NUMERO DE CAPA \n");
			cin >> numcap;

			if (buscarCapaEnArbol(arbolImg, nomim, numcap) == true) //si existe la capa ingresada
			{
				printf(" \n -------FORMA DE LINEALIZACION------ \n");
				printf("1. POR COLUMNAS \n");
				printf("2. POR FILAS \n");
				string formlineal;
				printf(" \n--> INGRESE FORMA DE LINEALIZACION \n");
				cin >> formlineal;
				if (formlineal.compare("1") == 0)
				{
					reporteLinealizacionPorColumnaDeCuboDeArbol(arbolImg,nomim,numcap);
				}
				else if (formlineal.compare("2") == 0)
				{
					reporteLinealizacionPorFilaDeCuboDeArbol(arbolImg, nomim, numcap);
				}
				else{
					printf("\n--> OPCION INVALIDA \n");
				}
				bandeCap = false;
			}
			else{
				printf(" \n-------NO EXISTE LA CAPA-------\n");
			}

		} while (bandeCap != false);

		
	}
	else{
		printf(" \n-------NO EXISTE ESA IMAGEN-------\n");
	}
}

void subMenuReporteOp4()
{
	printf(" \n -------TIPOS DE RECORRIDOS------ \n");
	printf("1. INORDER \n");
	printf("2. POSTORDER \n");
	printf("3. PREORDER \n");
	printf("\n ELIJA UNA OPCION: \n");
	string op1;

	cin >> op1;
	if (op1.compare("1") == 0)
	{
		reporteInorder(arbolImg);
		string texto = listLinealizacion.textoGraphviz();
		graphvizEscrituraParaRecorrido("RECORRIDO_INORDER", texto);
		listLinealizacion.vaciar();
	}
	else if (op1.compare("2") == 0){
		reportePreorder(arbolImg);
		string texto = listLinealizacion.textoGraphviz();
		graphvizEscrituraParaRecorrido("RECORRIDO_PREORDER", texto);
		listLinealizacion.vaciar();
	}
	else if (op1.compare("3") == 0){
		reportePosorder(arbolImg);
		string texto = listLinealizacion.textoGraphviz();
		graphvizEscrituraParaRecorrido("RECORRIDO_POSORDER", texto);
		listLinealizacion.vaciar();
	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
}

void subMenuReporteOp5()
{
	printf(" \n -------TIPO DE REPORTE------ \n");
	printf("1. REPORTE DE TODOS LOS FILTROS \n");
	printf("2. REPORTE DE FILTRO INDIVIDUAL \n");
	printf("\n ELIJA UNA OPCION: \n");
	string op1;

	cin >> op1;
	if (op1.compare("1") == 0)
	{
		string rep = listFiltros.reporteFiltros();
		graphvizEscrituraParaListaDobleFiltros(rep);
	}
	else if (op1.compare("2") == 0){
		cout << listFiltros.filtrosAplicados() << endl;

		if (listFiltros.primero!=NULL)//si hay filtros
		{
			string nomfil;
			printf(" \n--> INGRESE NOMBRE DEL FILTRO \n");
			cin >> nomfil;
			if (listFiltros.existeFiltro(nomfil)==true)//si el filtro se encontro
			{
				string oppf;
				printf(" \n -------OPCIONES------ \n");
				printf("1. TODAS LAS CAPAS \n");
				printf("2. CAPA ESPECIFICA \n");
				printf(" \n--> ELIJA UNA OPCION \n");
				cin >> oppf;
				if (oppf.compare("1")==0)
				{
					ReporteGraphvizTodasLasCapasListaFiltros(nomfil);
				}
				else if (oppf.compare("2") == 0){
					int numcap;
					bool bandeCap = true;
					do
					{
						printf(" \n--> INGRESE NUMERO DE CAPA \n");
						cin >> numcap;

						if (listFiltros.existeCapaEnFiltroEspecifico(nomfil,numcap) == true) //si existe la capa ingresada
						{
							ReporteGraphvizCapaEspecificaListaFiltros(nomfil, numcap);
							bandeCap = false;
						}
						else{
							printf(" \n-------NO EXISTE LA CAPA-------\n");
						}

					} while (bandeCap != false);
				}
				else{
					printf("\n--> OPCION INVALIDA \n");
				}
			}
			else{
				printf(" \n-------FILTRO NO ENCONTRADO-------\n");
			}
		}

	}
	else{
		printf("\n--> OPCION INVALIDA \n");
	}
}

/////////////////////////////////////////////////////////////// SUB MENU EXPORTAR IMAGEN //////////////////////////////////////////////////////////////////////////////

void subMenuExportarImagen()
{
	printf(" \n -------EXPORTAR IMAGEN------ \n");
	printf("0. Original \n");
	cout << listFiltros.filtrosAplicados() << endl;
	printf("\n INGRESAR NOMBRE DE IMAGEN A EXPORTAR: \n");
	string nimg;
	cin >> nimg;
	if (nimg.compare("Original")==0) //si es la original
	{
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
		nombreFiltroActual = nimg;
		generarHTML();
		generarCSS();
		//restaurando datos
		arrDimImage[0] = 0;
		banderaMosaico = false;
		copCubo.raizCopCubo->siguiente = NULL;
		copiaCuboDisperso();
	}
	else{
		if (listFiltros.existeFiltro(nimg)==true) //si existe el filtro
		{
			if (strstr(nimg.c_str(), "Mosaico")) //si es mosaico
			{
				banderaMosaico = true;
				nombreFiltroActual = nimg;
				copiarFiltroACuboCopia(nimg);
				generarHTML();
				generarCSS();
				//restaurando datos
				arrDimImage[0] = 0;
				banderaMosaico = false;
				copCubo.raizCopCubo->siguiente = NULL;
				copiaCuboDisperso();
			}
			else if (strstr(nimg.c_str(), "Collage")) //si es collage
			{
				int pixhtmll = listDimensionCollage.extraerPixHTML(nimg);
				int wwcss = listDimensionCollage.extraerwcss(nimg);
				int hhcss = listDimensionCollage.extraerhcss(nimg);
				int ppwwcss = listDimensionCollage.extraerpwcss(nimg);
				int pphhcss = listDimensionCollage.extraerphcss(nimg);
				arrDimImage[0] = pixhtmll;
				arrDimImageCss[0] = wwcss;
				arrDimImageCss[1] = hhcss;
				arrDimImageCss[2] = ppwwcss;
				arrDimImageCss[3] = pphhcss;
				nombreFiltroActual = nimg;
				copiarFiltroACuboCopia(nimg);
				generarHTML();
				generarCSS();
				//restaurando datos
				arrDimImage[0] = 0;
				banderaMosaico = false;
				copCubo.raizCopCubo->siguiente = NULL;
				copiaCuboDisperso();
			}
			else{
				nombreFiltroActual = nimg;
				copiarFiltroACuboCopia(nimg);
				generarHTML();
				generarCSS();
				//restaurando datos
				arrDimImage[0] = 0;
				banderaMosaico = false;
				copCubo.raizCopCubo->siguiente = NULL;
				copiaCuboDisperso();
			}
		}
		else{
			printf("\n NO EXISTE FILTRO! \n");
		}
	}

}