#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTableWidget>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <string>
#include <QHeaderView>
#include <QTimer>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QDockWidget>
#include <QProgressDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QStack>
#include "mainwindow_file_actions.h"

class MainWindowUI;
class MainWindowFileActions;
class MainWindowProcessActions;
class MainWindowTestActions;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MainWindowUI;
    friend class MainWindowFileActions;
    friend class MainWindowProcessActions;
    friend class MainWindowTestActions;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCurrentDirectory(const std::string& dir, bool addToHistory = true);
    QString getCurrentDirectory() const { return QString::fromStdString(currentPath); }

public slots:
    void handleSelectionChanged();
    void handleLs();
    void handleMkdir();
    void handleRm();
    void handleChmod();
    void handleCp();
    void handleRename();
    void handleLn();
    void handleCat();
    void handlePs();
    void handleKill(const QString &pid = QString());
    void createNewFolder();
    void deleteSelected();
    void copySelected();
    void pasteToCurrentDir();
    void showContextMenu(const QPoint &pos);
    void showFileDetails(const QString &fileName);
    void handleRmdir();
    void handleMmapTest();
    void handleExecuteProgram();

private:
    QFileSystemModel *fileSystemModel;
    QTreeView *treeView;
    QListView *listView;
    QToolBar *fileToolBar;
    std::string currentPath;
    QModelIndexList selectedIndexes;
    bool moveOperation = false;  // 이동 작업 여부를 나타내는 플래그

    // Actions
    QAction *newFolderAction;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *renameAction;
    QAction *chmodAction;
    QAction *lsAction;
    QAction *mkdirAction;
    QAction *rmAction;
    QAction *cpAction;
    QAction *lnAction;
    QAction *catAction;
    QAction *psAction;
    QAction *killAction;
    QAction *exitAction;
    QAction *rmdirAction;
    QAction *mmapTestAction;
    QAction *execProgramAction;
    QAction *backAction;
    QStack<QString> directoryHistory;
}; 

#endif // MAINWINDOW_H 