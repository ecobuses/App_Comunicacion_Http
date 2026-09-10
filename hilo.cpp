#include "hilo.h"
#include "util.h"
#include "servidor.h"
hilo::hilo() {
}
void hilo::run(){
    server = new Servidor(this);
    servidor2 = new Servidor(this);
    idBateria = mp.leerIdArchivo();
    if(!server->iniciar("telemetria")){
        qDebug()<<"No hay conexión entre aplicaciones";
    }
    if(!servidor2->iniciar("carga_descarga")){
        qDebug()<<"No hay conexión para la carga/descarga";
    }
    connect(server,&Servidor::datosRecibidos,this,&hilo::procesarTramasTelemetria);
    connect(servidor2, &Servidor::datosRecibidos, this, &hilo::procesarTramasCargaDescarga);
    exec();
}
void hilo::enviarDatosDelExcel(util* u,Manipular_Archivos* mp){
    QJsonObject obj;
    QJsonArray aEnviar;
    obj = mp->leerDatoTelemetria();
    int respuesta=0;
    while(!obj.isEmpty()){

        qDebug()<<"Que tiene el objeto antes de enviarlo "<<obj["idBateria"];
        aEnviar = u->armarQJsonArray(&obj);
        //Qué pasa si el ID guardado por alguna razón está desactualizado?
        respuesta = u->postHttp(aEnviar,this->ulrServidor+"/magnitud");
        validacionDeId(&respuesta,&idBateria);
        obj = mp->leerDatoTelemetria();
    }
}
void hilo::validacionDeId(int* respuesta, int* idBateria){
    if(*respuesta != -1/* && id != -1*/){
        *idBateria = *respuesta;
        mp.guardarIdArchivo(*idBateria);
        //No se encontro la batería a la que pertenecen esos datos
    }else if(*respuesta == 0){
        *idBateria = -1;
    }
}
//Recibo los datos de telemtria.
void hilo::procesarTramasTelemetria(){
    qDebug()<<"El id es "<<idBateria;
    QJsonArray jsonArray;
    QJsonObject datos = server->getDatos();
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
        //Envío datos del Excel si hay
        servidorAlive = variableUtil.postHttp(jsonArray,QString(this->ulrServidor+"/util/isAlive")) == 1? true:false;
        if(hayInternet && servidorAlive){
            //Tengo que poder determinar si el servidor esta vivo
            qDebug()<<"Que recibio servidorAlive" << servidorAlive;
            qDebug()<<"Esta vivo";
            enviarDatosDelExcel(&variableUtil,&mp);
            //Luego voy a enviar el dato leído actual.
            jsonArray = variableUtil.armarQJsonArray(&datos);
            respuesta = variableUtil.postHttp(jsonArray,QString(this->ulrServidor+"/magnitud"));
            qDebug()<<"Se guardo la entrada que llego en el momento";
            //Se ingresaron correctamente los datos.
            validacionDeId(&respuesta,&idBateria);
        }
        //Surgio un error al enviar la petición HTTP, es decir no se enviaron los datos al servidor.
        //En este caso también tendría que guardar los datos, y luego intentar volver a enviarlos.
        //solo quiero que intente enviar información si tiene internet
        if(!servidorAlive || !hayInternet){
            //Bien aca lo que yo tengo que hacer es escribir los datos en el excel.
            mp.guardarDatoTelelmetria(&datos);
            qDebug()<<"Se guardaron datos de telemetria en el Excel";
        }
        //Los datos son vacios.
    }else{
        qDebug()<<"Recibi dato nulos, comunicación Telemetria";
    }
}
//Recibo los datos carga/descarga
void hilo::procesarTramasCargaDescarga(){
    // Recibo los datos.
    QJsonObject datos = servidor2->getDatos();
    datos["idBateria"] = idBateria;
    QJsonArray jsonArray;
    if(!variableUtil.determinarConexionAInternet()){
        qDebug()<<"No hay internet para enviar los datos de carga/descarga";
        return;
    }
    if(datos.isEmpty()){
        qDebug()<<"No se recibieron datos";
        return;
    }
    jsonArray.append(datos);
    variableUtil.postHttp(jsonArray,QString(this->ulrServidor+"/cargaDescarga"));
    qDebug()<<"Se envían los datos de carga/descarga";
}