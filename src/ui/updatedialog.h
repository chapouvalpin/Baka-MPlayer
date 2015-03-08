#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QTime>


namespace Ui {
class UpdateDialog;
}

class KuroEngine;

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(KuroEngine *kuro, QWidget *parent = 0);
    ~UpdateDialog();

    static void CheckForUpdates(KuroEngine *kuro, QWidget *parent = 0);

protected slots:
    void Prepare();
    void ShowInfo();

private:
    Ui::UpdateDialog *ui;
    KuroEngine *kuro;

    QTime *timer;
    double avgSpeed = 1,
           lastSpeed=0;
    int lastProgress,
        lastTime,
        state;
    bool init;
};

#endif // UPDATEDIALOG_H
