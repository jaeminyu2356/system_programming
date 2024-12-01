#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <string>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QItemSelectionModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QRegularExpression>
#include <unistd.h>
#include "../include/commands.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCurrentDirectory(const std::string& dir);

private slots:
    // GUI 이벤트 처리 함수들
    void createNewFolder();
    void deleteSelected();
    void copySelected();
    void pasteToCurrentDir();
    void handleSelectionChanged();
    void updateStatusBar();
    
    // 기존 명령어들의 GUI 버전
    void handleLs();
    void handleCd(const QString& path);
    void handleMkdir();
    void handleRm();
    void handleChmod();
    void handleCp();
    void handleRename();
    void handleLn();
    void handleCat();
    void handlePs();
    void handleKill();

private:
    void setupUI();
    void createActions();
    void createToolBar();
    void createMenuBar();
    void executeCommand(const QString& command);

    QTreeView *treeView;
    QListView *listView;
    QToolBar *fileToolBar;
    std::string currentPath;
    QString clipboardPath;
    QAction *newFolderAction;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *pasteAction;

    // 액션들
    QAction *lsAction;
    QAction *mkdirAction;
    QAction *rmAction;
    QAction *chmodAction;
    QAction *cpAction;
    QAction *renameAction;
    QAction *lnAction;
    QAction *catAction;
    QAction *psAction;
    QAction *killAction;

    // 파일 시스템 모델
    QFileSystemModel *fileSystemModel;

    // 유틸리티 함수들
    void refreshFileList();
    QString formatSize(qint64 size);

    QModelIndexList selectedIndexes;
    void copyDirectory(const QString &sourcePath, const QString &destPath);
};

#endif // MAINWINDOW_H
