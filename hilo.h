#ifndef HILO_H
#define HILO_H
#include <QThread>
#include <QTimeZone>
#include "util.h"
#include "manipuar_archivos.h"
class hilo:public QThread
{
public:
    hilo();
    void setId(int id){this->id=id;}
    int getId(){return this->id;}
private:
    int id;
    util variableUtil;
    Manipular_Archivos mp;

protected:
    void run();
};

#endif // HILO_H
