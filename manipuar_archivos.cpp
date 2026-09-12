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
bool Manipular_Archivos::guardarDatoExcel(QJsonObject* objeto, int t){
    QString path;
    determinarPath(t,&path);
    //Obtengo el archivo.
    QFile archivo(path);
    QStringList cabeceras;
    //Abro el archivo
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Append)) {
        // Si no puedo abrirlo, no puedo escribir.
        qDebug()<<"Manipular_Arcivos/guardarDatoTelemetria - No pudo abrir el excel para escribir las telemetrias";
        return false;
    }
    // Me sirve para escribir y leer en el archivo de manera más fácil.
    determinarCabeceras(&archivo,&cabeceras,t);
    QTextStream salida(&archivo);
    // Me sirve para separa los campos.
    QString sep = ",";
    if(archivo.size() == 0){
        //Defino el encabezado del excel
        //salida <<"Fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria" <<"\n";
        escribirCabeceras(&salida,cabeceras);
    }
    escribirExcel(&salida,objeto,t);
    //Cierro el archivo
    archivo.close();
    return true;
}
//Lee una línea del Excel y la borrar
QJsonObject Manipular_Archivos::leerDatoExcel(int t){
    QJsonObject leeido;
    QString path;
    determinarPath(t,&path);
    QFile archivo(path);
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
    this->deStringAQJSonbject(&leeido,primeraLinea,t);
    //Piso el contenido del archivo sin la primera linea
    QString restoDelArchivo = entrada.readAll();
    archivo.close();
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug()<<"No pude abrir el archivo excel para re-escribir los datos guardados";
        return leeido;
    }
    QString sep = ",";
    QTextStream salida(&archivo);
    determinarCabeceras(&archivo,&cabeceras,t);
    //salida<<"Fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria" <<"\n";
    escribirCabeceras(&salida,cabeceras);
    salida<<restoDelArchivo;
    archivo.close();
    qDebug()<<"que recupera del excel "<<leeido.value("idBateria");
    return leeido;
}
void Manipular_Archivos::deStringAQJSonbject(QJsonObject* objeto,const QString linea, int t){
    switch(t){
        case 0: {
            QStringList columnas = linea.split(",");
            objeto->insert("fecha",columnas.value(0).trimmed());
            objeto->insert("carga",columnas.value(1).trimmed().toDouble());
            objeto->insert("corriente",columnas.value(2).trimmed().toDouble());
            objeto->insert("voltaje",columnas.value(3).trimmed().toDouble());
            objeto->insert("temperatura",columnas.value(4).trimmed().toInt());
            objeto->insert("idBateria",columnas.value(5).trimmed().toInt());
            break;
        }
        case 1: {
            QStringList columnas = linea.split(",");
            objeto->insert("fechaEntrada",columnas.value(0).trimmed());
            objeto->insert("inicioDescarga",columnas.value(1).trimmed());
            objeto->insert("finDescarga",columnas.value(2).trimmed());
            objeto->insert("descarga",columnas.value(3).trimmed().toDouble());
            objeto->insert("inicioCarga",columnas.value(4).trimmed());
            objeto->insert("finCarga",columnas.value(5).trimmed());
            objeto->insert("carga",columnas.value(6).trimmed().toDouble());
            objeto->insert("diferenciaDescarga",columnas.value(7).trimmed().toDouble());
            objeto->insert("difernciaCarga",columnas.value(8).trimmed().toDouble());
            objeto->insert("idBateria",columnas.value(9).trimmed().toInt());
            break;
        }
        case 2:{
            QStringList columnas = linea.split(",");
            objeto->insert("fecha",columnas.value(0).trimmed());
            objeto->insert("latitud",columnas.value(1).trimmed().toDouble());
            objeto->insert("longitud",columnas.value(2).trimmed().toDouble());
            objeto->insert("velocidad",columnas.value(3).trimmed().toDouble());
            objeto->insert("sentido",columnas.value(4).trimmed().toDouble());
            objeto->insert("idBateria",columnas.value(5).trimmed().toInt());
            break;
        }
    }
}
void Manipular_Archivos::escribirCabeceras(QTextStream *stream,QStringList cabeceras){
    QString sep = ",";
    for(const QString &c:cabeceras){
        *stream<<c<<sep;
    }
    *stream<<"\n";
}

void Manipular_Archivos::determinarCabeceras(QFile*archivo,QStringList *cabeceras,int t){
    switch(t){
    case 0:{
        archivo->setFileName(pathExcelTelemetria);
        *cabeceras = cabecerasTelemetria;
        break;
    }
    case 1: {
        archivo->setFileName(pathExcelDescargaCarga);
        *cabeceras = cabecerasDescargaCarga;
    }
    case 2: {
        archivo->setFileName(pathExcelGps);
        *cabeceras = cabecerasGps;
    }
    }
}
void Manipular_Archivos::escribirExcel(QTextStream *salida,QJsonObject *objeto, int t){
    //Esto lo tengo que chequear. Me tengo que fijar que esas key existan
    QString sep=",";
    switch(t){
        case 0:{
            if(!objeto->isEmpty() && objeto->value("carga").toVariant().toString() !=""){
                *salida<<objeto->value("fecha").toVariant().toString()<<sep;
                qDebug()<<"Que se guarda en el Excel"<<objeto->value("carga").toVariant().toString();
                *salida<<objeto->value("carga").toVariant().toString()<<sep;
                *salida<<objeto->value("corriente").toVariant().toString()<<sep;
                *salida<<objeto->value("voltaje").toVariant().toString()<<sep;
                *salida<<objeto->value("temperatura").toVariant().toString()<<sep;
                *salida<<objeto->value("idBateria").toVariant().toString()<<"\n";
            }
            break;
        }
        case 1:{
            if(!objeto->isEmpty() && objeto->value("latitud").toVariant().toString() !=""){
                *salida<<objeto->value("fechaEntrada").toVariant().toString()<<sep;
                *salida<<objeto->value("inicioDescarga").toVariant().toString()<<sep;
                *salida<<objeto->value("finDescarga").toVariant().toString()<<sep;
                *salida<<objeto->value("descarga").toVariant().toString()<<sep;
                *salida<<objeto->value("inicioCarga").toVariant().toString()<<sep;
                *salida<<objeto->value("finCarga").toVariant().toString()<<sep;
                *salida<<objeto->value("carga").toVariant().toString()<<sep;
                *salida<<objeto->value("diferenciaDescarga").toVariant().toString()<<sep;
                *salida<<objeto->value("diferenciaCarga").toVariant().toString()<<sep;
                *salida<<objeto->value("idBateria").toVariant().toString()<<"\n";
            }
            break;
        }
        case 2:{
            if(!objeto->isEmpty() && objeto->value("latitud").toVariant().toString() !=""){
                *salida<<objeto->value("fecha").toVariant().toString()<<sep;
                *salida<<objeto->value("latitud").toVariant().toString()<<sep;
                *salida<<objeto->value("longitud").toVariant().toString()<<sep;
                *salida<<objeto->value("velocidad").toVariant().toString()<<sep;
                *salida<<objeto->value("sentido").toVariant().toString()<<sep;
                *salida<<objeto->value("idBateria").toVariant().toString()<<"\n";
            }
            break;
        }
    }
}
void Manipular_Archivos::determinarPath(int t, QString *path){
    switch(t){
    case 0:{
        *path =pathExcelTelemetria;
        break;
    }
    case 1:{
       *path=pathExcelDescargaCarga;
        break;
    }
    case 2:{
        *path=pathExcelGps;
        break;
    }
    }
}