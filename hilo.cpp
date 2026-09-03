#include "hilo.h"
#include "util.h"
#include "servidor.h"
hilo::hilo() {
}
void hilo::run(){
    int idBateria = mp.leerIdArchivo();
    Servidor server;
    if(!server.iniciar("telemetria")){
        qDebug()<<"No hay conexión entre aplicaciones";
    }
    while(true){
        qDebug()<<"El id es "<<idBateria;
        sleep(30);
        //Determino si hay internet
        bool hayInternet = true;//variableUtil.determinarConexionAInternet();
        int respuesta = -2;
        //Guarda todos los valores
        QJsonArray jsonArray;
        //Recibe los datos de la aplicación pantallaBus.
        // No necesito crear un QObject nuevo para guardar los datos de telemtria
        // Solo tengo que modificar este.
        QJsonObject datos = server.getDatos();
        datos["fecha"] = variableUtil.fechaActual();
        datos["idBateria"] = idBateria;
        qDebug()<<"Datos carga: " << datos["carga"];
        qDebug()<<"Datos corriente: " << datos["corriente"];
        qDebug()<<"Datos voltaje: " << datos["voltaje"];
        qDebug()<<"Datos temperatura: " << datos["temperatura"];
        //No quiero que me mande datos vacios.
        if(datos["carga"] != QJsonValue::Null
            &&datos["corriente"] != QJsonValue::Null
            &&datos["voltaje"] != QJsonValue::Null
            &&datos["temperatura"] != QJsonValue::Null
        ){
            //Determino si tengo conexión a internet.
            if(hayInternet){
                //Envío datos del Excel si hay
                enviarDatosDelExcel(&variableUtil,&mp);
                //Luego voy a enviar el dato leído actual.
                jsonArray = variableUtil.armarQJsonArray(&datos);
                respuesta = variableUtil.postHttp(jsonArray);
                //Se ingresaron correctamente los datos.
                validacionDeId(&respuesta,&idBateria);
            }
            //Surgio un error al enviar la petición HTTP, es decir no se enviaron los datos al servidor.
            //En este caso también tendría que guardar los datos, y luego intentar volver a enviarlos.
            //solo quiero que intente enviar información si tiene internet
            if(respuesta == -1 || !hayInternet){
                //Bien aca lo que yo tengo que hacer es escribir los datos en el excel.
                mp.guardarDatoTelelmetria(&datos);
                qDebug()<<"Se guardaron datos de telemetria en el Excel";
            }
        //Los datos son vacios.
        }else{
            qDebug()<<"Recibi dato nulos, comunicación Telemetria";
        }
    }
}
void hilo::enviarDatosDelExcel(util* u,Manipular_Archivos* mp){
    QJsonObject obj;
    QJsonArray aEnviar;
    obj = mp->leerDatoTelemetria();
    while(!obj.empty()){
        aEnviar = u->armarQJsonArray(&obj);
        id = u->postHttp(aEnviar);
    }

}
void hilo::validacionDeId(int* respuesta, int* idBateria){
    if(*respuesta != -1 && id != -1){
        *idBateria = *respuesta;
        mp.guardarIdArchivo(*idBateria);
        //No se encontro la batería a la que pertenecen esos datos
    }else if(respuesta == 0){
        *idBateria = -1;
    }
}
