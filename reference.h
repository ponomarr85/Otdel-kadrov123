#ifndef REFERENCE_H
#define REFERENCE_H

#include <QString>
#include <QDataStream>

class Reference
{
public:
    Reference();

    friend QDataStream &operator >>(QDataStream &inputStream, Reference &reference);

    friend QDataStream &operator <<(QDataStream &outputStream, const Reference &reference);

    const int &number() const;
    void setNumber(const int &number);

    const int &workerId() const;
    void setWorkerId(const int &workerId);

    const QString &description() const;
    void setDescription(const QString &description);

    const QString &createdDate() const;
    void setCreatedDate(const QString &createdDate);
private:
    int mNumber, mWorkerId;
    QString mDescription, mCreatedDate;
};

#endif // REFERENCE_H
