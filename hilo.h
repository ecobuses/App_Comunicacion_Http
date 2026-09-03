#ifndef HILO_H
#define HILO_H
#include <QThread>
#include <QTimeZone>
#include "util.h"
#include "manipuar_archivos.h"
#include "servidor.h"
class hilo:public QThread
{
public:
    hilo();
    void setId(int id){this->id=id;}
    int getId(){return this->id;}
    //Los datos de telemetria que quedaron guardados en el excel se tienen que enviar.
    void enviarDatosDelExcel(util* u, Manipular_Archivos *mp);
private:
    //Variables
    int id;
    util variableUtil;
    Manipular_Archivos mp;
    Servidor server;
    //Funciones
    void validacionDeId(int* id,int * idBateria);
protected:
    void run();
};

#endif // HILO_H
