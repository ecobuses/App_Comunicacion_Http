#ifndef HILO_H
#define HILO_H
#include <QThread>
#include <QTimeZone>
#include "util.h"
#include "servidor.h"
class hilo:public QThread
{
public:
    hilo();
    void setId(int id){this->id=id;}
    int getId(){return this->id;}
private:
    int id;
    util variableUtil;
protected:
    void run();
};

#endif // HILO_H
