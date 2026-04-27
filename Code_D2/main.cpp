// ======================================================================================
// Universidad de Antioquia
// Facultad de Ingenieria - Departamento de Telecomunicaciones -
// @authors:
//        Clayder Areiza          CC: 1042150543
//        Arlington Zahir Llerena CC: 72 277 711
// Informatica II - Semestre - 1
// Desafio II
// 26 abril de 2026
// ======================================================================================
// Librerias invocadas en el script

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <cstdlib>
#include <string>

// ======================================================================================
// ETAPA 1: INFRAESTRUCTURA BASE DE C++ (Plantillas y Memoria Dinámica)
// ======================================================================================

template <class T>
class Lista {
private:
    T* elementos;
    int tamanoActual, capacidad;
    static long long totalIteraciones;
    static size_t memoriaEnUso;

    void redimensionar() {
        capacidad *= 2;
        T* nuevoEspacio = new T[capacidad];
        memoriaEnUso += sizeof(T) * capacidad;
        for (int i = 0; i < tamanoActual; i++) {
            nuevoEspacio[i] = elementos[i];
            totalIteraciones++;
        }
        delete[] elementos;
        elementos = nuevoEspacio;
    }

public:
    Lista() : tamanoActual(0), capacidad(5) {
        elementos = new T[capacidad];
        memoriaEnUso += sizeof(T) * capacidad;
    }
    ~Lista() { delete[] elementos; }

    void agregar(T objeto) {
        if (tamanoActual == capacidad) redimensionar();
        elementos[tamanoActual++] = objeto;
        totalIteraciones++;
    }

    T& obtener(int indice) {
        totalIteraciones++;
        return elementos[indice];
    }

    int getTamano() const { return tamanoActual; }

    static void mostrarAuditoria() {
        std::cout << "\n============================================\n";
        std::cout << "|   REPORTE DE EFICIENCIA (AUDITORIA UdeA) |\n";
        std::cout << "============================================\n";
        std::cout << "-> Iteraciones de Memoria: " << totalIteraciones << "\n";
        std::cout << "-> Memoria Dinamica en uso: " << memoriaEnUso << " bytes\n";
        std::cout << "============================================\n\n";
    }
};

template <class T> long long Lista<T>::totalIteraciones = 0;
template <class T> size_t Lista<T>::memoriaEnUso = 0;

// ======================================================================================
// CLASES DE REGISTRO TÉCNICO
// ======================================================================================

class RegistroPoisson {
public:
    std::string local, visita;
    float lambdaL, lambdaV;
    int golesL, golesV;

    void configurar(std::string l, std::string v, float la, float lb, int ga, int gb) {
        local = l; visita = v; lambdaL = la; lambdaV = lb; golesL = ga; golesV = gb;
    }
};

class RegistroHistorial {
public:
    std::string fase, id, local, visita, ganador;
    int golesL, golesV;

    void configurar(std::string f, std::string i, std::string l, std::string v, std::string gan, int gl, int gv) {
        fase = f; id = i; local = l; visita = v; ganador = gan; golesL = gl; golesV = gv;
    }
};

// ======================================================================================
// ETAPA 2: ENTIDAD EQUIPO
// ======================================================================================

class Equipo {
private:
    char rankingFIFA[50], pais[50], dt[50], fed[100], conf[20];
    char gf_historico[10], gc_historico[10], pg_h[10], pe_h[10], pp_h[10];
    int puntos, golesFavor, golesContra, diferenciaGoles, pg, pe, pp, pesoJerarquia;
    std::string etapa;
    char grupoOrigen;

public:
    Equipo() : puntos(0), golesFavor(0), golesContra(0), diferenciaGoles(0), pg(0), pe(0), pp(0), pesoJerarquia(8), etapa("Fase de Grupos"), grupoOrigen('?') { pais[0] = '\0'; }

