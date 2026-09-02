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
bool Manipular_Archivos::guardarDatoTelelmetria(QJsonObject objeto){
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
        salida <<"fecha"<<sep<<"Carga"<<sep<<"Corriente"<<sep<<"Tensión"<<sep<<"Temperatura"<<sep<<"idBateria";
    }
    for(auto it = objeto.begin(); it != objeto.end(); it++){
        //Si escribo la última columna no quiero que ponga un separador
        if(it.key() == "idBateria"){
            salida << it.value().toVariant().toString();
        //Si todavía no es la última columna que lo haga.
        }else{
         salida << it.value().toVariant().toString()<<sep;
        }
    }
    //Escribo para que pase a la fila siguiente.
    salida<<"\n";
    //Cierro el archivo
    archivo.close();
    return true;
}
QJsonObject Manipular_Archivos::leerDatoTelemetria(){
    QJsonObject leeido;
    return leeido;
}