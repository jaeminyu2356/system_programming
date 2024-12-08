#ifndef MAINWINDOW_ACTIONS_H
#define MAINWINDOW_ACTIONS_H

#include "mainwindow.h"

// 파일 관련 액션 처리
void MainWindow::handleLs();
void MainWindow::handleMkdir();
void MainWindow::handleRm();
void MainWindow::handleChmod();
void MainWindow::handleCp();
void MainWindow::handleRename();
void MainWindow::handleLn();
void MainWindow::handleCat();
void MainWindow::handleRmdir();

// 프로세스 관련 액션 처리
void MainWindow::handlePs();
void MainWindow::handleKill(const QString &pid);
void MainWindow::refreshProcessList(QTableWidget *processTable);

// 테스트 관련 액션 처리
void MainWindow::handleMmapTest();
void MainWindow::handleExecuteProgram();

// 유틸리티 함수들
void MainWindow::createNewFolder();
void MainWindow::deleteSelected();
void MainWindow::copySelected();
void MainWindow::pasteToCurrentDir();
void MainWindow::refreshFileList();
void MainWindow::updateStatusBar();
QString MainWindow::formatSize(qint64 size);
void MainWindow::showContextMenu(const QPoint &pos);
void MainWindow::showFileDetails(const QString &fileName);

#endif // MAINWINDOW_ACTIONS_H 