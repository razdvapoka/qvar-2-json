#include "jsonserializer.h"
#include "QStringList"

QByteArray JsonSerializer::serialize(QVariant content, bool& ok)
{
    QByteArray jsonString;
    if(content.isValid())
    {
        if(!content.isNull())
        {
            switch(content.type())
            {
                case QVariant::Map: jsonString = serializeDict<QVariantMap>(content.toMap(), ok); break;
                case QVariant::Hash: jsonString = serializeDict<QVariantHash>(content.toHash(), ok); break;
                case QVariant::List: jsonString = serializeList<QVariant>(content.toList(), ok); break;
                case QVariant::StringList: jsonString = serializeList<QString>(content.toStringList(), ok); break;
                case QVariant::Int:
                case QVariant::UInt:
                case QVariant::LongLong:
                case QVariant::ULongLong:
                case QVariant::Double:
                case QVariant::Bool: jsonString = content.toByteArray(); break;
                default: jsonString = doubleQuoted(escaped(content.toByteArray())); break;
            }
        }
        else
            jsonString += "null";
    }
    else
    {
        jsonString += doubleQuoted(QByteArray());
        ok = false;
    }

    return jsonString;
}


QByteArray JsonSerializer::serialize(QVariant content)
{
    bool ok;
    return serialize(content, ok);
}


template <class T>
QByteArray JsonSerializer::serializeDict(T dict, bool &ok)
{
    QByteArray jsonString;
    QStringList pairs;
    foreach(QString key, dict.keys())
        pairs << linkWithColon(doubleQuoted(key.toUtf8()), serialize(dict[key], ok));

    jsonString += commaSeparated(pairs);
    jsonString = embraceCurly(jsonString);
    return jsonString;
}


template <class T>
QByteArray JsonSerializer::serializeList(QList<T> list, bool &ok)
{
    QByteArray jsonString;
    QStringList items;
    foreach(T item, list)
        items += serialize(item, ok);
    jsonString += commaSeparated(items);
    jsonString = embraceSquare(jsonString);
    return jsonString;
}


QByteArray JsonSerializer::doubleQuoted(QByteArray str)
{
    return ("\"" + str + "\"");
}


QByteArray JsonSerializer::embraceCurly(QByteArray str)
{
    return ("{ " + str + " }");
}


QByteArray JsonSerializer::embraceSquare(QByteArray str)
{
    return ("[ " + str + " ]");
}


QByteArray JsonSerializer::linkWithColon(QByteArray firstStr, QByteArray secondStr)
{
    return (firstStr + " : " + secondStr);
}


QByteArray JsonSerializer::commaSeparated(QStringList list)
{
    QByteArray commaSeparatedStr;
    if(!list.isEmpty())
    {
        commaSeparatedStr += list.first();
        for(int i = 1; i < list.count(); i++)
            commaSeparatedStr += ", " + list[i];

    }
    return commaSeparatedStr;
}


QByteArray JsonSerializer::escaped(QByteArray str)
{
    str.replace("\\", "\\\\");
    str.replace("\"", "\\\"");
    str.replace("\b", "\\b");
    str.replace("\f", "\\f");
    str.replace("\n", "\\n");
    str.replace("\r", "\\r");
    str.replace("\t", "\\t");
    return str;
}
