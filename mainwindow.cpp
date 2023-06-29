
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionOpenFolder_triggered()
{
    folderPath = QFileDialog::getExistingDirectory(this, "选择被修改文件夹");

    // 2. 读取被修改文件夹路径中的所有文件路径（旧文件路径）
    QDir folderDir(folderPath);
    fileNames = folderDir.entryList(QDir::Files | QDir::NoDotAndDotDot); // 获取文件夹中的所有文件名

    // 将文件路径插入到 listWidget1 组件中
    ui->listWidget1->clear(); // 清空列表显示框
    ui->listWidget1->addItems(fileNames);
}

void MainWindow::on_actionOpenCsv_triggered()
{
    ui->listWidget2->clear(); // 清空列表显示框
    filePath = QFileDialog::getOpenFileName(this, "选择csv文件", "", "CSV 文件 (*.csv)");
    // 1. 读取csv文件中的每一行的值（新文件路径），读取后显示到列表显示框（组件类型：QListWidget，对象名称:listWidget）
    QFile csvFile(filePath);
    if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&csvFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            ui->listWidget2->addItem(line); // 将每一行的值添加到列表显示框中
        }
        csvFile.close();
    }
}

void MainWindow::on_pushButtonClear_clicked()
{
    // 清空列表显示框和进度条
    ui->listWidget1->clear();
    ui->listWidget2->clear();
    ui->progressBar->reset();
}

void MainWindow::on_pushButtonOk_clicked()
{
    // 3. 将listWidget1中的文件名修改成对应的listWidget2中的文件名
    int count = qMin(ui->listWidget1->count(), ui->listWidget2->count()); // 取列表项数量的最小值

    ui->progressBar->setRange(0, count); // 设置进度条的范围
    for (int i = 0; i < count; ++i)
    {
        QString oldFilePath = folderPath + "/" + ui->listWidget1->item(i)->text();
        QString newFilePath = folderPath + "/" + ui->listWidget2->item(i)->text();
        QFile::rename(oldFilePath, newFilePath);

        ui->progressBar->setValue(i + 1); // 更新进度条的值
    }

    QMessageBox::information(this, "修改成功", "文件名修改成功！");
}


void MainWindow::on_actionCheckPath_triggered()
{
    // 1. 检查listWidget1中的文件名和listWidget2中的文件名数量是否一致
    int count1 = ui->listWidget1->count();
    int count2 = ui->listWidget2->count();

    if (count1 != count2)
    {
        QMessageBox::critical(this, "错误", "文件名数量不一致！");
        return;
    }

    // 2. 检查listWidget1中的文件名是否为合法路径
    for (int i = 0; i < count1; ++i)
    {
        QString filePath = folderPath + "/" + ui->listWidget1->item(i)->text();
        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists() || !fileInfo.isFile())
        {
            QMessageBox::critical(this, "错误", "文件路径无效！");
            return;
        }
    }

    // 3. 检查listWidget1中是否有重复值
    QStringList list1;
    for (int i = 0; i < count1; ++i)
    {
        QString fileName = ui->listWidget1->item(i)->text();
        if (list1.contains(fileName))
        {
            QMessageBox::critical(this, "错误", "待修改文件路径中存在重复值！");
            return;
        }
        list1.append(fileName);
    }

    // 4. 检查listWidget2中是否有重复值
    QStringList list2;
    for (int i = 0; i < count2; ++i)
    {
        QString fileName = ui->listWidget2->item(i)->text();
        if (list2.contains(fileName))
        {
            QMessageBox::critical(this, "错误", "新文件路径中存在重复值！");
            return;
        }
        list2.append(fileName);
    }

    QMessageBox::information(this, "检查完成", "路径检查通过！");
}


