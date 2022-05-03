#ifndef PICTUREFRAME_H
#define PICTUREFRAME_H

#include <QFrame>
#include <QString>
#include <QPair>

namespace Ui {
class PictureFrame;
}

class PictureFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PictureFrame(QWidget *parent, const QString & srcFileName);
    ~PictureFrame();
    void setFirst(bool first);
    void setLast(bool last);

    QPair<QString, QString> getData2Pdf() const;
signals:
    void deleteFile(const QString fileName);
    void moveFile(bool raport, const QString fileName);
    void upFile(const QString fileName);
    void downFile(const QString fileName);
private slots:
    void on_deleteBtn();
    void on_moveBtn();
    void on_upBtn();
    void on_downBtn();

private:
    Ui::PictureFrame *ui;
    QString fileName;
    bool inRaport;
};

#endif // PICTUREFRAME_H