    void setGrupo(char g) { grupoOrigen = g; }
    char getGrupo() const { return grupoOrigen; }
    int getPeso() const { return pesoJerarquia; }
    void setEtapaFinal(std::string e, int p) { if(p < pesoJerarquia) { etapa = e; pesoJerarquia = p; } }

    bool esIgualA(const char* nombreBuscado) {
        int i = 0; while (pais[i] != '\0' && nombreBuscado[i] != '\0') { if (pais[i] != nombreBuscado[i]) return false; i++; }
        return (pais[i] == nombreBuscado[i]);
    }

    void configurarDesdeArreglo(char datos[10][100]) {
        auto copiar = [](char* dest, const char* src, int limite) {
            int i = 0; while (src[i] != '\0' && i < limite - 1) { dest[i] = src[i]; i++; } dest[i] = '\0';
        };
        copiar(rankingFIFA, datos[0], 50); copiar(pais, datos[1], 50); copiar(dt, datos[2], 50);
        copiar(fed, datos[3], 100); copiar(conf, datos[4], 20); copiar(gf_historico, datos[5], 10);
        copiar(gc_historico, datos[6], 10); copiar(pg_h, datos[7], 10); copiar(pe_h, datos[8], 10); copiar(pp_h, datos[9], 10);
    }

    float getPromedioGF() const {
        int t = std::atoi(pg_h) + std::atoi(pe_h) + std::atoi(pp_h);
        return (t == 0) ? 0.01f : (float)std::atof(gf_historico) / t;
    }

    float getPromedioGC() const {
        int t = std::atoi(pg_h) + std::atoi(pe_h) + std::atoi(pp_h);
        return (t == 0) ? 0.01f : (float)std::atof(gc_historico) / t;
    }

    void registrarResultado(int favor, int contra) {
        golesFavor += favor; golesContra += contra; diferenciaGoles = golesFavor - golesContra;
        if (favor > contra) { puntos += 3; pg++; } else if (favor == contra) { puntos += 1; pe++; } else pp++;
    }

    int getPuntos() const { return puntos; }
    int getGD() const { return diferenciaGoles; }
    const char* getNombre() const { return pais; }
    int getRankingInt() const { return std::atoi(rankingFIFA); }

    void exportarBase(std::ofstream& a) { a << rankingFIFA << ";" << pais << ";" << dt << ";" << fed << ";" << conf << ";" << gf_historico << ";" << gc_historico << ";" << pg_h << ";" << pe_h << ";" << pp_h << "\n"; }
    void exportarSimulado(std::ofstream& a) { a << pais << ";" << puntos << ";" << pg << ";" << pe << ";" << pp << ";" << golesFavor << ";" << golesContra << ";" << diferenciaGoles << "\n"; }
    void exportarEscalafon(std::ofstream& a) { a << pais << ";" << etapa << ";" << puntos << ";" << diferenciaGoles << ";" << rankingFIFA << "\n"; }
};

// ======================================================================================
// ETAPA 3: GESTORES (Input/Output) Y LÓGICA DE CLASIFICACIÓN
// ======================================================================================

class GestorArchivos {
public:
    // CORRECCIÓN: Se reintroduce el parámetro lineasCabecera
    bool leerArchivo(std::string ruta, Lista<Equipo>& lista, int lineasCabecera) {
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) {
            std::cout << "[ERROR CRITICO] No se encuentra el archivo en: " << ruta << "\n";
            return false;
        }

        char linea[1000];
        for(int i=0; i < lineasCabecera; i++) {
            archivo.getline(linea, 1000);
        }

