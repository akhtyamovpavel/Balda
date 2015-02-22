#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QObject>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "mainwindow.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
    int lettersWidth;
    int lettersHeight;

    QSpinBox* widthSpinBox;
    QLabel* widthLabel;

    QSpinBox* heightSpinBox;
    QLabel* heightLabel;

    QHBoxLayout* widthLayout;
    QHBoxLayout* heightLayout;
    QPushButton* saveButton;
    QPushButton* closeButton;

    QVBoxLayout* panelLayout;


public:
    explicit SettingsDialog(MainWindow* window, QWidget *parent = 0);
    void connectToPanel(QObject* panel);

    void setHeight(int height);
    void setWidth(int width);

signals:
    void saveData(const QPair<int, int>& packedData);

public slots:

    void sendData();
};

#endif // SETTINGSDIALOG_H
