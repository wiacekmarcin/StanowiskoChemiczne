#include "projectitem.h"
#include "projektwidget.h"
ProjectItem::ProjectItem(const QString &name_, const QString &members_, const QString &workdir_,
                         const QString &comment_, const QString &date_)
    : name(name_), members(members_), workDir(workdir_), comment(comment_), createData(date_)
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