        while (archivo.getline(linea, 1000)) {
            char campos[10][100]; int c=0, j=0;
            for (int i=0; linea[i]!='\0'; i++) {
                if (linea[i]==';') { campos[c][j]='\0'; c++; j=0; }
                else { if(c < 10) campos[c][j++] = linea[i]; }
            }
            campos[c][j]='\0';

            if(c > 0) { // Evita procesar líneas vacías al final del CSV
                Equipo equipoNuevo;
                equipoNuevo.configurarDesdeArreglo(campos);
                lista.agregar(equipoNuevo);
            }
        }
        archivo.close();
        return true;
    }

    void generarGruposBase(std::string rutaBase, Lista<Equipo>& maestro) {
        std::cout << "\n[Fase 1] Extrayendo y generando 12 archivos base de la FIFA...\n";
        const char* nombresGrupos[] = {"Grupo_A","Grupo_B","Grupo_C","Grupo_D","Grupo_E","Grupo_F","Grupo_G","Grupo_H","Grupo_I","Grupo_J","Grupo_K","Grupo_L"};
        const char* matrizFIFA[12][4] = {{"Mexico","South Africa","Korea Republic","Czech Republic"},{"Canada","Bosnia and Herzegovina","Qatar","Denmark"},{"Brazil","Morocco","Haiti","Scotland"},{"United States","Paraguay","Australia","Turkey"},{"Germany","Serbia","Nigeria","Costa Rica"},{"Netherlands","Japan","Sweden","Tunisia"},{"Belgium","Egypt","Iran","New Zealand"},{"Spain","Cabo Verde","Saudi Arabia","Uruguay"},{"France","Senegal","Iraq","Cameroon"},{"Argentina","Italy","Austria","Jordan"},{"Portugal","DR Congo","Uzbekistan","Colombia"},{"England","Croatia","Ghana","Panama"}};

        Lista<Equipo> controlDuplicados;
        for(int g=0; g<12; g++) {
            std::ofstream archivoCSV(rutaBase + "/" + nombresGrupos[g] + ".csv");
            if(archivoCSV.is_open()) {
                archivoCSV << "Ranking;Pais;DT;Fed;Conf;GF;GC;PG;PE;PP\n";
                int asignados = 0;
                for(int p=0; p<4; p++) {
                    for(int i=0; i<maestro.getTamano(); i++) {
                        if(maestro.obtener(i).esIgualA(matrizFIFA[g][p])) {
                            maestro.obtener(i).exportarBase(archivoCSV);
                            controlDuplicados.agregar(maestro.obtener(i));
                            asignados++; break;
                        }
                    }
                }
                if(asignados < 4) {
                    for(int i=0; i<maestro.getTamano() && asignados < 4; i++) {
                        bool yaExiste = false;
                        for(int u=0; u<controlDuplicados.getTamano(); u++) if(maestro.obtener(i).esIgualA(controlDuplicados.obtener(u).getNombre())) yaExiste=true;
                        if(!yaExiste) { maestro.obtener(i).exportarBase(archivoCSV); controlDuplicados.agregar(maestro.obtener(i)); asignados++; }
                    }
                }
                archivoCSV.close();
            }
        }
        std::cout << "-> 12 Grupos extraidos exitosamente.\n";
    }
};

