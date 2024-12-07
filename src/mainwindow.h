class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    // ... 기존 private 멤버들 ...
    
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
}; 