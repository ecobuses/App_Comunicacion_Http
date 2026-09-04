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
#include <QTimeZone>
#include <QJsonArray>
class util
{
public:
    util();
    //Envía un petición POST con los datos de telemetria.
    //También devuelve el ID de la batería a la que corresponden esos datos.
    int  postHttp(QJsonArray &);
    // ------------------- Guardar datos de telemetría que no se pueden enviar --------------- //
        // Chequear si hay internet.
     bool determinarConexionAInternet();
        // Escribir datos en el archivo si no hay internet
        // Cuando haya datos e internet, leer enviar fila, antes un envío exitoso borrar si el envío falla volver a leer la misma fila
        // Cuando se termine el proceso de enviar l

    // ---------------------------------------------------------------------------------------//
    //armarJSonArray - Crea objetos QJsonObject y los mete en un QJsonArray para enviar al servidor
     QJsonArray armarQJsonArray(QJsonObject* datos);
    //devuelve la fecha actual en formato Strign
     QString fechaActual();
private:
    const  QString url="http://localhost:8080/magnitud";
    const  QString pathId = "/home/pi/App_Comunicacion_Http/archivos_configuracion/valorId.txt";
};

#endif // UTIL_H
