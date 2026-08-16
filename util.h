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
    bool guardarIdArchivo(int id);
    int  leerIdArchivo();
    QString getPathId(){return this->pathId;}
private:
    const  QString url="http://localhost:8080/magnitud";
    const  QString pathId = "/home/pi/aplicaciones/App_Comunicacion_Http/valorId.txt";
};

#endif // UTIL_H
