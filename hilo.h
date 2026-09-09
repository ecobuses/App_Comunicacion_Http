#ifndef HILO_H
#define HILO_H
#include <QThread>
#include <QTimeZone>
#include <QObject>
#include "util.h"
#include "manipuar_archivos.h"
#include "servidor.h"
class hilo:public QThread
{
    Q_OBJECT
public:
    hilo();
    void setId(int id){this->idBateria=id;}
    int getId(){return this->idBateria;}
    //Los datos de telemetria que quedaron guardados en el excel se tienen que enviar.
    void enviarDatosDelExcel(util* u, Manipular_Archivos *mp);
private slots:
    void procesarTramasTelemetria();
    void procesarTramasCargaDescarga();
private:
    //Variables
    int idBateria;
    bool servidorAlive = false;
    util variableUtil;
    Manipular_Archivos mp;
    //Funciones
    void validacionDeId(int* id,int * idBateria);
    QString ulrServidor = "http://100.103.238.52:8080";
    //Este servidor recibe la telemetría.
    Servidor server;
    //Este servidro recibe el ciclo de carga/descarga.
    Servidor servidor2;
protected:
    void run();
};

#endif // HILO_H