class ClasificadorLlaves {
public:
    Lista<Equipo> primeros, segundos, terceros, mejoresTerceros, tercerosUsados;
    void registrarPosiciones(Lista<Equipo>& grupo, char letra) {
        grupo.obtener(0).setGrupo(letra); primeros.agregar(grupo.obtener(0));
        grupo.obtener(1).setGrupo(letra); segundos.agregar(grupo.obtener(1));
        grupo.obtener(2).setGrupo(letra); terceros.agregar(grupo.obtener(2));
    }
    void seleccionarMejoresTerceros() {
        for(int i=0; i<terceros.getTamano()-1; i++) for(int j=0; j<terceros.getTamano()-i-1; j++) {
                if(terceros.obtener(j).getPuntos() < terceros.obtener(j+1).getPuntos() || (terceros.obtener(j).getPuntos() == terceros.obtener(j+1).getPuntos() && terceros.obtener(j).getGD() < terceros.obtener(j+1).getGD())) {
                    Equipo temp = terceros.obtener(j); terceros.obtener(j) = terceros.obtener(j+1); terceros.obtener(j+1) = temp;
                }
            }
        for(int i=0; i<8; i++) mejoresTerceros.agregar(terceros.obtener(i));
    }
    Equipo buscarTercero(const char* gruposPermitidos) {
        for(int i=0; i<mejoresTerceros.getTamano(); i++) {
            Equipo& candidato = mejoresTerceros.obtener(i); bool enUso = false;
            for(int u=0; u<tercerosUsados.getTamano(); u++) if(candidato.esIgualA(tercerosUsados.obtener(u).getNombre())) enUso = true;
            if(!enUso) {
                bool cumpleRegla = false; for(int p=0; gruposPermitidos[p]!='\0'; p++) if(candidato.getGrupo()==gruposPermitidos[p]) cumpleRegla = true;
                if(cumpleRegla) { tercerosUsados.agregar(candidato); return candidato; }
            }
        }
        for(int i=0; i<mejoresTerceros.getTamano(); i++) {
            Equipo& candidato = mejoresTerceros.obtener(i); bool enUso = false;
            for(int u=0; u<tercerosUsados.getTamano(); u++) if(candidato.esIgualA(tercerosUsados.obtener(u).getNombre())) enUso = true;
            if(!enUso) { tercerosUsados.agregar(candidato); return candidato; }
        }
        return mejoresTerceros.obtener(0);
    }
};

// ======================================================================================
// ETAPA 4: MOTOR MATEMÁTICO
// ======================================================================================

class MotorMundial {
private:
    const float ALFA=0.6f, BETA=0.4f, MU=1.35f;
    std::mt19937 randomizador;
public:
    MotorMundial() : randomizador(std::random_device{}()) {}

    void ejecutarFaseGrupos(std::string rutaBase, GestorArchivos& io, ClasificadorLlaves& llaves, Lista<RegistroHistorial>& historial, Lista<Equipo>& maestro) {
        const char letras[] = {'A','B','C','D','E','F','G','H','I','J','K','L'};
        std::cout << "\n==================================================================================\n";
        std::cout << "|                         FASE DE GRUPOS (72 PARTIDOS)                           |\n";
        std::cout << "==================================================================================\n";

        for(int k=0; k<12; k++) {
            std::string rutaCSV = rutaBase + "/Grupo_" + letras[k] + ".csv";
            Lista<Equipo> grupoActual;

            // CORRECCIÓN: Se indica que los archivos de Grupo SOLO tienen 1 línea de cabecera
            io.leerArchivo(rutaCSV, grupoActual, 1);

            if(grupoActual.getTamano() < 4) {
                std::cout << "[ALERTA] Fallo de integridad en Grupo " << letras[k] << "\n";
                continue;
            }

            Lista<RegistroPoisson> logTecnico;
            for(int i=0; i<3; i++) for(int j=i+1; j<4; j++) {
                    Equipo& eqA = grupoActual.obtener(i); Equipo& eqB = grupoActual.obtener(j);

                    float lambdaA = MU * std::pow((eqA.getPromedioGF()/MU), ALFA) * std::pow((eqB.getPromedioGC()/MU), BETA);
                    float lambdaB = MU * std::pow((eqB.getPromedioGF()/MU), ALFA) * std::pow((eqA.getPromedioGC()/MU), BETA);
                    std::poisson_distribution<int> distribucionA(lambdaA), distribucionB(lambdaB);

                    int golesA = distribucionA(randomizador); int golesB = distribucionB(randomizador);
                    eqA.registrarResultado(golesA, golesB); eqB.registrarResultado(golesB, golesA);

                    RegistroPoisson rp; rp.configurar(eqA.getNombre(), eqB.getNombre(), lambdaA, lambdaB, golesA, golesB); logTecnico.agregar(rp);

                    std::string identificadorFase = "Grupo "; identificadorFase += letras[k];
                    RegistroHistorial rh; rh.configurar(identificadorFase, "GRP", eqA.getNombre(), eqB.getNombre(), (golesA>golesB?eqA.getNombre():(golesB>golesA?eqB.getNombre():"Empate")), golesA, golesB);
                    historial.agregar(rh);

                    std::cout << "| " << identificadorFase << " | " << eqA.getNombre() << " " << golesA << " - " << golesB << " " << eqB.getNombre() << "\n";
                }

            // SINCRONIZACIÓN DE PUNTOS AL MAESTRO (ACTIVO Y FUNCIONAL)
            for(int i=0; i<grupoActual.getTamano(); i++) {
                for(int m=0; m<maestro.getTamano(); m++) {
                    if(maestro.obtener(m).esIgualA(grupoActual.obtener(i).getNombre())) { maestro.obtener(m) = grupoActual.obtener(i); }
                }
            }

            for(int i=0; i<3; i++) for(int j=0; j<3-i; j++) {
                    if(grupoActual.obtener(j).getPuntos() < grupoActual.obtener(j+1).getPuntos() || (grupoActual.obtener(j).getPuntos() == grupoActual.obtener(j+1).getPuntos() && grupoActual.obtener(j).getGD() < grupoActual.obtener(j+1).getGD())) {
                        Equipo temp = grupoActual.obtener(j); grupoActual.obtener(j) = grupoActual.obtener(j+1); grupoActual.obtener(j+1) = temp;
                    }
                }
            llaves.registrarPosiciones(grupoActual, letras[k]);

            std::ofstream archivoSimulado(rutaBase + "/Grupo_" + letras[k] + "_Simulado.csv");
            if(archivoSimulado.is_open()) {
                archivoSimulado << "TABLA;PTS;PG;PE;PP;GF;GC;GD\n";
                for(int i=0; i<4; i++) grupoActual.obtener(i).exportarSimulado(archivoSimulado);
                archivoSimulado << "\nLOG TECNICO (LAMBDA);VISITA;LAM_L;LAM_V;G_L;G_V\n";
                for(int i=0; i<logTecnico.getTamano(); i++) { RegistroPoisson& p = logTecnico.obtener(i); archivoSimulado << p.local << ";" << p.visita << ";" << p.lambdaL << ";" << p.lambdaV << ";" << p.golesL << ";" << p.golesV << "\n"; }
                archivoSimulado.close();
            }
        }
    }

