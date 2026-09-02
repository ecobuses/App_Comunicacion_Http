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
        //Se duerme 5 segúndos si 300 es el argumento
        sleep(300);
        //Determino si hay internet
        bool hayInternet = variableUtil.determinarConexionAInternet();
        QJsonArray jsonArray;
        QJsonObject objCarga;
        QJsonObject objTipo;
        QJsonObject datos = server.getDatos();
        qDebug()<<"Datos carga: " << datos["carga"];
        qDebug()<<"Datos corriente: " << datos["corriente"];
        qDebug()<<"Datos voltaje: " << datos["voltaje"];
        qDebug()<<"Datos temperatura: " << datos["temperatura"];
        //No quiero que me mande datos vacios.
        //Además solo quiero que intente enviar información si tiene internet
        if(datos["carga"] != QJsonValue::Null
            &&datos["corriente"] != QJsonValue::Null
            &&datos["voltaje"] != QJsonValue::Null
            &&datos["temperatura"] != QJsonValue::Null
        ){
            QDateTime horaUtc = QDateTime::currentDateTimeUtc();
            // 2. Crear la zona horaria UTC-3 (3 horas * 3600 segundos = 10800)
            QTimeZone zonaMenosTres(-10800);
            // 3. Convertir la hora a la nueva zona
            QDateTime horaLocal = horaUtc.toTimeZone(zonaMenosTres);
            // Formatear a: yyyy-MM-dd HH:mm:ss
            QString formato = horaLocal.toString("yyyy-MM-dd HH:mm:ss");
            qDebug()<<"La hora que toma el sistema es: "<< formato;
            objCarga.insert("id",QJsonValue::Null);
            objCarga.insert("fecha",formato);
            objCarga.insert("valor",datos["carga"].toDouble());
            objTipo.insert("id",QJsonValue::Null);
            objTipo.insert("tipo","CARGA");
            objCarga.insert("idBateria",idBateria);
            objCarga.insert("tipo",objTipo);
            jsonArray.append(objCarga);
            QJsonObject objCorriente;
            objCorriente.insert("id",QJsonValue::Null);
            objCorriente.insert("fecha",formato);
            objCorriente.insert("valor",datos["corriente"].toDouble());
            objTipo.insert("id",QJsonValue::Null);
            objTipo.insert("tipo","CORRIENTE");
            objCorriente.insert("tipo",objTipo);
            objCorriente.insert("idBateria",idBateria);
            jsonArray.append(objCorriente);
            QJsonObject objTension;
            objTension.insert("id",QJsonValue::Null);
            objTension.insert("fecha",formato);
            objTension.insert("valor",datos["voltaje"].toDouble());
            objTipo.insert("id",QJsonValue::Null);
            objTipo.insert("tipo","TENSION");
            objTension.insert("tipo",objTipo);
            objTension.insert("idBateria",idBateria);
            jsonArray.append(objTension);
            QJsonObject objTemperatura;
            objTemperatura.insert("id",QJsonValue::Null);
            objTemperatura.insert("fecha",formato);
            objTemperatura.insert("valor",datos["temperatura"].toDouble());
            objTipo.insert("id",QJsonValue::Null);
            objTipo.insert("tipo","TEMPERATURA");
            objTemperatura.insert("tipo",objTipo);
            objTemperatura.insert("idBateria",idBateria);
            jsonArray.append(objTemperatura);
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
            }else if (respuesta == -1 || !hayInternet){

            }
        }
    }
}
