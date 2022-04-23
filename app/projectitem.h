#ifndef PROJECTITEM_H
#define PROJECTITEM_H


#include <QList>

class QTreeWidgetItem;
class ProjektWidget;

#include <QDateTime>
#include <QString>

class ProjectItem
{
public:
    ProjectItem()  {}
    ProjectItem(const QString & name, const QString & members, const QString & workdir,
                const QString & comment, const QString & date, const QDateTime & dt );


    QString getName() const;
    void setName(const QString &value);

    QString getMembers() const;
    void setMembers(const QString &value);

    QString getWorkDir() const;
    void setWorkDir(const QString &value);

    QString getComment() const;
    void setComment(const QString &value);

    QString getCreateData() const;
    void setCreateData(const QString &value);

    void setWidget(ProjektWidget *w);

    const QDateTime &getCreateDt() const;
    void setCreateDt(const QDateTime &newCreateDt);

private:
    QString name;
    QString members;
    QString workDir;
    QString comment;
    QString createData;
    QDateTime createDt;
};

#endif // PROJECTITEM_H
