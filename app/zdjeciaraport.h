#ifndef ZDJECIARAPORT_H
#define ZDJECIARAPORT_H

#include <QFrame>
#include <QMap>
#include <QSharedPointer>
#include <QList>
#include <QVector>
#include <QPair>

class PictureFrame;
namespace Ui {
class ZdjeciaRaport;
}

class ZdjeciaRaport : public QFrame
{
    Q_OBJECT

public:
    explicit ZdjeciaRaport(QWidget *parent = nullptr);
    ~ZdjeciaRaport();

    void addImages(const QStringList & files, const QString &workDir);
    QVector<QPair<QString, QString>> getImagesDecription() const;

private slots:
    void deleteFile(const QString fileName);
    void moveFile(bool raport, const QString fileName);
    void upFile(const QString fileName);
    void downFile(const QString fileName);
private:
    Ui::ZdjeciaRaport *ui;

    QMap<QString, QSharedPointer<PictureFrame> > pictures;
    QList<QSharedPointer<PictureFrame> > selected;
};

#endif // ZDJECIARAPORT_H
