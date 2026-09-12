#ifndef MANIPUAR_ARCHIVOS_H
#define MANIPUAR_ARCHIVOS_H
#include <QDataStream>
#include <QTextStream>
#include <QString>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
class Manipular_Archivos
{
public:
    Manipular_Archivos();
    // -------------- archivo ID ---------------//
    bool guardarIdArchivo(int id);
    int  leerIdArchivo();
    QString getPathId(){return this->pathId;}
    // ----------------------------------------//

    //--------------- archivo excel telemetrias fallidas ------------//
    bool guardarDatoExcel(QJsonObject *obj,int t);
    QJsonObject leerDatoExcel(int t);
    void determinarCabeceras(QStringList *cabeceras,int t);
    void determinarPath(int, QString*);
    //---------------------------------------------------------------//
private:
    //------------------------------------------------- Funciones ----------------------------------------------//
    void deStringAQJSonbject(QJsonObject* objeto,const QString linea,int t);
    void escribirCabeceras(QTextStream*,QStringList);
    void escribirExcel(QTextStream *, QJsonObject *objeto ,int t);
    //----------------------------------------------------------------------------------------------------------//
    //------------------------------------------------- Variables ----------------------------------------------//
    // Variable con la ruta del archvio donde se escribe el ID de la batería del ecobus en el sistema.
   const  QString pathId = "/home/pi/App_Comunicacion_Http/archivos_configuracion/valorId.txt";
    // Variable con la ruta del excel con las entradas de telemetria.
   const QString pathExcelTelemetria = "/home/pi/App_Comunicacion_Http/archivos_configuracion/telemetrias.csv";
    // Variable con la ruta del excel con las entradas de carga/descarga
   const QString pathExcelDescargaCarga = "/home/pi/App_Comunicacion_Http/archivos_configuracion/ciclo_descarga_carga.csv";
   // Variable con la ruta del excel con las entradas de gps
   const QString pathExcelGps = "/home/pi/App_Comunicacion_Http/archivos_configuracion/gps.csv";
   //Cabeceras del excell telemetria
   const QStringList cabecerasTelemetria = {"Fecha","Carga","Corriente","Tensión","Temperatura","idBateria"};
   //Cabeceras del excell descarga/carga
   const QStringList cabecerasDescargaCarga = {"Fecha ingreso",
                                               "Fecha inicio descarga",
                                               "Fecha fin descarga",
                                               "Descarga",
                                               "Fecha inicio carga",
                                               "Fecha fin carga",
                                               "Carga",
                                               "Diferencia descarga",
                                               "Diferencia carga",
                                               "idBateria"};
   //Cabeceras del excell Gps
   const QStringList cabecerasGps = {"Fecha","Latitud","Longitud","Velocidad","Sentido","idBateria"};
   //Claves del objeto telemetria -> Implementación futura
   const QStringList clavesTelemetria = {"fecha","carga","corriente","voltaje","voltaje","temperatura","idBateria"};
   //--------------------------------------------------------------------------------------------------------------//
};

#endif // MANIPUAR_ARCHIVOS_H
