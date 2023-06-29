
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenFolder_triggered();

    void on_pushButtonClear_clicked();

    void on_pushButtonOk_clicked();

    void on_actionOpenCsv_triggered();

    void on_actionCheckPath_triggered();

private:
    Ui::MainWindow *ui;
    QString folderPath;
    QString filePath;
    QStringList fileNames;

};

#endif // MAINWINDOW_H
