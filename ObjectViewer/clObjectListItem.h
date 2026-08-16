#ifndef CLOBJECTLISTITEM_H_WVD_08102011
#define CLOBJECTLISTITEM_H_WVD_08102011

#include <vector>


#include <QtCore/QString>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QListWidget>


using namespace std;


class clObjectListItem : public QObject, public QListWidgetItem
{

    Q_OBJECT;

public:
    clObjectListItem(QString &paItem, QWidget* paParent = 0, const char* paName = 0) : QListWidgetItem(paItem)   {}
    clObjectListItem(const QIcon & icon, const QString & text) : QListWidgetItem(icon,text)   {}
public:
    void setName(QString paName);
    void setAlias(QString paAlias);
    void setType(QString paType);
    void setExtra(QString paExtra);
    void setReference(QString paReference);
    void setValue(QString paValue);
    void setTableName(QString paTableName);
    void setId(QString meId);

    QString getName();
    QString getAlias();
    QString getType();
    QString getExtra();
    QString getReference();
    QString getValue();
    QString getTableName();
    QString getId();
private:

    QString meName;
    QString meAlias;
    QString meType;
    QString meExtra;
    QString meReference;
    QString meValue;
    QString meId;
    QString meTableName;
};

#endif
