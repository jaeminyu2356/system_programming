#include <QtWidgets>
#include "../include/mainwindow.h"
#include "../include/mainwindow_ui.h"
#include "../include/mainwindow_file_actions.h"
#include "../include/mainwindow_process_actions.h"
#include "../include/mainwindow_test_actions.h"
#include "../include/config.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // UI 초기화
    MainWindowUI::setupUI(this);
    MainWindowUI::createActions(this);
    MainWindowUI::createToolBar(this);
    MainWindowUI::createMenuBar(this);

    // 초기 디렉토리 설정
    currentPath = BASE_DIR;
    setCurrentDirectory(currentPath);

    // 시그널/슬롯 연결
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setCurrentDirectory(const std::string& dir, bool addToHistory)
{
    if (addToHistory) {
        directoryHistory.push(QString::fromStdString(currentPath));
        backAction->setEnabled(true);
    }
    
    currentPath = dir;
    MainWindowFileActions::refreshFileList(this);
}

// slots 구현 - 각 액션 클래의 정적 메서드 호출
void MainWindow::handleLs() { MainWindowFileActions::handleLs(this); }
void MainWindow::handleMkdir() { MainWindowFileActions::handleMkdir(this); }
void MainWindow::handleRm() { MainWindowFileActions::handleRm(this); }
void MainWindow::handleChmod() { MainWindowFileActions::handleChmod(this); }
void MainWindow::handleCp() { MainWindowFileActions::handleCp(this); }
void MainWindow::handleRename() { MainWindowFileActions::handleRename(this); }
void MainWindow::handleLn() { MainWindowFileActions::handleLn(this); }
void MainWindow::handleCat() { MainWindowFileActions::handleCat(this); }
void MainWindow::handleRmdir() { MainWindowFileActions::handleRmdir(this); }

void MainWindow::handlePs() { MainWindowProcessActions::handlePs(this); }
void MainWindow::handleKill(const QString &pid) { MainWindowProcessActions::handleKill(this, pid); }

void MainWindow::handleMmapTest() { MainWindowTestActions::handleMmapTest(this); }
void MainWindow::handleExecuteProgram() { MainWindowTestActions::handleExecuteProgram(this); }

void MainWindow::createNewFolder() { MainWindowFileActions::createNewFolder(this); }
void MainWindow::deleteSelected()
{
    qDebug() << "MainWindow::deleteSelected called";
    MainWindowFileActions::deleteSelected(this);
}
void MainWindow::copySelected() { MainWindowFileActions::copySelected(this); }
void MainWindow::pasteToCurrentDir() { MainWindowFileActions::pasteToCurrentDir(this); }
void MainWindow::showContextMenu(const QPoint &pos)
{
    qDebug() << "MainWindow::showContextMenu called at" << pos;
    MainWindowFileActions::showContextMenu(this, pos);
}
void MainWindow::showFileDetails(const QString &fileName) { MainWindowFileActions::showFileDetails(this, fileName); }
void MainWindow::handleSelectionChanged() { MainWindowUI::handleSelectionChanged(this); }
  