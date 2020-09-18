#ifndef ARESULT_H
#define ARESULT_H

#include <QVariant>
#include <QSharedPointer>

#include <aqsqlexports.h>

class ASQL_EXPORT AResultPrivate
{
public:
    virtual ~AResultPrivate();

    virtual bool lastResulSet() const = 0;
    virtual bool error() const = 0;
    virtual QString errorString() const = 0;

    virtual int size() const = 0;
    virtual int fields() const = 0;
    virtual int numRowsAffected() const = 0;

    int indexOfField(const QString &name) const;
    virtual QString fieldName(int column) const = 0;
    virtual QVariant value(int row, int column) const = 0;
};

class ASQL_EXPORT ARow
{
public:
    ARow();
    ARow(const QSharedPointer<AResultPrivate> &priv, int index) : d(priv), row(index) {}

private:
    QSharedPointer<AResultPrivate> d;
    int row;
};

class ASQL_EXPORT AResult
{
public:
    AResult();
    AResult(const QSharedPointer<AResultPrivate> &priv);
    AResult(const AResult &other);
    virtual ~AResult();

    bool lastResulSet() const;
    bool error() const;
    QString errorString() const;

    int size() const;
    int fields() const;
    int numRowsAffected() const;

    int indexOfField(const QString &name) const;
    QString fieldName(int column) const;

    /*!
     * \brief columnNames returns the column names
     * \return
     */
    QStringList columnNames() const;

    /*!
     * \brief hash returns the first row as a variant list
     * \return
     */
    QVariantList array() const;

    /*!
     * \brief hash returns the first row as a QHash object
     * \return
     */
    QVariantHash hash() const;

    /*!
     * \brief hashes returns all rows as QVariantHash objects
     * \return
     */
    QVariantList hashes() const;

    /*!
     * \brief jsonObject returns the first row as a JSON object
     * \return
     */
    QJsonObject jsonObject() const;

    /*!
     * \brief jsonArray returns all rows as an array of JSON objects.
     * \return
     */
    QJsonArray jsonArray() const;

    AResult &operator=(const AResult &copy);
    bool operator==(const AResult &other) const;

    class ARow {
    public:
        QSharedPointer<AResultPrivate> d;
        int row;

        explicit inline ARow(QSharedPointer<AResultPrivate> data, int index) : d(data), row(index) { }

        inline int at() const { return row; }
        inline QVariant value(int column) const { return d->value(row, column); }
        inline QVariant value(const QString &name) const { return d->value(row, d->indexOfField(name)); }
        inline QVariant operator[](int column) const { return d->value(row, column); }
        inline QVariant operator[](const QString &name) const { return d->value(row, d->indexOfField(name)); }
    };

    class const_iterator {
    public:
        QSharedPointer<AResultPrivate> d;
        int i;
        typedef std::random_access_iterator_tag  iterator_category;
        typedef ARow value_type;
        typedef ARow reference;

        inline const_iterator() : i(0) { }
        explicit inline const_iterator(QSharedPointer<AResultPrivate> data, int index) : d(data), i(index) { }
        inline const_iterator(const const_iterator &o) : d(o.d), i(o.i) {}

        inline ARow operator*() const { return ARow(d, i); }

        inline int at() const { return i; }
        inline QVariant value(int column) const { return d->value(i, column); }
        inline QVariant value(const QString &name) const { return d->value(i, d->indexOfField(name)); }
        inline QVariant operator[](int column) const { return d->value(i, column); }
        inline QVariant operator[](const QString &name) const { return d->value(i, d->indexOfField(name)); }

        inline bool operator==(const const_iterator &o) const { return i == o.i; }
        inline bool operator!=(const const_iterator &o) const { return i != o.i; }
        inline bool operator<(const const_iterator& other) const { return i < other.i; }
        inline bool operator<=(const const_iterator& other) const { return i <= other.i; }
        inline bool operator>(const const_iterator& other) const { return i > other.i; }
        inline bool operator>=(const const_iterator& other) const { return i >= other.i; }
        inline const_iterator &operator++() { ++i; return *this; }
        inline const_iterator operator++(int) { const_iterator n = *this; ++i; return n; }
        inline const_iterator &operator--() { i--; return *this; }
        inline const_iterator operator--(int) { const_iterator n = *this; i--; return n; }
        inline const_iterator &operator+=(int j) { i+=j; return *this; }
        inline const_iterator &operator-=(int j) { i-=j; return *this; }
        inline const_iterator operator+(int j) const { return const_iterator(d, i+j); }
        inline const_iterator operator-(int j) const { return const_iterator(d, i-j); }
        inline int operator-(const_iterator j) const { return i - j.i; }
    };
    friend class const_iterator;

    // stl style
    inline const_iterator begin() const { return const_iterator(d, 0); }
    inline const_iterator constBegin() const { return const_iterator(d, 0); }
    inline const_iterator end() const { return const_iterator(d, size()); }
    inline const_iterator constEnd() const { return const_iterator(d, size()); }

protected:
    QSharedPointer<AResultPrivate> d;
};

Q_DECLARE_METATYPE(AResult)

#endif // ARESULT_H
