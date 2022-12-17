#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <iomanip>


using namespace std;

const int NUM_CASILLAS = 63;

const int TURNO_POSADA = 1;
const int TURNO_PRISION = 2;
const int TURNO_POZO = 3;

const int NUM_JUGADORES = 4;
const int MAX_JUGADORES = 4;
const int NUM_FILAS_A_DIBUJAR = 3;
const int MAX_PARTIDAS = 10;

typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA }tCasillas;
typedef tCasillas tTablero[NUM_CASILLAS];
//typedef int tJugadores[NUM_JUGADORES];



bool MODO_DEBUG;

typedef struct {
    int casilla;
    int penalizacion;
}tEstadoJugador;

typedef tEstadoJugador tEstadoJugadores[NUM_JUGADORES];


typedef struct {
    tEstadoJugadores arrayJ;
    tTablero tablero;
    int turno_Jugador;
}tEstadoPartida;



typedef tEstadoPartida tArrayPartidas[MAX_PARTIDAS];

typedef struct {
    tArrayPartidas arrayPartidas;
    int contador;
}tListaPartidas;




int tirarDado();
int tirardadoManual();
int quienEmpieza();
void iniciaTablero(tTablero tablero);
bool cargaTablero(tTablero tablero);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasillas tipo, int& posicion);
void buscaCasillaAvanzando(const tTablero tablero, tCasillas tipo, int& posicion);
void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug);
int saltaACasilla(const tTablero tablero, int casillaActual);
void iniciaJugadores(tEstadoJugadores jugadores);
bool esCasillaPremio(const tTablero tablero, int casillaActual);
void tirada(const tTablero tablero, tEstadoJugador& estadoJug, bool MODO_DEBUG);
void pintaTablero(const tEstadoPartida& partida);
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoPartida& partida, int fila, int casillasPorFila);
string casillaAstring(tCasillas casilla);
int partida(tEstadoPartida& estado);

void cargaTablero(tTablero tablero, ifstream& archivo);
bool cargaPartidas(tListaPartidas& partidas);
void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo);
bool insertaNuevaPartida (const tListaPartidas &partidas, const tEstadoPartida& partidaOca);
void guardaTablero (const tTablero tablero, ofstream& archivo);


