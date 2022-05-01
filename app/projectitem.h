#ifndef PROJECTITEM_H
#define PROJECTITEM_H


#include <QList>

class QTreeWidgetItem;
class ProjektWidget;

#include <QDateTime>
#include <QString>

#include <QDataStream>




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

    friend QDataStream & operator<<(QDataStream & ds, const ProjectItem & item);
    friend QDataStream & operator>>(QDataStream & ds, ProjectItem & item);

private:
    QString name;
    QString members;
    QString workDir;
    QString comment;
    QString createData;
    QDateTime createDt;
};

QDataStream & operator<<(QDataStream & ds, const ProjectItem & item);
QDataStream & operator>>(QDataStream & ds, ProjectItem & item);

#endif // PROJECTITEM_H
