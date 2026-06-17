#include "servidor.h"

Servidor::Servidor(QObject *parent)
    : QObject(parent)
{
    server = new QLocalServer(this);
}

bool Servidor::iniciar(const QString &nombrePipe)
{
    QLocalServer::removeServer(nombrePipe);

    if(!server->listen(nombrePipe)){
        qDebug() << "Error servidor:"
                 << server->errorString();
        return false;
    }

    connect(server,
            &QLocalServer::newConnection,
            this,
            &Servidor::nuevaConexion);

    qDebug() << "Servidor escuchando";

    return true;
}

void Servidor::nuevaConexion()
{
    QLocalSocket *socket =
        server->nextPendingConnection();

    if(!socket){
        return;
    }

    qDebug() << "Cliente conectado";

    connect(socket,
            &QLocalSocket::readyRead,
            this,
            [this, socket](){

                leerDatos(socket);

            });

    connect(socket,
            &QLocalSocket::disconnected,
            this,
            [this, socket](){

                clienteDesconectado(socket);

            });
}

void Servidor::leerDatos(QLocalSocket *socket)
{
    QByteArray data = socket->readAll();

    QJsonParseError error;

    QJsonDocument doc =
        QJsonDocument::fromJson(data, &error);

    if(error.error != QJsonParseError::NoError){

        qDebug() << "JSON inválido";
        return;
    }

    datos = doc.object();
    this->setDatos(doc.object());
    qDebug()<<"Carga: "<<datos["carga"];
    qDebug() << "\n--- MENSAJE RECIBIDO ---";

    qDebug() << "Voltaje:"
             << datos["voltaje"].toDouble();

    qDebug() << "Temperatura:"
             << datos["temperatura"].toDouble();

    qDebug() << "Corriente:"
             << datos["corriente"].toDouble();
    qDebug() << "Carga:"
             << datos["carga"].toDouble();

    QJsonObject respuesta;

    respuesta["status"] = "OK";

    QJsonDocument respuestaDoc(respuesta);

    socket->write(respuestaDoc.toJson());

    socket->flush();
}

void Servidor::clienteDesconectado(QLocalSocket *socket)
{
    qDebug() << "Cliente desconectado";

    socket->deleteLater();
}
