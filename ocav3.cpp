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

typedef enum { NORMAL, OCA, PUENTE1, PUENTE2, POZO, POSADA, LABERINTO, DADO1, DADO2, CARCEL, CALAVERA } tCasillas;
typedef tCasillas tTablero[NUM_CASILLAS];
typedef int tJugadores[NUM_JUGADORES];



typedef struct {
    int casilla;
    int penalizacion;
}tEstadoJugador;

typedef tEstadoJugador tEstadoJudadores [NUM_JUGADORES]; //O MAX_JUGADORES

typedef struct {
    tEstadoJudadores estadoJudadores;
    tTablero tablero;
    int jugador;
}tEstadoPartida;

typedef tEstadoPartida tArrayPartidas [MAX_PARTIDAS];

typedef struct {
    int cont;
    tArrayPartidas arrayPartidas;
}tListaPartidas;


int tirarDado();
int tirardadoManual();
int quienEmpieza();
void pintaTablero(const tEstadoPartida &partida); //MODIFICADO
void pintaNumCasilla(int fila, int casillasPorFila);
void pintaBorde(int casillasPorFila);
void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila);
void pintaJugadores(const tEstadoPartida, int fila, int casillasPorFila);
string casillaAstring(tCasillas casilla);
bool cargaTablero(tTablero tablero);
void iniciaTablero(tTablero tablero);
void buscaCasillaRetrocediendo(const tTablero tablero, tCasillas tipo, int& posicion);
void buscaCasillaAvanzando(const tTablero tablero, tCasillas tipo, int& posicion);
void efectoTirada(const tTablero tablero, tEstadoJugador &estadoJug); //MODIFICADO
int saltaACasilla(const tTablero tablero, int casillaActual);
void iniciaJugadores(tEstadoJudadores jugadores); //MODIFICADO
bool esCasillaPremio(const tTablero tablero, int casilla);
int partida(tEstadoPartida &estado);//MODIFICADO
void tirada(const tTablero tablero, tEstadoJugador &estadoJug, bool MODO_DEBUG);

bool MODO_DEBUG;

bool cargaPartidas (tListaPartidas &partidas);
void cargaTablero (tTablero tablero, ifstream &archivo);
void cargaJugadores ( tEstadoJudadores &jugadores, ifstream &archivo);



