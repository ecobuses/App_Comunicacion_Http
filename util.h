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
#include <QNetworkInformation>
class util
{
public:
    util();
    int  postHttp(QJsonArray &);
    // ------------------- Guardar datos de telemetría que no se pueden enviar --------------- //
        // Chequear si hay internet.
     bool determinarConexionAInternet();
        // Escribir datos en el archivo si no hay internet
        // Cuando haya datos e internet, leer enviar fila, antes un envío exitoso borrar si el envío falla volver a leer la misma fila
        // Cuando se termine el proceso de enviar l

    // ---------------------------------------------------------------------------------------//
    QString getPathId(){return this->pathId;}
private:
    const  QString url="http://localhost:8080/magnitud";
};

#endif // UTIL_H
