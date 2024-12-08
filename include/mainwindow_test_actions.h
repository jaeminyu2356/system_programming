#ifndef MAINWINDOW_TEST_ACTIONS_H
#define MAINWINDOW_TEST_ACTIONS_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"

class MainWindowTestActions : public QObject {
    Q_OBJECT
public:
    explicit MainWindowTestActions(QObject *parent = nullptr);
    static void handleMmapTest(MainWindow* window);
    static void handleExecuteProgram(MainWindow* window);
};

#endif // MAINWINDOW_TEST_ACTIONS_H 