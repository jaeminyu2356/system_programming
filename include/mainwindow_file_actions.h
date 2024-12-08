#ifndef MAINWINDOW_FILE_ACTIONS_H
#define MAINWINDOW_FILE_ACTIONS_H

#include <QMainWindow>
#include <QObject>

class MainWindow;  // Forward declaration

class MainWindowFileActions : public QObject {
    Q_OBJECT
public:
    explicit MainWindowFileActions(QObject *parent = nullptr);

    // 파일 관련 액션 처리
    static void handleLs(MainWindow* window);
    static void handleMkdir(MainWindow* window);
    static void handleRm(MainWindow* window);
    static void handleChmod(MainWindow* window);
    static void handleCp(MainWindow* window);
    static void handleRename(MainWindow* window);
    static void handleLn(MainWindow* window);
    static void handleCat(MainWindow* window);
    static void handleRmdir(MainWindow* window);

    // 파일 유틸리티 함수들
    static void createNewFolder(MainWindow* window);
    static void deleteSelected(MainWindow* window);
    static void copySelected(MainWindow* window, bool isMove = false);
    static void pasteToCurrentDir(MainWindow* window);
    static void refreshFileList(MainWindow* window);
    static void updateStatusBar(MainWindow* window);
    static QString formatSize(qint64 size);
    static void showContextMenu(MainWindow* window, const QPoint &pos);
    static void showFileDetails(MainWindow* window, const QString &fileName);

private:
    static void copyDirectory(const QString &sourcePath, const QString &destPath);
    static bool removeDirectory(const QString &dirPath);
};

#endif // MAINWINDOW_FILE_ACTIONS_H 