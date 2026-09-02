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
bool determinarConexionAInternet(){
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