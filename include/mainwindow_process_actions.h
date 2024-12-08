#ifndef MAINWINDOW_PROCESS_ACTIONS_H
#define MAINWINDOW_PROCESS_ACTIONS_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"

class MainWindowProcessActions : public QObject {
    Q_OBJECT
public:
    explicit MainWindowProcessActions(QObject *parent = nullptr);
    static void handlePs(MainWindow* window);
    static void handleKill(MainWindow* window, const QString &pid);
    static void refreshProcessList(MainWindow* window, QTableWidget *processTable);
};

#endif // MAINWINDOW_PROCESS_ACTIONS_H 