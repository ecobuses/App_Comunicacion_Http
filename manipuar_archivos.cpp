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
bool Manipular_Archivos::guardarDatoTelelmetria(QJsonArray telemetrias){
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
        salida <<"fecha"<<sep<<"valor"<<sep<<"tipo"<<sep<<"idBateria";
    }
    //Recorro el array de QJson
    //Entonces escribo cada dato en una fila, una entrada de telemetria corresponde 4 filas del excel.
    for(const QJsonValue &valor : telemetrias){
        //Convierto el valor del array en un objeto QJsonObject
        QJsonObject objeto = valor.toObject();
        //Este objeto tiene
            // un id propio de la entrada que es nulo, no veo necesidad de guardar esto
            // una fecha de cuando se realizo la carga, se guarda.
            // el valor, se guarda
            // el tipo se guarda, no hay necesidad de guardar el ID se determina en el servidor.
            // se guarda el id de la bateria a la que pertenecen estos valores.
        //Recorro el objeto QObjecJson para tomar sus valores
        for(auto it = objeto.begin(); it != objeto.end(); it++){
            if(it.value() == QJsonValue::Null){
                //Si el dato es nulo, salteo la iteración.
                continue;
            }
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
    }
    return true;
}
QJsonArray Manipular_Archivos::leerDatoTelemetria(){
    QJsonArray leeido;
    return leeido;
}