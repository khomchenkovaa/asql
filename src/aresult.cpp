#include "aresult.h"

#include <QJsonArray>
#include <QJsonObject>

AResult::AResult()
{

}

AResult::~AResult()
{

}

QStringList AResult::columnNames()
{
    QStringList columns;
    for (int i = 0; i < fields(); ++i) {
        columns.append(fieldName(i));
    }
    return columns;
}

QVariantList AResult::array()
{
    QVariantList ret;
    if (size()) {
        setAt(0);
        for (int i = 0; i < fields(); ++i) {
            ret.append(value(i));
        }
    }
    return ret;
}

QVariantHash AResult::hash()
{
    QVariantHash ret;
    if (size()) {
        setAt(0);
        for (int i = 0; i < fields(); ++i) {
            ret.insert(fieldName(i), value(i));
        }
    }
    return ret;
}

QVariantList AResult::hashes()
{
    QVariantList ret;
    if (size()) {
        setAt(0);
        const QStringList columns = columnNames();

        QVariantHash obj;
        do {
            for (int i = 0; i < fields(); ++i) {
                obj.insert(columns[i],value(i));
            }
            ret.append(obj);
        } while (next());
    }
    return ret;
}

QJsonObject AResult::jsonObject()
{
    QJsonObject ret;
    if (size()) {
        setAt(0);
        for (int i = 0; i < fields(); ++i) {
            ret.insert(fieldName(i), QJsonValue::fromVariant(value(i)));
        }
    }
    return ret;
}

QJsonArray AResult::jsonArray()
{
    QJsonArray ret;
    if (size()) {
        setAt(0);
        const QStringList columns = columnNames();

        QJsonObject obj;
        do {
            for (int i = 0; i < fields(); ++i) {
                obj.insert(columns[i], QJsonValue::fromVariant(value(i)));
            }
            ret.append(obj);
        } while (next());
    }
    return ret;
}

