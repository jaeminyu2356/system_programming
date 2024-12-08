#ifndef MAINWINDOW_UI_H
#define MAINWINDOW_UI_H

#include <QMainWindow>
#include <QObject>
#include "mainwindow.h"
#include "mainwindow_file_actions.h"
#include "mainwindow_process_actions.h"
#include "mainwindow_test_actions.h"

class MainWindowUI : public QObject {
    Q_OBJECT
public:
    explicit MainWindowUI(QObject *parent = nullptr);
    static void setupUI(MainWindow* window);
    static void createActions(MainWindow* window);
    static void createToolBar(MainWindow* window);
    static void createMenuBar(MainWindow* window);
    static void handleSelectionChanged(MainWindow* window);
};

#endif // MAINWINDOW_UI_H 