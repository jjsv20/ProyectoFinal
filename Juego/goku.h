#ifndef GOKU_H
#define GOKU_H

#include <QObject>

class Goku : public QObject
{
    Q_OBJECT
public:
    explicit Goku(QObject *parent = nullptr);

signals:
};

#endif // GOKU_H
