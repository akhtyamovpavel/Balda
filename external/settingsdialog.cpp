#include "settingsdialog.h"

#include <QWidget>
#include <QPair>


SettingsDialog::SettingsDialog(MainWindow* window, QWidget *parent) :
    QDialog(parent)
{
    widthSpinBox = new QSpinBox;
    widthLabel = new QLabel;
    heightSpinBox = new QSpinBox;
    heightLabel = new QLabel;


    widthLabel->setText(tr("Width"));
    heightLabel->setText(tr("Height"));

    widthSpinBox->setMinimum(3);
    widthSpinBox->setMaximum(10);
    heightSpinBox->setMinimum(3);
    heightSpinBox->setMaximum(10);


    widthSpinBox->setValue(window->getWidth());
    heightSpinBox->setValue(window->getHeight());


    widthLayout = new QHBoxLayout;
    widthLayout->addWidget(widthLabel);
    widthLayout->addWidget(widthSpinBox);

    heightLayout = new QHBoxLayout();
    heightLayout->addWidget(heightLabel);
    heightLayout->addWidget(heightSpinBox);

    saveButton = new QPushButton;
    saveButton->setText(tr("Save"));
    closeButton = new QPushButton;
    closeButton->setText(tr("Close"));



    connect(saveButton, SIGNAL(released()), this, SLOT(sendData()));
    connect(closeButton, SIGNAL(released()), this, SLOT(close()));

    connect(this, SIGNAL(saveData(const QPair<int,int>&)),
            window, SLOT(setSettings(const QPair<int,int>&)));

    panelLayout = new QVBoxLayout;
    panelLayout->addLayout(widthLayout);
    panelLayout->addLayout(heightLayout);

    panelLayout->addWidget(saveButton);
    panelLayout->addWidget(closeButton);

    setLayout(panelLayout);
    resize(150, 200);

}


void SettingsDialog::setWidth(int width) {
    lettersWidth = width;
}

void SettingsDialog::setHeight(int height) {
    lettersHeight = height;
}

void SettingsDialog::sendData() {
    QPair<int,int> packedData = qMakePair(widthSpinBox->value(),
                                          heightSpinBox->value());
    emit saveData(packedData);
}
