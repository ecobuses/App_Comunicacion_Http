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
    bool guardarDatoTelelmetria(QJsonObject obj);
    QJsonObject leerDatoTelemetria();
    //---------------------------------------------------------------//
private:
    // Variable con la ruta del archvio donde se escribe el ID de la batería del ecobus en el sistema.
   const  QString pathId = "/home/pi/App_Comunicacion_Http/archivos_configuracion/valorId.txt";
    // Variable con la ruta del excel con las entradas.
   const QString pathExcelTelemetria = "home/pi/App_Comunicacion_Http/archivos_configuracion/telemetrias.csv";
};

#endif // MANIPUAR_ARCHIVOS_H
