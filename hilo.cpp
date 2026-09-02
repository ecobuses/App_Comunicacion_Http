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
        //Se duerme 5 minutos si 300 es el argumento
        sleep(300);
        //Determino si hay internet
        bool hayInternet = variableUtil.determinarConexionAInternet();
        //Guarda todos los valores
        QJsonArray jsonArray;
        //Recibe los datos de la aplicación pantallaBus.
        // No necesito crear un QObject nuevo para guardar los datos de telemtria
        // Solo tengo que modificar este.
        QJsonObject datos = server.getDatos();
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
            //Devuelve los datos en el QJsonArray
            jsonArray = variableUtil.armarQJsonArray(datos,idBateria);
            int respuesta =variableUtil.postHttp(jsonArray);
            //Se ingresaron correctamente los datos.
            if(respuesta != -1 && id != -1){
                idBateria = respuesta;
                mp.guardarIdArchivo(idBateria);
            //No se encontro la batería a la que pertenecen esos datos
            }else if(respuesta == 0){
                idBateria = -1;
            //Surgio un error al enviar la petición HTTP, es decir no se enviaron los datos al servidor.
            //En este caso también tendría que guardar los datos, y luego intentar volver a enviarlos.
            //solo quiero que intente enviar información si tiene internet
            }else if (respuesta == -1 || !hayInternet){
                //Bien aca lo que yo tengo que hacer es escribir los datos en el excel.
                mp.guardarDatoTelelmetria(datos);
            }
        }else{
            qDebug()<<"Recibi dato nulos, comunicación Telemetria";
        }
    }
}
