#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QObject>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QDialog *parent = 0);
    void connectToPanel(QObject* panel);

signals:

public slots:

};

#endif // SETTINGSDIALOG_H