    Equipo simularPartidoEliminatorio(const char* fase, const char* id, Equipo eqA, Equipo eqB, Lista<RegistroHistorial>& historial) {
        float lambdaA = MU * std::pow((eqA.getPromedioGF()/MU), ALFA) * std::pow((eqB.getPromedioGC()/MU), BETA);
        float lambdaB = MU * std::pow((eqB.getPromedioGF()/MU), ALFA) * std::pow((eqA.getPromedioGC()/MU), BETA);
        std::poisson_distribution<int> distribucionA(lambdaA), distribucionB(lambdaB);
        int golesA = distribucionA(randomizador), golesB = distribucionB(randomizador);

        Equipo ganador;
        if (golesA > golesB) ganador = eqA;
        else if (golesB > golesA) ganador = eqB;
        else ganador = (eqA.getRankingInt() < eqB.getRankingInt() ? eqA : eqB);

        RegistroHistorial rh; rh.configurar(fase, id, eqA.getNombre(), eqB.getNombre(), ganador.getNombre(), golesA, golesB); historial.agregar(rh);
        std::cout << "| " << fase << " | " << id << " | " << eqA.getNombre() << " vs " << eqB.getNombre() << " |  " << golesA << " - " << golesB << "  | " << ganador.getNombre() << " |\n";
        return ganador;
    }
};

// ======================================================================================
// FUNCION QUE EJECUTA
// ======================================================================================

