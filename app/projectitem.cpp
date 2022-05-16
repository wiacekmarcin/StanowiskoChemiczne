#include "projectitem.h"
#include "projektwidget.h"

#include <QDebug>

ProjectItem::ProjectItem(const QString &name_, const QString &members_, const QString &workdir_,
                         const QString &comment_, const QString &date_, const QDateTime & dt_)
    : name(name_), members(members_), workDir(workdir_), comment(comment_), createData(date_),
      createDt(dt_)
{

}

QString ProjectItem::getName() const
{
    return name;
}

void ProjectItem::setName(const QString &value)
{
    name = value;
}

QString ProjectItem::getMembers() const
{
    return members;
}

void ProjectItem::setMembers(const QString &value)
{
    members = value;
}

QString ProjectItem::getWorkDir() const
{
    return workDir;
}

void ProjectItem::setWorkDir(const QString &value)
{
    workDir = value;
}

QString ProjectItem::getComment() const
{
    return comment;
}

void ProjectItem::setComment(const QString &value)
{
    comment = value;
}

QString ProjectItem::getCreateData() const
{
    return createData;
}

void ProjectItem::setCreateData(const QString &value)
{
    createData = value;
}

void ProjectItem::setWidget(ProjektWidget *w)
{
    w->setDane(name, members, workDir, comment, createData);
}

const QDateTime &ProjectItem::getCreateDt() const
{
    return createDt;
}

void ProjectItem::setCreateDt(const QDateTime &newCreateDt)
{
    createDt = newCreateDt;
}

QDataStream & operator<<(QDataStream & ds, const ProjectItem & item)
{
    qInfo() << "ProjectItem::save" << item.name << "," << item.members << "," << item.workDir << "," << item.comment << "," << item.createData << "," << item.createDt;
    ds << item.name << item.members << item.workDir << item.comment << item.createData << item.createDt;
    return ds;
}

QDataStream & operator>>(QDataStream & ds, ProjectItem &item)
{
    ds >> item.name >> item.members >> item.workDir >> item.comment >> item.createData >> item.createDt;
    qInfo() << "ProjectItem::load" << item.name << "," << item.members << "," << item.workDir << "," << item.comment << "," << item.createData << "," << item.createDt;
    return ds;
}
