#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "QVariant"

class JsonSerializer
{
public:
    static QByteArray serialize(QVariant content, bool& ok);
    static QByteArray serialize(QVariant content);

private:
    JsonSerializer() {}
    static QByteArray doubleQuoted(QByteArray str);
    static QByteArray embraceCurly(QByteArray str);
    static QByteArray embraceSquare(QByteArray str);
    static QByteArray linkWithColon(QByteArray firstStr, QByteArray secondStr);
    static QByteArray commaSeparated(QStringList list);
    static QByteArray escaped(QByteArray str);

    template <class T>
    static QByteArray serializeDict(T dict, bool& ok);

    template <class T>
    static QByteArray serializeList(QList<T> list, bool& ok);
};

#endif // JSONSERIALIZER_H
