#include "manipuar_archivos.h"

Manipular_Archivos::Manipular_Archivos() {}
bool Manipular_Archivos::guardarIdArchivo(int id){
    QFile file(this->getPathId());
    if(file.open(QIODevice::WriteOnly  | QIODevice::Text)){
        QTextStream in (&file);
        in<<id;
        return true;
    }
    return false;
}
int Manipular_Archivos::leerIdArchivo(){
    QFile file(this->getPathId());
    int id;
    if(file.open(QIODevice::ReadOnly  | QIODevice::Text)){
        QTextStream in (&file);
        in>>id;
        if(id > 0){
            return id;
        }
    }
    return -1;
}
bool Manipular_Archivos::guardarDatoTelelmetria(QJsonObject* objeto, int t){
    //Obtengo el archivo.
    QFile archivo(pathExcelTelemetria);
    //Abro el archivo
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Append)) {
        // Si no puedo abrirlo, no puedo escribir.
        qDebug()<<"Manipular_Arcivos/guardarDatoTelemetria - No pudo abrir el excel para escribir las telemetrias";
        return false;
    }
    // Me sirve para escribir y leer en el archivo de manera más fácil.
    QTextStream salida(&archivo);
    // Me sirve para separa los campos.
    QString sep = ",";
    if(archivo.size() == 0){
        //Defino el encabezado del excel
        //salida <<"Fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria" <<"\n";
        escribirCabeceras(&salida,cabeceras);
    }
    //Esto lo tengo que chequear. Me tengo que fijar que esas key existan
    if(!objeto->isEmpty() && objeto->value("carga").toVariant().toString() !=""){
        salida<<objeto->value("fecha").toVariant().toString()<<sep;
        qDebug()<<"Que se guarda en el Excel"<<objeto->value("carga").toVariant().toString();
        salida<<objeto->value("carga").toVariant().toString()<<sep;
        salida<<objeto->value("corriente").toVariant().toString()<<sep;
        salida<<objeto->value("voltaje").toVariant().toString()<<sep;
        salida<<objeto->value("temperatura").toVariant().toString()<<sep;
        salida<<objeto->value("idBateria").toVariant().toString()<<"\n";
    }

    //Cierro el archivo
    archivo.close();
    return true;
}
//Lee una línea del Excel y la borrar
QJsonObject Manipular_Archivos::leerDatoTelemetria(int t){
    QJsonObject leeido;
    QFile archivo;
    QStringList cabeceras;
    //Obtengo una variable del archivo

    //Abro el archivo
    if(!archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"No pude abrir el archivo excel para leer los datos de telemetria guardados";
        return leeido;
    }
    QTextStream entrada(&archivo);
    //Leo la primera linea
    if(!entrada.atEnd()){
        //Leo las cabeceras.
        entrada.readLine();
    }
    QString primeraLinea = entrada.readLine();
    //Si la primera linea no tiene nada
    if(primeraLinea.isNull()){
        qDebug()<<"El archivo ya está vacio.";
        archivo.close();
        return leeido;
    }
    //Convierte la primera linea de datos en un QJsonObject
    this->deStringAQJSonbject(&leeido,primeraLinea);
    //Piso el contenido del archivo sin la primera linea
    QString restoDelArchivo = entrada.readAll();
    archivo.close();
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug()<<"No pude abrir el archivo excel para re-escribir los datos de telemetria guardados";
        return leeido;
    }
    QString sep = ",";
    QTextStream salida(&archivo);
    //salida<<"Fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria" <<"\n";
    escribirCabeceras(&salida,cabeceras);
    salida<<restoDelArchivo;
    archivo.close();
    qDebug()<<"que recupera del excel "<<leeido.value("idBateria");
    return leeido;
}
void Manipular_Archivos::deStringAQJSonbject(QJsonObject* objeto,const QString linea){
    QStringList columnas = linea.split(",");
    objeto->insert("fecha",columnas.value(0).trimmed());
    objeto->insert("carga",columnas.value(1).trimmed().toDouble());
    objeto->insert("corriente",columnas.value(2).trimmed().toDouble());
    objeto->insert("voltaje",columnas.value(3).trimmed().toDouble());
    objeto->insert("temperatura",columnas.value(4).trimmed().toInt());
    objeto->insert("idBateria",columnas.value(5).trimmed().toInt());
}
void Manipular_Archivos::escribirCabeceras(QTextStream *stream,QStringList cabeceras){
    QString sep = ",";
    for(const QString &c:cabeceras){
        *stream<<c<<sep;
    }
    *stream<<"\n";
}

void Manipular_Archivos::determinarCabeceras(QString *archivo,int t){
    switch(t){
    case 0:{
        archivo.setFileName(pathExcelTelemetria);
        cabeceras = cabecerasTelemetria;
        break;
    }
    case 1: {
        archivo.setFileName(pathExcelDescargaCarga);
        cabeceras = cabecerasDescargaCarga;
    }
    case 2: {
        archivo.setFileName(pathExcelGps);
        cabeceras = cabecerasGps;
    }
    }
}