int main() {
    tEstadoPartida estado;
    //tEstadoJugadores jugadores;
    iniciaTablero(estado.tablero);
    cargaTablero(estado.tablero);
    //pintaTablero(estado);
    //iniciaJugadores(jugadores);
    //estado.turno_Jugador = quienEmpieza();
    //partida(estado);
    srand(time(NULL));
    int dado;
    bool premio;
    //tEstadoJugador estadoJug;
    //tEstadoJugadores jugadores;
    char siguiente = '\n';
    iniciaJugadores(estado.arrayJ);
    cout << "|           --------         --------     -------   -------- " << endl;
    cout << "|          |        |       |        |   |         |        |" << endl;
    cout << "|          |--------|       |        |   |         |--------|" << endl;
    cout << "|          |        |       |        |   |         |        |" << endl;
    cout << "|________  |        |        --------     -------  |        |" << endl;
    pintaTablero(estado);
    cout << "SELECCIONE MODO DE JUEGO:" << endl;
    cout << "MODO NORMAL(0) ------- MODO DEBUG(1)" << endl;
    cin >> MODO_DEBUG;
    int casillaActual;
    estado.turno_Jugador = quienEmpieza();
    while (estado.arrayJ[estado.turno_Jugador - 1].casilla < NUM_CASILLAS) {
        casillaActual = estado.arrayJ[estado.turno_Jugador - 1].casilla;
        if (estado.arrayJ[estado.turno_Jugador - 1].penalizacion > 0) {
            cout << "El jugador " << estado.turno_Jugador << " tiene " << estado.arrayJ[estado.turno_Jugador - 1].penalizacion << " turnos sin jugar" << endl;
            estado.arrayJ[estado.turno_Jugador - 1].penalizacion--;
            if (estado.turno_Jugador < NUM_JUGADORES) {
                estado.turno_Jugador++;
            }
            else {
                estado.turno_Jugador = 1;
            }

            cout << "\t++TURNO DEL JUGADOR " << estado.turno_Jugador << "++" << endl;

        }
        cout << "CASILLA ACTUAL: " << casillaActual << endl;
        if (MODO_DEBUG == true) {
            cout << "VALOR DEL DADO: ";
            dado = tirardadoManual();
            cout << endl;
        }
        else {
            cout << "VALOR DEL DADO: ";
            dado = tirarDado();
            cout << dado << endl;

        }
        casillaActual += dado;
        cout << "PASAS A LA CASILLA: " << casillaActual << endl;
        premio = esCasillaPremio(estado.tablero, casillaActual);
        //LA FUNCION EFECTO TIRADA NS SI FUNCIONARA BIEN CON estadoJug
        efectoTirada(estado.tablero, estado.arrayJ[estado.turno_Jugador - 1]);
        tirada(estado.tablero, estado.arrayJ[estado.turno_Jugador - 1], MODO_DEBUG);
        cout << "PULSE ENTER PARA CONTINUAR";
        pintaTablero(estado);
        cin.get(siguiente);
        cout << endl;
        estado.arrayJ[estado.turno_Jugador - 1].casilla = casillaActual;
        if (estado.arrayJ[estado.turno_Jugador - 1].casilla < NUM_CASILLAS) {
            if (estado.turno_Jugador < NUM_JUGADORES) {
                estado.turno_Jugador++;
            }
            else {
                estado.turno_Jugador = 1;
            }


            cout << "\t++TURNO DEL JUGADOR " << estado.turno_Jugador << "++" << endl;
        }


    }
    cout << "\t **** GANA LA PARTIDA EL JUGADOR " << estado.turno_Jugador << endl;


    return 0;
}

int tirarDado() {
    int dado;
    srand(time(0));
    dado = rand() % 6 + 1;
    return dado;
}
int tirardadoManual() {
    int dado;
    cin >> dado;
    return dado;
}
int quienEmpieza() {
    int jugador;
    srand(time(0));
    jugador = rand() % NUM_JUGADORES + 1;
    cout << "\tCOMIENZA EL JUGADOR " << jugador << endl;
    return jugador;
}

void iniciaTablero(tTablero tablero) {
    for (int i = 0; i < NUM_CASILLAS; i++) {
        tablero[i] = NORMAL;
    }
    tablero[NUM_CASILLAS - 1] = OCA;

}


bool cargaTablero(tTablero tablero) {
    ifstream archivo;
    int normal;
    string nombres_normal;
    bool abre_fichero = false;

    archivo.open("tablero.txt");
    if (archivo.is_open()) {
        abre_fichero = true;
        archivo >> normal >> nombres_normal;
        while (normal != 0) {

            if (nombres_normal == "OCA") {
                tablero[normal - 1] = OCA;
            }
            else if (nombres_normal == "PUENTE1") {
                tablero[normal - 1] = PUENTE1;
            }
            else if (nombres_normal == "PUENTE2") {
                tablero[normal - 1] = PUENTE2;
            }
            else if (nombres_normal == "POSADA") {
                tablero[normal - 1] = POSADA;
            }
            else if (nombres_normal == "DADO1") {
                tablero[normal - 1] = DADO1;
            }
            else if (nombres_normal == "POZO") {
                tablero[normal - 1] = POZO;
            }
            else if (nombres_normal == "LABERINTO") {
                tablero[normal - 1] = LABERINTO;
            }
            else if (nombres_normal == "CARCEL") {
                tablero[normal - 1] = CARCEL;
            }
            else if (nombres_normal == "DADO2") {
                tablero[normal - 1] = DADO2;
            }
            else if (nombres_normal == "CALAVERA") {
                tablero[normal - 1] = CALAVERA;
            }

            archivo >> normal >> nombres_normal;
        }
    }
    else {
        cout << "No se ha podido abrir el archivo" << endl;

    }

    archivo.close();
    return abre_fichero;
}


