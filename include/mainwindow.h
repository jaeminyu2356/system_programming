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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCurrentDirectory(const std::string& dir);

private slots:
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
    void handleKill(const QString &pid);
    void createNewFolder();
    void deleteSelected();
    void copySelected();
    void pasteToCurrentDir();
    void showContextMenu(const QPoint &pos);
    void showFileDetails(const QString &fileName);
    void handleRmdir();

private:
    void setupUI();
    void createActions();
    void createToolBar();
    void createMenuBar();
    void refreshFileList();
    void updateStatusBar();
    void copyDirectory(const QString &sourcePath, const QString &destPath);
    QString formatSize(qint64 size);
    void handleCd(const QString& path);
    void refreshProcessList(QTableWidget *processTable);

    QFileSystemModel *fileSystemModel;
    QTreeView *treeView;
    QListView *listView;
    QToolBar *fileToolBar;
    std::string currentPath;
    QModelIndexList selectedIndexes;

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
}; 

#endif // MAINWINDOW_H 