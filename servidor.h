#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

class Servidor : public QObject
{
    Q_OBJECT

public:
    explicit Servidor(QObject *parent = nullptr);

    bool iniciar(const QString &nombrePipe);
    void setDatos(QJsonObject d){this->datos = d;}
    QJsonObject getDatos(){return this->datos;}
private slots:
    void nuevaConexion();

private:
    void leerDatos(QLocalSocket *socket);
    void clienteDesconectado(QLocalSocket *socket);
    QJsonObject datos;
    QLocalServer *server;
};

#endif