void buscaCasillaRetrocediendo(const tTablero tablero, tCasillas tipo, int& posicion) {
    if (tipo == DADO2) {
        tipo = DADO1;
    }
    else {
        if (tipo == PUENTE2) {
            tipo = PUENTE1;
        }
    }
    while (posicion < NUM_CASILLAS && tablero[posicion] != tipo) {
        posicion--;

    }
    posicion++;

}


void buscaCasillaAvanzando(const tTablero tablero, tCasillas tipo, int& posicion) {
    if (tipo == DADO1) {
        tipo = DADO2;
    }
    else {
        if (tipo == PUENTE1) {
            tipo = PUENTE2;
        }
    }
    while (posicion < NUM_CASILLAS && tablero[posicion] != tipo) {
        posicion++;

    }
    posicion++;
}


void efectoTirada(const tTablero tablero, tEstadoJugador& estadoJug) {

    if (esCasillaPremio(tablero, estadoJug.casilla)) {
        cout << "ROBER ES ANORMAL" << endl;
        estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
    }
    else {
        if (tablero[estadoJug.casilla - 1] == CALAVERA) {
            estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);

        }
        else {
            if (tablero[estadoJug.casilla - 1] == LABERINTO) {
                estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);

            }
            else {
                if (tablero[estadoJug.casilla - 1] == POSADA) {
                    cout << "HAS CAIDO EN LA POSADA, PIERDES UN TURNO" << endl;
                    estadoJug.penalizacion = estadoJug.penalizacion + 1;

                }
                else {
                    if (tablero[estadoJug.casilla - 1] == CARCEL) {
                        cout << "HAS CAIDO EN LA CARCEL, PIERDES DOS TURNOS" << endl;
                        estadoJug.penalizacion = estadoJug.penalizacion + 2;

                    }
                    else {
                        if (tablero[estadoJug.casilla - 1] == POZO) {
                            cout << "HAS CAIDO EN EL POZO, PIERDES TRES TURNOS" << endl;
                            estadoJug.penalizacion = estadoJug.penalizacion + 3;

                        }
                        else {
                            if (tablero[estadoJug.casilla - 1] == DADO2) {
                                estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
                            }
                            else {
                                if (tablero[estadoJug.casilla - 1] == PUENTE2) {
                                    estadoJug.casilla = saltaACasilla(tablero, estadoJug.casilla);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


int saltaACasilla(const tTablero tablero, int casillaActual) {
    if (esCasillaPremio(tablero, casillaActual)) {
        if (tablero[casillaActual - 1] == OCA && casillaActual < NUM_CASILLAS) {
            tCasillas tipo = OCA;
            cout << "HAS CAIDO EN UNA OCA, PASAS A LA SIGUIENTE OCA" << endl;
            buscaCasillaAvanzando(tablero, tipo, casillaActual);
            cout << "CASILLA ACTUAL: " << casillaActual << endl;
            cout << "DE OCA EN OCA Y TIRO PORQUE ME TOCA" << endl;
        }
        else {
            if (tablero[casillaActual - 1] == PUENTE1) {
                tCasillas tipo = PUENTE2;
                cout << "HAS CAIDO EN UN PUENTE, PASAS A EL SIGUIENTE PUENTE" << endl;
                buscaCasillaAvanzando(tablero, tipo, casillaActual);
                cout << "CASILLA ACTUAL: " << casillaActual << endl;
                cout << "DE PUENTE EN PUENTE Y TIRO PORQUE ME LLEVA LA CORRIENTE" << endl;
            }
            else {
                if (tablero[casillaActual - 1] == DADO1) {
                    tCasillas tipo = DADO2;
                    cout << "HAS CAIDO EN UN DADO, PASAS A EL SIGUIENTE DADO" << endl;
                    buscaCasillaAvanzando(tablero, tipo, casillaActual);
                    cout << "CASILLA ACTUAL: " << casillaActual << endl;
                    cout << "DE DADO A DADO Y TIRO PORQUE ME HA TOCADO" << endl;
                }
            }
        }

    }
    else {
        if (tablero[casillaActual - 1] == CALAVERA) {
            cout << "HAS CAIDO EN LA CALAVERA, RETROCEDES A LA PRIMERA CASILLA" << endl;
            casillaActual = 1;
            cout << "CASILLA ACTUAL: " << casillaActual << endl;
        }
        else {
            if (tablero[casillaActual - 1] == LABERINTO) {
                cout << "HAS CAIDO EN EL LABERINTO, RETROCEDES 12 CASILLAS" << endl;
                casillaActual = casillaActual - 12;
                cout << "CASILLA ACTUAL: " << casillaActual << endl;
            }
            else {
                if (tablero[casillaActual - 1] == DADO2) {
                    tCasillas tipo = DADO2;
                    cout << "HAS CAIDO EN UN DADO, RETROCEDES A EL SIGUIENTE DADO" << endl;
                    buscaCasillaRetrocediendo(tablero, tipo, casillaActual);
                    cout << "CASILLA ACTUAL: " << casillaActual << endl;
                }
                else {
                    if (tablero[casillaActual - 1] == PUENTE2) {
                        tCasillas tipo = PUENTE2;
                        cout << "HAS CAIDO EN UN PUENTE, RETROCEDES AL PUENTE ANTERIOR" << endl;
                        buscaCasillaRetrocediendo(tablero, tipo, casillaActual);
                        cout << "CASILLA ACTUAL: " << casillaActual << endl;

                    }
                }
            }
        }
    }
    return casillaActual;
}



void iniciaJugadores(tEstadoJugadores jugadores) {
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i].casilla = 1;
        jugadores[i].penalizacion = 0;
    }
}

bool esCasillaPremio(const tTablero tablero, int casillaActual) {
    bool premio;
    if (tablero[casillaActual - 1] == OCA || tablero[casillaActual - 1] == PUENTE1 || tablero[casillaActual - 1] == DADO1 || tablero[casillaActual - 1] == PUENTE2 || tablero[casillaActual - 1] == DADO2) {
        premio = true;
        cout << "JAJAJAJA" << endl;
    }
    else {
        premio = false;
    }

    return premio;
}

void tirada(const tTablero tablero, tEstadoJugador& estadoJug, bool MODO_DEBUG) {
    bool premio = esCasillaPremio(tablero, estadoJug.casilla);
    char siguiente = '\n';
    int dado;
    if (premio) {
        while (premio && estadoJug.casilla < NUM_CASILLAS) {
            cout << "VUELVES A TIRAR" << endl;
            cin.get(siguiente);
            if (MODO_DEBUG == true) {
                cout << "VALOR DEL DADO: ";
                dado = tirardadoManual();
                cout << endl;
            }
            else {
                cout << "VALOR DEL DADO: ";
                dado = tirarDado();
                cout << dado << endl;
            }
            estadoJug.casilla = estadoJug.casilla + dado;
            cout << "PASAS A LA CASILLA: " << estadoJug.casilla << endl;
            premio = esCasillaPremio(tablero, estadoJug.casilla);
            efectoTirada(tablero, estadoJug);
        }
    }
}



void pintaTablero(const tEstadoPartida& partid) {

    int casillasPorFila = NUM_CASILLAS / NUM_FILAS_A_DIBUJAR;
    cout << endl;
    for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(partid.tablero, fila, casillasPorFila);
        //LA FUNCION PINTAJUGADORES DA EL ERROR!
        pintaJugadores(partid, fila, casillasPorFila);
    }
    pintaBorde(casillasPorFila);
    cout << endl;
}

void pintaNumCasilla(int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";

    cout << "|" << endl;

}

void pintaBorde(int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|====";

    cout << "|" << endl;

}





void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);

    cout << "|" << endl;

}
// HAY QUE CAMBIAR ALGO EN ESTA FUNCION
void pintaJugadores(const tEstadoPartida& partida, int fila, int casillasPorFila) {
    int casilla;

    int blancos = MAX_JUGADORES - NUM_JUGADORES;
    string b = "";
    for (int i = 0; i < blancos; i++) b = b + " ";
    cout << "|";
    for (int i = 1; i <= casillasPorFila; i++) {
        casilla = i + fila * casillasPorFila;
        for (int jug = 0; jug < NUM_JUGADORES; jug++) {
            if (partida.arrayJ[jug].casilla == casilla)
                cout << jug + 1;
            else
                cout << " ";
        }
        cout << b;
        cout << "|";
    }
    cout << endl;

}

string casillaAstring(tCasillas casilla) {
    string cadena;
    switch (casilla) {
        case NORMAL:
            cadena = " ";
            break;
        case OCA:
            cadena = "OCA";
            break;
        case DADO1:
        case DADO2:
            cadena = "DADO";
            break;
        case PUENTE1:
        case PUENTE2:
            cadena = "PNTE";
            break;
        case POSADA:
            cadena = "PSDA";
            break;
        case CALAVERA:
            cadena = "MUER";
            break;
        case LABERINTO:
            cadena = "LBRN";
            break;
        case POZO:
            cadena = "POZO";
            break;
        case CARCEL:
            cadena = "CRCL";
            break;
    }
    return cadena;
}


//Excepción no controlada en 0x00007FF64A8AC748 en Pruebas_Oca.exe: 0xC0000005 : Infracción de acceso al leer la ubicación 0x00000008A1632C2C.

bool cargaPartidas(tListaPartidas& partidas) {
    string nombre_fichero_partidas;
    ifstream archivo;
    int numero_partidas;
    bool abre_fichero = false;
    cout << "INTRODUCE EL NOMBRE DEL FICHERO DE LAS PARTIDAS: ";
    cin >> nombre_fichero_partidas;
    archivo.open(nombre_fichero_partidas);
    if (archivo.is_open()) {
        abre_fichero = true;
        cin >> numero_partidas;
        for (int i = 1; i < numero_partidas; i++) {
            cargaTablero(partidas.arrayPartidas[i].tablero, archivo);
            cin >> partidas.arrayPartidas[i].turno_Jugador;
            cargaJugadores(partidas.arrayPartidas[i].arrayJ, archivo);
        }

    }
    return abre_fichero;
}

void cargaTablero(tTablero tablero, ifstream& archivo) {
    // hacer string a casilla segun kaka
    int normal;
    string nombres_normal;
    archivo >> normal >> nombres_normal;
    while (normal != 0) {

        if (nombres_normal == "OCA") {
            tablero[normal - 1] = OCA;
        }
        else if (nombres_normal == "PUENTE1") {
            tablero[normal - 1] = PUENTE1;
        }
        else if (nombres_normal == "PUENTE2") {
            tablero[normal - 1] = PUENTE2;
        }
        else if (nombres_normal == "POSADA") {
            tablero[normal - 1] = POSADA;
        }
        else if (nombres_normal == "DADO1") {
            tablero[normal - 1] = DADO1;
        }
        else if (nombres_normal == "POZO") {
            tablero[normal - 1] = POZO;
        }
        else if (nombres_normal == "LABERINTO") {
            tablero[normal - 1] = LABERINTO;
        }
        else if (nombres_normal == "CARCEL") {
            tablero[normal - 1] = CARCEL;
        }
        else if (nombres_normal == "DADO2") {
            tablero[normal - 1] = DADO2;
        }
        else if (nombres_normal == "CALAVERA") {
            tablero[normal - 1] = CALAVERA;
        }

        archivo >> normal >> nombres_normal;
    }
}

void cargaJugadores(tEstadoJugadores& jugadores, ifstream& archivo) {
    archivo.open("partidas.txt");
    for (int i = 0; i < NUM_JUGADORES; i++) { //LO DE NUM JUGADORES HAY QUE VERLO BIEN
        archivo >> jugadores[i].casilla;
        archivo >> jugadores[i].penalizacion;
    }

}

bool insertaNuevaPartida ( tListaPartidas &partidas, const tEstadoPartida& partidaOca) {
    bool insertar = false;
    if  (partidas.contador < MAX_PARTIDAS) {
        partidas.arrayPartidas[partidas.contador] = partidaOca;
        insertar = true;
        partidas.contador++;
    }
    return insertar;
}

void guardaTablero (const tTablero tablero, ofstream& archivo) {
    //archivo << tablero

}

void guardaJugadores (const tEstadoJugadores jugadores, ofstream& archivo) {
    for (int i = 0; i < NUM_JUGADORES; i++) { //falta lo de ver en que momento esta el jugador
        archivo << jugadores[i].casilla << " " << jugadores[i].penalizacion;
    }
}

int partida(tEstadoPartida& estado) {
    srand(time(NULL));
    int dado;
    bool premio;
    tEstadoJugador estadoJug;
    tEstadoJugadores jugadores;
    char siguiente = '\n';
    iniciaJugadores(estado.arrayJ);
    cout << "|           --------         --------     -------   -------- " << endl;
    cout << "|          |        |       |        |   |         |        |" << endl;
    cout << "|          |--------|       |        |   |         |--------|" << endl;
    cout << "|          |        |       |        |   |         |        |" << endl;
    cout << "|________  |        |        --------     -------  |        |" << endl;
    pintaTablero(estado);
    cout << "SELECCIONE MODO DE JUEGO:" << endl;
    cout << "MODO NORMAL(0) ------- MODO DEBUG(1)" << endl;
    cin >> MODO_DEBUG;
    //int casillaActual;
    estado.turno_Jugador = quienEmpieza();
    while (estado.arrayJ[estado.turno_Jugador - 1].casilla < NUM_CASILLAS) {

        if (estado.arrayJ[estado.turno_Jugador - 1].penalizacion > 0) {
            cout << "El jugador " << estado.turno_Jugador << " tiene " << estado.arrayJ[estado.turno_Jugador - 1].penalizacion << " turnos sin jugar" << endl;
            estado.arrayJ[estado.turno_Jugador - 1].penalizacion--;
            if (estado.turno_Jugador < NUM_JUGADORES) {
                estado.turno_Jugador++;
            }
            else {
                estado.turno_Jugador = 1;
            }

            cout << "\t++TURNO DEL JUGADOR " << estado.turno_Jugador << "++" << endl;

        }
        cout << "CASILLA ACTUAL: " << estado.arrayJ[estado.turno_Jugador - 1].casilla << endl;
        if (MODO_DEBUG == true) {
            cout << "VALOR DEL DADO: ";
            dado = tirardadoManual();
            cout << endl;
        }
        else {
            cout << "VALOR DEL DADO: ";
            dado = tirarDado();
            cout << dado << endl;

        }
        estado.arrayJ[estado.turno_Jugador - 1].casilla += dado;
        cout << "PASAS A LA CASILLA: " << estado.arrayJ[estado.turno_Jugador - 1].casilla << endl;
        premio = esCasillaPremio(estado.tablero, estado.arrayJ[estado.turno_Jugador - 1].casilla);

        //LA FUNCION EFECTO TIRADA NS SI FUNCIONARA BIEN CON estadoJug
        efectoTirada(estado.tablero, estadoJug);
        tirada(estado.tablero, estadoJug, MODO_DEBUG);
        cout << "PULSE ENTER PARA CONTINUAR";
        pintaTablero(estado);
        cin.get(siguiente);
        cout << endl;
        //estado.arrayJ[estado.turno_Jugador - 1].casilla = casillaActual;
        if (estado.arrayJ[estado.turno_Jugador - 1].casilla < NUM_CASILLAS) {
            if (estado.turno_Jugador < NUM_JUGADORES) {
                estado.turno_Jugador++;
            }
            else {
                estado.turno_Jugador = 1;
            }


            cout << "\t++TURNO DEL JUGADOR " << estado.turno_Jugador << "++" << endl;
        }


    }
    cout << "\t **** GANA LA PARTIDA EL JUGADOR " << estado.turno_Jugador << endl;
    return estado.turno_Jugador;
}

