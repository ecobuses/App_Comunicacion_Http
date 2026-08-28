#ifndef UTIL_H
#define UTIL_H
#include<QString>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QEventLoop>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QTimer>
#include <QFile>
#include <QTextStream>
#include <QSettings>
class util
{
public:
    util();
    int  postHttp(QJsonArray &);
    // -------------- archivo ID ---------------//
    bool guardarIdArchivo(int id);
    int  leerIdArchivo();
    // ----------------------------------------//
    // ------------------- Guardar datos de telemetría que no se pueden enviar --------------- //
        // Chequear si hay internet.
        // Escribir datos en el archivo si no hay internet
        // Cuando haya datos e internet, leer enviar fila, antes un envío exitoso borrar si el envío falla volver a leer la misma fila
        // Cuando se termine el proceso de enviar l

    // ---------------------------------------------------------------------------------------//
    QString getPathId(){return this->pathId;}
private:
    const  QString url="http://localhost:8080/magnitud";
    const  QString pathId = "/home/pi/aplicaciones/App_Comunicacion_Http/valorId.txt";
};

#endif // UTIL_H
