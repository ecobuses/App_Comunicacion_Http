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
    void enviarDatosDelExcel(util* u, Manipular_Archivos *mp,int t,QString url);
private slots:
    // int t: Es una variable que sirve para ver que tipo de archivo tiene que guardar o leer.
    void procesarTramas(Servidor *, const QString endUrl,int t);
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
    Servidor *server{nullptr};
    //Este servidro recibe el ciclo de carga/descarga.
    Servidor *servidor2{nullptr};
    //Este servidor recibe los datos del Gps.
    Servidor *servidor3{nullptr};
protected:
    void run();
};

#endif // HILO_H
