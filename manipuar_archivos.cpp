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
bool Manipular_Archivos::guardarDatoTelelmetria(QJsonObject* objeto){
    //Obtengo el archivo.
    QFile archivo(pathExcelTelemetria);
    //Abro el archivo
    if(!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
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
        salida <<"Fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria" <<"\n";
    }
    //Esto lo tengo que chequear. Me tengo que fijar que esas key existan
    salida<<objeto->value("fecha").toString()<<sep;
    salida<<objeto->value("carga").toString()<<sep;
    salida<<objeto->value("corriente").toString()<<sep;
    salida<<objeto->value("voltaje").toString()<<sep;
    salida<<objeto->value("temperatura").toString()<<sep;
    salida<<objeto->value("idBateria").toString();
    //Escribo para que pase a la fila siguiente.
    salida<<"\n";
    //Cierro el archivo
    archivo.close();
    return true;
}
//Lee una línea del Excel y la borrar
QJsonObject Manipular_Archivos::leerDatoTelemetria(){
    QJsonObject leeido;
    //Obtengo una variable del archivo
    QFile archivo(pathExcelTelemetria);
    //Abro el archivo
    if(!archivo.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"No pude abrir el archivo excel para leer los datos de telemetria guardados";
        return leeido;
    }
    QTextStream entrada(&archivo);
    //Leo la primera linea
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
    QTextStream salida(&archivo);
    salida<<restoDelArchivo;
    archivo.close();
    return leeido;
}
void Manipular_Archivos::deStringAQJSonbject(QJsonObject* objeto,const QString linea){
    QStringList columnas = linea.split(",");
    objeto->insert("fecha",columnas.value(0));
    objeto->insert("carga",columnas.value(1));
    objeto->insert("corriente",columnas.value(2));
    objeto->insert("voltaje",columnas.value(3));
    objeto->insert("temperatura",columnas.value(4));
    objeto->insert("idBateria",columnas.value(5));
}
