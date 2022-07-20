#ifndef CKINECTREADER_H
#define CKINECTREADER_H

#include <QObject>

class CKinectReader : public QObject
{
    Q_OBJECT
public:
    explicit CKinectReader(QObject *parent = nullptr);

signals:

};

#endif // CKINECTREADER_H