int main() {
    std::string entornoTrabajo = "/Users/zahir_llerena/My Drive/UdeA/Semestre_3/Informatica_II/Informatica/Code_Desafio_2/Code_D2";
    Lista<Equipo> seleccionMaestra; GestorArchivos gestorIO; ClasificadorLlaves arbitro; MotorMundial fifa2026; Lista<RegistroHistorial> baseDatosPartidos;

    std::cout << ">>> INICIANDO PLATAFORMA UdeAWorldCup 2026 <<<\n";

    // CORRECCIÓN: Se indica explícitamente saltar 2 líneas para el archivo maestro original
    if (gestorIO.leerArchivo(entornoTrabajo + "/selecciones_clasificadas_mundial.csv", seleccionMaestra, 2)) {

        gestorIO.generarGruposBase(entornoTrabajo, seleccionMaestra);
        fifa2026.ejecutarFaseGrupos(entornoTrabajo, gestorIO, arbitro, baseDatosPartidos, seleccionMaestra);
        arbitro.seleccionarMejoresTerceros();

        std::cout << "\n==================================================================================\n";
        std::cout << "| Fase   | ID   | Encuentro                        | Marcador | Clasifica / Gana |\n";
        std::cout << "|--------|------|----------------------------------|----------|------------------|\n";

        Equipo ganador_R32_1 = fifa2026.simularPartidoEliminatorio("R32","M73",arbitro.segundos.obtener(0),arbitro.segundos.obtener(1),baseDatosPartidos);
        Equipo ganador_R32_2 = fifa2026.simularPartidoEliminatorio("R32","M74",arbitro.primeros.obtener(4),arbitro.buscarTercero("ABCDF"),baseDatosPartidos);
        Equipo ganador_R32_3 = fifa2026.simularPartidoEliminatorio("R32","M75",arbitro.primeros.obtener(5),arbitro.segundos.obtener(2),baseDatosPartidos);
        Equipo ganador_R32_4 = fifa2026.simularPartidoEliminatorio("R32","M76",arbitro.primeros.obtener(2),arbitro.segundos.obtener(5),baseDatosPartidos);
        Equipo ganador_R32_5 = fifa2026.simularPartidoEliminatorio("R32","M77",arbitro.primeros.obtener(8),arbitro.buscarTercero("CDFGH"),baseDatosPartidos);
        Equipo ganador_R32_6 = fifa2026.simularPartidoEliminatorio("R32","M78",arbitro.segundos.obtener(4),arbitro.segundos.obtener(8),baseDatosPartidos);
        Equipo ganador_R32_7 = fifa2026.simularPartidoEliminatorio("R32","M79",arbitro.primeros.obtener(0),arbitro.buscarTercero("CEFHI"),baseDatosPartidos);
        Equipo ganador_R32_8 = fifa2026.simularPartidoEliminatorio("R32","M80",arbitro.primeros.obtener(11),arbitro.buscarTercero("EHIJK"),baseDatosPartidos);
        Equipo ganador_R32_9 = fifa2026.simularPartidoEliminatorio("R32","M81",arbitro.primeros.obtener(3),arbitro.buscarTercero("BEFIJ"),baseDatosPartidos);
        Equipo ganador_R32_10 = fifa2026.simularPartidoEliminatorio("R32","M82",arbitro.primeros.obtener(6),arbitro.buscarTercero("AEHIJ"),baseDatosPartidos);
        Equipo ganador_R32_11 = fifa2026.simularPartidoEliminatorio("R32","M83",arbitro.segundos.obtener(10),arbitro.segundos.obtener(11),baseDatosPartidos);
        Equipo ganador_R32_12 = fifa2026.simularPartidoEliminatorio("R32","M84",arbitro.primeros.obtener(7),arbitro.segundos.obtener(9),baseDatosPartidos);
        Equipo ganador_R32_13 = fifa2026.simularPartidoEliminatorio("R32","M85",arbitro.primeros.obtener(1),arbitro.buscarTercero("EFGIJ"),baseDatosPartidos);
        Equipo ganador_R32_14 = fifa2026.simularPartidoEliminatorio("R32","M86",arbitro.primeros.obtener(9),arbitro.segundos.obtener(7),baseDatosPartidos);
        Equipo ganador_R32_15 = fifa2026.simularPartidoEliminatorio("R32","M87",arbitro.primeros.obtener(10),arbitro.buscarTercero("DEIJL"),baseDatosPartidos);
        Equipo ganador_R32_16 = fifa2026.simularPartidoEliminatorio("R32","M88",arbitro.segundos.obtener(3),arbitro.segundos.obtener(6),baseDatosPartidos);

        Equipo ganador_R16_1 = fifa2026.simularPartidoEliminatorio("R16","P89",ganador_R32_2,ganador_R32_5,baseDatosPartidos);
        Equipo ganador_R16_2 = fifa2026.simularPartidoEliminatorio("R16","P90",ganador_R32_1,ganador_R32_3,baseDatosPartidos);
        Equipo ganador_R16_3 = fifa2026.simularPartidoEliminatorio("R16","P91",ganador_R32_4,ganador_R32_6,baseDatosPartidos);
        Equipo ganador_R16_4 = fifa2026.simularPartidoEliminatorio("R16","P92",ganador_R32_7,ganador_R32_8,baseDatosPartidos);
        Equipo ganador_R16_5 = fifa2026.simularPartidoEliminatorio("R16","P93",ganador_R32_11,ganador_R32_12,baseDatosPartidos);
        Equipo ganador_R16_6 = fifa2026.simularPartidoEliminatorio("R16","P94",ganador_R32_9,ganador_R32_10,baseDatosPartidos);
        Equipo ganador_R16_7 = fifa2026.simularPartidoEliminatorio("R16","P95",ganador_R32_14,ganador_R32_16,baseDatosPartidos);
        Equipo ganador_R16_8 = fifa2026.simularPartidoEliminatorio("R16","P96",ganador_R32_13,ganador_R32_15,baseDatosPartidos);

        Equipo semifinalista_1 = fifa2026.simularPartidoEliminatorio("QF","QF1",ganador_R16_1,ganador_R16_2,baseDatosPartidos);
        Equipo semifinalista_2 = fifa2026.simularPartidoEliminatorio("QF","QF2",ganador_R16_3,ganador_R16_4,baseDatosPartidos);
        Equipo semifinalista_3 = fifa2026.simularPartidoEliminatorio("QF","QF3",ganador_R16_5,ganador_R16_6,baseDatosPartidos);
        Equipo semifinalista_4 = fifa2026.simularPartidoEliminatorio("QF","QF4",ganador_R16_7,ganador_R16_8,baseDatosPartidos);

        Equipo finalista_1 = fifa2026.simularPartidoEliminatorio("SF","SF1",semifinalista_1,semifinalista_2,baseDatosPartidos);
        Equipo finalista_2 = fifa2026.simularPartidoEliminatorio("SF","SF2",semifinalista_3,semifinalista_4,baseDatosPartidos);

        Equipo perdedor_SF1 = (finalista_1.esIgualA(semifinalista_1.getNombre())) ? semifinalista_2 : semifinalista_1;
        Equipo perdedor_SF2 = (finalista_2.esIgualA(semifinalista_3.getNombre())) ? semifinalista_4 : semifinalista_3;

        Equipo tercerPuesto = fifa2026.simularPartidoEliminatorio("Final","M103",perdedor_SF1,perdedor_SF2,baseDatosPartidos);
        Equipo campeonMundial = fifa2026.simularPartidoEliminatorio("Final","M104",finalista_1,finalista_2,baseDatosPartidos);
        Equipo subcampeon = (campeonMundial.esIgualA(finalista_1.getNombre())) ? finalista_2 : finalista_1;

        std::cout << "\n============================================\n|     PODIO FINAL - COPA MUNDIAL 2026      |\n============================================\n";
        std::cout << "| 1ero (Oro)   | " << campeonMundial.getNombre() << "\n| 2do (Plata)  | " << subcampeon.getNombre() << "\n| 3ero (Bronce)| " << tercerPuesto.getNombre() << "\n============================================\n";

        // Consolidación y Exportación
        std::ofstream h_f(entornoTrabajo + "/Resumen_Eliminatorias_2026.csv");
        if(h_f.is_open()){
            h_f << "Fase;ID;Local;Visita;G_L;G_V;Ganador\n";
            for(int i=0; i<baseDatosPartidos.getTamano(); i++) { RegistroHistorial& h = baseDatosPartidos.obtener(i); h_f << h.fase << ";" << h.id << ";" << h.local << ";" << h.visita << ";" << h.golesL << ";" << h.golesV << ";" << h.ganador << "\n"; }
            h_f.close();
        }

        for(int i=0; i<baseDatosPartidos.getTamano(); i++) {
            for(int j=0; j<seleccionMaestra.getTamano(); j++) {
                if(seleccionMaestra.obtener(j).esIgualA(baseDatosPartidos.obtener(i).local.c_str()) || seleccionMaestra.obtener(j).esIgualA(baseDatosPartidos.obtener(i).visita.c_str())) {
                    std::string f = baseDatosPartidos.obtener(i).fase;
                    if(f == "R32") seleccionMaestra.obtener(j).setEtapaFinal("Dieciseisavos", 7);
                    else if(f == "R16") seleccionMaestra.obtener(j).setEtapaFinal("Octavos", 6);
                    else if(f == "QF") seleccionMaestra.obtener(j).setEtapaFinal("Cuartos", 5);
                    else if(f == "SF") seleccionMaestra.obtener(j).setEtapaFinal("Semifinalistas", 4);
                }
            }
        }
        for(int j=0; j<seleccionMaestra.getTamano(); j++) {
            if(seleccionMaestra.obtener(j).esIgualA(tercerPuesto.getNombre())) seleccionMaestra.obtener(j).setEtapaFinal("Tercer Puesto", 3);
            if(seleccionMaestra.obtener(j).esIgualA(subcampeon.getNombre())) seleccionMaestra.obtener(j).setEtapaFinal("Subcampeon", 2);
            if(seleccionMaestra.obtener(j).esIgualA(campeonMundial.getNombre())) seleccionMaestra.obtener(j).setEtapaFinal("Campeon", 1);
        }

        for(int i=0; i<seleccionMaestra.getTamano()-1; i++) for(int j=0; j<seleccionMaestra.getTamano()-i-1; j++) {
                if(seleccionMaestra.obtener(j).getPeso() > seleccionMaestra.obtener(j+1).getPeso() || (seleccionMaestra.obtener(j).getPeso() == seleccionMaestra.obtener(j+1).getPeso() && seleccionMaestra.obtener(j).getPuntos() < seleccionMaestra.obtener(j+1).getPuntos())) {
                    Equipo temporal = seleccionMaestra.obtener(j); seleccionMaestra.obtener(j) = seleccionMaestra.obtener(j+1); seleccionMaestra.obtener(j+1) = temporal;
                }
            }

        std::ofstream r_f(entornoTrabajo + "/Posiciones_Finales_Mundial_2026.csv");
        if(r_f.is_open()){
            r_f << "Ranking;Pais;Etapa;Pts;GD;Ranking_FIFA\n";
            for(int i=0; i<seleccionMaestra.getTamano(); i++) { r_f << i+1 << ";"; seleccionMaestra.obtener(i).exportarEscalafon(r_f); }
            r_f.close();
            std::cout << "[EXITO] Ranking Mundial actualizado correctamente con Puntos y GD.\n";
        }
    }

    Lista<int>::mostrarAuditoria();
    return 0;
}

// ======================================================================================
// Muchas gracias senor profesor por la oportunidad de realizar este trabajo.
// Desafortunadamente no pudimos darles respuesta a todos los item que solicito en la guia
// Sin embargo, dimos respuestas a los item que en nuestra opinion son los mas importantes
// en la simulacion. Agradecemos el espacio brindado.
// ======================================================================================





