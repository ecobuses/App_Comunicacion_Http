#include "hilo.h"
#include "util.h"
#include "servidor.h"
hilo::hilo() {
}
void hilo::run(){
    Servidor server;
    int idBateria = mp.leerIdArchivo();
    if(!server.iniciar("telemetria")){
        qDebug()<<"No hay conexión entre aplicaciones";
    }
    while(true){
        qDebug()<<"El id es "<<idBateria;
        sleep(300);
        QJsonArray jsonArray;
        QJsonObject datos = server.getDatos();
        qDebug()<<"Datos carga: " << datos["carga"];
        qDebug()<<"Datos corriente: " << datos["corriente"];
        qDebug()<<"Datos voltaje: " << datos["voltaje"];
        qDebug()<<"Datos temperatura: " << datos["temperatura"];
        //Guarda todos los valores
        datos["fecha"] = variableUtil.fechaActual();
        datos["idBateria"] = idBateria;
        //No quiero que me mande datos vacios.
        if(!datos.isEmpty()
        ){
             int respuesta=-1;
            //Determino si hay internet
            bool hayInternet = variableUtil.determinarConexionAInternet();
            //Determino si tengo conexión a internet.
            if(hayInternet){
                //Tengo que poder determinar si el servidor esta vivo
                //Envío datos del Excel si hay
                if(servidorAlive){
                    enviarDatosDelExcel(&variableUtil,&mp);
                }
                //Luego voy a enviar el dato leído actual.
                jsonArray = variableUtil.armarQJsonArray(&datos);
                respuesta = variableUtil.postHttp(jsonArray);
                qDebug()<<"Se guardo la entrada que llego en el momento";
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
    int respuesta=0;
    while(!obj.isEmpty()){
        if(id != obj["idBateria"]){
            obj["idBateria"] = id;
        }
        aEnviar = u->armarQJsonArray(&obj);
        //Qué pasa si el ID guardado por alguna razón está desactualizado?
        respuesta = u->postHttp(aEnviar);
        validacionDeId(&respuesta,&id);
        obj = mp->leerDatoTelemetria();
    }

}
void hilo::validacionDeId(int* respuesta, int* idBateria){
    if(*respuesta != -1/* && id != -1*/){
        *idBateria = *respuesta;
        mp.guardarIdArchivo(*idBateria);
        //No se encontro la batería a la que pertenecen esos datos
    }else if(respuesta == 0){
        *idBateria = -1;
    }
}