int main() {
    tEstadoPartida estado;

    iniciaTablero(estado.tablero);
    cargaTablero(estado.tablero);
    partida(estado);

    return 0;
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
bool esCasillaPremio(const tTablero tablero, int casilla) {
    bool premio = false;
    if (tablero[casilla - 1] == OCA || tablero[casilla - 1] == PUENTE1 || tablero[casilla - 1] == DADO1 || tablero[casilla - 1] == PUENTE2 || tablero[casilla - 1] == DADO2) {
        premio = true;
    }
    return premio;
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
void efectoTirada(const tTablero tablero, tEstadoJugador &estadoJug) {

    if (esCasillaPremio(tablero, estadoJug.casilla)) {
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

void iniciaJugadores(tEstadoJudadores jugadores) {
    for (int i = 0; i < NUM_JUGADORES; i++) {
        jugadores[i].casilla = 1;
        jugadores[i].penalizacion = 0;
    }
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

void pintaTablero(const tEstadoPartida &partida) {

    int casillasPorFila = NUM_CASILLAS / NUM_FILAS_A_DIBUJAR;
    cout << endl;
    for (int fila = 0; fila < NUM_FILAS_A_DIBUJAR; fila++) {
        pintaBorde(casillasPorFila);
        pintaNumCasilla(fila, casillasPorFila);
        pintaTipoCasilla(partida.tablero, fila, casillasPorFila);
        pintaJugadores(partida, fila, casillasPorFila); //mal
    }
    pintaBorde(casillasPorFila);
    cout << endl;
}


void pintaBorde(int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|====";

    cout << "|" << endl;

}


void pintaNumCasilla(int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "| " << setw(2) << setfill('0') << i + fila * casillasPorFila << " ";

    cout << "|" << endl;

}


void pintaTipoCasilla(const tTablero tablero, int fila, int casillasPorFila) {

    for (int i = 1; i <= casillasPorFila; i++)
        cout << "|" << setw(4) << setfill(' ') << casillaAstring(tablero[i - 1 + fila * casillasPorFila]);

    cout << "|" << endl;

}

void pintaJugadores(const tEstadoPartida estadoPartida, int fila, int casillasPorFila) {
    int casilla;

    int blancos = MAX_JUGADORES - NUM_JUGADORES;
    string b = "";
    for (int i = 0; i < blancos; i++) b = b + " ";
    cout << "|";
    for (int i = 1; i <= casillasPorFila; i++) {
        casilla = i + fila * casillasPorFila;
        for (int jug = 0; jug < NUM_JUGADORES; jug++) {
            if (estadoPartida.estadoJudadores[jug].casilla == casilla) //error despues de iniciar el juego
                cout << jug + 1;
            else
                cout << " ";
        }
        cout << b;
        cout << "|";
    }
    cout << endl;

}

void tirada(const tTablero tablero, tEstadoJugador &estadoJug, bool MODO_DEBUG) {
    bool premio = esCasillaPremio(tablero, estadoJug.casilla);
    char siguiente = '\n';
    int dado;
    if (premio) {
        while (premio && estadoJug.casilla < NUM_CASILLAS) {
            cout << "VUELVES A TIRAR" << endl;
            cin.get(siguiente);
            if (MODO_DEBUG) {
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


int partida(tEstadoPartida &estado) {
    srand(time(NULL));
    char siguiente = '\n';
    bool premio;
    int casillaActual = 1;
    int penalizacionJ = 0;
    tJugadores casillasJ;
    tJugadores penalizacion;
    tEstadoJugador estadoJugador;
    tEstadoJudadores jugadores;

    iniciaJugadores(jugadores);
    int dado;
    int jugador;
    cout << "|           --------         --------     –––––––   -------- "  << endl;
    cout << "|          |        |       |        |   |         |        |"  << endl;
    cout << "|          |--------|       |        |   |         |--------|"  << endl;
    cout << "|          |        |       |        |   |         |        |"  << endl;
    cout << "|________  |        |        --------     –––––––  |        |"  << endl;
    pintaTablero(estado);;
    cout << "\t**** LA OCA ****" << endl;
    cout << "SELECCIONE MODO DE JUEGO:" << endl;
    cout << "MODO NORMAL(0) ------- MODO DEBUG(1)" << endl;
    cin >> MODO_DEBUG;

    jugador = quienEmpieza();
    while (casillasJ[jugador - 1] < NUM_CASILLAS) {
        penalizacionJ = penalizacion[jugador - 1];
        if (penalizacionJ > 0) {
            cout << "El jugador " << jugador << " tiene " << penalizacion[jugador - 1] << " turnos sin jugar" << endl;
            penalizacion[jugador - 1]--;
            if (jugador < NUM_JUGADORES) {
                jugador++;
            }
            else {
                jugador = 1;
            }

            cout << "\t++TURNO DEL JUGADOR " << jugador << "++" << endl;
            penalizacionJ = penalizacion[jugador - 1];
        }

        casillaActual = casillasJ[jugador - 1];
        cout << "CASILLA ACTUAL: " << casillaActual << endl;
        if (MODO_DEBUG) {
            cout << "VALOR DEL DADO: ";
            dado = tirardadoManual();
            cout << endl;
        }
        else {
            cout << "VALOR DEL DADO: ";
            dado = tirarDado();
            cout << dado << endl;
        }
        casillaActual = casillaActual + dado;
        cout << "PASAS A LA CASILLA: " << casillaActual << endl;
        premio = esCasillaPremio(estado.tablero, casillaActual);
        efectoTirada(estado.tablero, estadoJugador);
        tirada(estado.tablero, estadoJugador, MODO_DEBUG);
        penalizacion[jugador - 1] = penalizacionJ;
        casillasJ[jugador - 1] = casillaActual;
        cout << "PULSE ENTER PARA CONTINUAR";
        pintaTablero(estado);
        cin.get(siguiente);
        cout << endl;
        if (casillasJ[jugador - 1] < NUM_CASILLAS) {
            if (jugador == NUM_JUGADORES && casillasJ[jugador - 1] < NUM_CASILLAS) {
                jugador = 1;
            }
            else {
                jugador++;
            }
            cout << "\t++TURNO DEL JUGADOR " << jugador << "++" << endl;
        }
    }
    cout << "\t **** GANA LA PARTIDA EL JUGADOR " << jugador << endl;
    return jugador;
}




/*bool cargaPartidas (tListaPartidas &partidas) {

}


void cargaTablero (tTablero tablero, ifstream &archivo) {
    archivo.open("tablero.txt");
}

void cargaJugadores ( tEstadoJudadores &jugadores, ifstream &archivo) {
    archivo.open("partidas.txt");
    int n;
    archivo >> n;
    while (n != 0) {
        archivo >> n;
    }


}*/
