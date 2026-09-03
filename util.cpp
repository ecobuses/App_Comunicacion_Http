#include "util.h"

util::util() {

}
int util::postHttp(QJsonArray &json){


    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req(QUrl(QString("http://100.103.238.52:8080/magnitud") ) );
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = mgr.post(req,QJsonDocument(json).toJson());
    QTimer *timer = new QTimer();
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, reply, [reply]() {
        qDebug()<<"Aborto";
        reply->abort();
    });
    timer->start(1000);
    eventLoop.exec(); // blocks stack until "finished()" has been called
    if (reply->error() == QNetworkReply::NoError) {
        //success
        int v = reply->readAll().toInt();
        qDebug() << "Success" <<v;
        delete reply;
        delete timer;
        return v;
    }
    else {
        //failure
        qDebug() << "Failure" <<reply->errorString();
        delete reply;
        delete timer;
        return -1;
    }
}

// Utiliza la librería QNetworkInformation que viene a partir de QT.6 para determinar si hay conexión.
bool util::determinarConexionAInternet(){
    if(QNetworkInformation::loadDefaultBackend()){
        QNetworkInformation *info = QNetworkInformation::instance();

        if(info && info->reachability() == QNetworkInformation::Reachability::Online){
            qDebug()<< "Archivo util/determinarConexionAInternet - Hay internet";
            return true;
        }else
        {
            qDebug()<<"Archivo util/determinarConexionAInternet - No hay internet";
            return false;
        }
    }
    qDebug()<< "Archivo util/determinarConexionAInternet - No se pudo determinar";
    return true;
}
//armarJSonArray - Crea objetos QJsonObject y los mete en un QJsonArray para enviar al servidor
QJsonArray util::armarQJsonArray(QJsonObject* datos){
    //Guarda el tipo del dato
    QJsonObject objTipo;
    QJsonObject objCarga;
    //Guarda todos los valores
    QJsonArray jsonArray;
    int idBateria = datos->value("idBateria").toInt();
    QString formato = datos->value("fecha").toString();
    qDebug()<<"La hora que toma el sistema es: "<< formato;
    objCarga.insert("id",QJsonValue::Null);
    objCarga.insert("fecha",formato);
    objCarga.insert("valor",datos->value("carga").toDouble());
    objTipo.insert("id",QJsonValue::Null);
    objTipo.insert("tipo","CARGA");
    objCarga.insert("idBateria",idBateria);
    objCarga.insert("tipo",objTipo);
    jsonArray.append(objCarga);
    QJsonObject objCorriente;
    objCorriente.insert("id",QJsonValue::Null);
    objCorriente.insert("fecha",formato);
    objCorriente.insert("valor",datos->value("corriente").toDouble());
    objTipo.insert("id",QJsonValue::Null);
    objTipo.insert("tipo","CORRIENTE");
    objCorriente.insert("tipo",objTipo);
    objCorriente.insert("idBateria",idBateria);
    jsonArray.append(objCorriente);
    QJsonObject objTension;
    objTension.insert("id",QJsonValue::Null);
    objTension.insert("fecha",formato);
    objTension.insert("valor",datos->value("voltaje").toDouble());
    objTipo.insert("id",QJsonValue::Null);
    objTipo.insert("tipo","TENSION");
    objTension.insert("tipo",objTipo);
    objTension.insert("idBateria",idBateria);
    jsonArray.append(objTension);
    QJsonObject objTemperatura;
    objTemperatura.insert("id",QJsonValue::Null);
    objTemperatura.insert("fecha",formato);
    objTemperatura.insert("valor",datos->value("temperatura").toDouble());
    objTipo.insert("id",QJsonValue::Null);
    objTipo.insert("tipo","TEMPERATURA");
    objTemperatura.insert("tipo",objTipo);
    objTemperatura.insert("idBateria",idBateria);
    jsonArray.append(objTemperatura);
    return jsonArray;
}
//devuelve la fecha actual en formato Strign
QString util::fechaActual(){
    QDateTime horaUtc = QDateTime::currentDateTimeUtc();
    // 2. Crear la zona horaria UTC-3 (3 horas * 3600 segundos = 10800)
    QTimeZone zonaMenosTres(-10800);
    // 3. Convertir la hora a la nueva zona
    QDateTime horaLocal = horaUtc.toTimeZone(zonaMenosTres);
    // Formatear a: yyyy-MM-dd HH:mm:ss
    return horaLocal.toString("yyyy-MM-dd HH:mm:ss");
}
