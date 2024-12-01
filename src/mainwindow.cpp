#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QFileInfo>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QItemSelectionModel>

#include "../include/mainwindow.h"
#include "../include/config.h"
#include "../include/commands.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createActions();
    createToolBar();
    createMenuBar();

    // Set initial directory
    currentPath = BASE_DIR;
    fileSystemModel->setRootPath(QString::fromStdString(currentPath));
    treeView->setRootIndex(fileSystemModel->index(QString::fromStdString(currentPath)));
    listView->setRootIndex(fileSystemModel->index(QString::fromStdString(currentPath)));

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);
}

void MainWindow::setupUI()
{
    // Create file system model
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    // Create views
    QSplitter *splitter = new QSplitter(this);
    treeView = new QTreeView(splitter);
    listView = new QListView(splitter);

    treeView->setModel(fileSystemModel);
    listView->setModel(fileSystemModel);

    // Hide unnecessary columns in tree view
    treeView->hideColumn(1); // Size
    treeView->hideColumn(2); // Type
    treeView->hideColumn(3); // Modified Date

    setCentralWidget(splitter);
    statusBar()->showMessage("Ready");
}

void MainWindow::createActions()
{
    // 파일 관련 액션들
    newFolderAction = new QAction(QIcon::fromTheme("folder-new"), tr("새 폴더"), this);
    newFolderAction->setStatusTip(tr("새 폴더 만들기"));
    connect(newFolderAction, &QAction::triggered, this, &MainWindow::createNewFolder);

    deleteAction = new QAction(QIcon::fromTheme("edit-delete"), tr("삭제"), this);
    deleteAction->setStatusTip(tr("선택한 항목 삭제"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelected);

    copyAction = new QAction(QIcon::fromTheme("edit-copy"), tr("복사"), this);
    copyAction->setStatusTip(tr("선택한 항목 복사"));
    connect(copyAction, &QAction::triggered, this, &MainWindow::copySelected);

    pasteAction = new QAction(QIcon::fromTheme("edit-paste"), tr("붙여넣기"), this);
    pasteAction->setStatusTip(tr("복사한 항목 붙여넣기"));
    connect(pasteAction, &QAction::triggered, this, &MainWindow::pasteToCurrentDir);

    // 파일 시스템 기본 동작
    lsAction = new QAction(QIcon::fromTheme("view-list-details"), tr("목록"), this);
    mkdirAction = new QAction(QIcon::fromTheme("folder-new"), tr("새 폴더"), this);
    rmAction = new QAction(QIcon::fromTheme("edit-delete"), tr("삭제"), this);
    chmodAction = new QAction(QIcon::fromTheme("document-properties"), tr("권한 변경"), this);
    cpAction = new QAction(QIcon::fromTheme("edit-copy"), tr("복사"), this);
    renameAction = new QAction(QIcon::fromTheme("edit-rename"), tr("이름 변경"), this);
    lnAction = new QAction(QIcon::fromTheme("insert-link"), tr("링크 생성"), this);
    catAction = new QAction(QIcon::fromTheme("document-open"), tr("파일 보기"), this);
    
    // 프로세스 관련 동작
    psAction = new QAction(QIcon::fromTheme("system-run"), tr("프로세스 목록"), this);
    killAction = new QAction(QIcon::fromTheme("process-stop"), tr("프로세스 종료"), this);

    // 시그널 연결
    connect(lsAction, &QAction::triggered, this, &MainWindow::handleLs);
    connect(mkdirAction, &QAction::triggered, this, &MainWindow::handleMkdir);
    connect(rmAction, &QAction::triggered, this, &MainWindow::handleRm);
    connect(chmodAction, &QAction::triggered, this, &MainWindow::handleChmod);
    connect(cpAction, &QAction::triggered, this, &MainWindow::handleCp);
    connect(renameAction, &QAction::triggered, this, &MainWindow::handleRename);
    connect(lnAction, &QAction::triggered, this, &MainWindow::handleLn);
    connect(catAction, &QAction::triggered, this, &MainWindow::handleCat);
    connect(psAction, &QAction::triggered, this, &MainWindow::handlePs);
    connect(killAction, &QAction::triggered, this, &MainWindow::handleKill);

    // 초기 상태 설정
    deleteAction->setEnabled(false);
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
}

void MainWindow::createToolBar()
{
    fileToolBar = addToolBar(tr("파일"));
    fileToolBar->addAction(newFolderAction);
    fileToolBar->addAction(deleteAction);
    fileToolBar->addAction(copyAction);
    fileToolBar->addAction(pasteAction);
}

void MainWindow::createMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("파일(&F)"));
    fileMenu->addAction(newFolderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(copyAction);
    fileMenu->addAction(pasteAction);

    QMenu *editMenu = menuBar()->addMenu(tr("편집(&E)"));
    // 필요한 경우 추가 메뉴 항목들을 여기에 추가
}

void MainWindow::handleSelectionChanged()
{
    QItemSelectionModel *selectionModel = treeView->hasFocus() ? 
                                        treeView->selectionModel() : 
                                        listView->selectionModel();
    bool hasSelection = selectionModel && selectionModel->hasSelection();

    deleteAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);

    if (hasSelection) {
        QModelIndexList selectedIndexes = selectionModel->selectedRows();
        QString statusMessage = tr("%1개 항목 선택됨").arg(selectedIndexes.count());
        statusBar()->showMessage(statusMessage);
    } else {
        statusBar()->clearMessage();
    }
}

void MainWindow::handleLs()
{
    ls_options opts = {0, 0, 0, 0};  // sort_by_time, sort_by_size, reverse_sort, show_all_times 순서로
    
    // 현재 디렉토리 경로 가져오기
    QString currentDir = QDir::currentPath();
    
    // ls 명령 실행
    call_ls(currentDir.toLocal8Bit().constData(), &opts);
    
    // GUI 업데이트
    refreshFileList();
}

void MainWindow::handleMkdir()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("새 폴더 만들기"),
                                             tr("폴더 이름:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !folderName.isEmpty()) {
        QString currentDir = QDir::currentPath();
        call_mkdir(currentDir.toLocal8Bit().constData(), 
                  folderName.toLocal8Bit().constData());
        refreshFileList();
    }
}

void MainWindow::handleRm()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("삭제 확인"),
        tr("선택한 항목을 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString currentDir = QDir::currentPath();
        for (const QModelIndex &index : selected) {
            QString path = fileSystemModel->filePath(index);
            call_rm(currentDir.toLocal8Bit().constData(),
                   path.toLocal8Bit().constData(),
                   QFileInfo(path).isDir()); // 디렉토리면 recursive
        }
        refreshFileList();
    }
}

void MainWindow::handleChmod()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    bool ok;
    QString mode = QInputDialog::getText(this, tr("권한 변경"),
                                       tr("새 권한 (예: 755):"),
                                       QLineEdit::Normal,
                                       QString(), &ok);
    
    if (ok && !mode.isEmpty()) {
        QString currentDir = QDir::currentPath();
        for (const QModelIndex &index : selected) {
            QString path = fileSystemModel->filePath(index);
            call_chmod(currentDir.toLocal8Bit().constData(),
                      path.toLocal8Bit().constData(),
                      mode.toLocal8Bit().constData());
        }
        refreshFileList();
    }
}

void MainWindow::handleCp()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString sourcePath = fileSystemModel->filePath(selected.first());
    
    QString targetPath = QFileDialog::getSaveFileName(this, tr("복사할 위치 선택"),
                                                    QDir::currentPath());
    
    if (!targetPath.isEmpty()) {
        QString currentDir = QDir::currentPath();
        call_cp(currentDir.toLocal8Bit().constData(),
                sourcePath.toLocal8Bit().constData(),
                targetPath.toLocal8Bit().constData());
        refreshFileList();
    }
}

void MainWindow::handleRename()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString oldPath = fileSystemModel->filePath(selected.first());
    bool ok;
    QString newName = QInputDialog::getText(this, tr("이름 변경"),
                                          tr("새 이름:"), QLineEdit::Normal,
                                          QFileInfo(oldPath).fileName(), &ok);
    
    if (ok && !newName.isEmpty()) {
        QString currentDir = QDir::currentPath();
        QString newPath = QFileInfo(oldPath).dir().filePath(newName);
        call_rename(currentDir.toLocal8Bit().constData(),
                   oldPath.toLocal8Bit().constData(),
                   newPath.toLocal8Bit().constData());
        refreshFileList();
    }
}

void MainWindow::handleLn()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString sourcePath = fileSystemModel->filePath(selected.first());
    
    QDialog dialog(this);
    dialog.setWindowTitle(tr("링크 생성"));
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    QLineEdit *targetEdit = new QLineEdit(&dialog);
    QCheckBox *symbolicCheck = new QCheckBox(tr("심볼릭 링크"), &dialog);
    QPushButton *okButton = new QPushButton(tr("확인"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("취소"), &dialog);
    
    layout->addWidget(new QLabel(tr("대상 경로:")));
    layout->addWidget(targetEdit);
    layout->addWidget(symbolicCheck);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString targetPath = targetEdit->text();
        QString currentDir = QDir::currentPath();
        call_ln(currentDir.toLocal8Bit().constData(),
                sourcePath.toLocal8Bit().constData(),
                targetPath.toLocal8Bit().constData(),
                symbolicCheck->isChecked());
        refreshFileList();
    }
}

void MainWindow::handleCat()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString filePath = fileSystemModel->filePath(selected.first());
    
    // 파일 내용을 보여줄 새 다이얼로그 생성
    QDialog *viewDialog = new QDialog(this);
    viewDialog->setWindowTitle(tr("파일 내용 - %1").arg(QFileInfo(filePath).fileName()));
    viewDialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(viewDialog);
    QTextEdit *textEdit = new QTextEdit(viewDialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);
    
    // cat 명령 실행 결과를 캡처하기 위한 임시 파일 사용
    QString tempFileName = QDir::temp().filePath("cat_output.tmp");
    QString currentDir = QDir::currentPath();
    
    // 파일 내용 읽기
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    if (temp) {
        // 표준 출력을 임시 파일로 리디렉션
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        call_cat(currentDir.toLocal8Bit().constData(),
                filePath.toLocal8Bit().constData());
        
        // 표준 출력 복구
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
        // 임시 파일에서 내용 읽기
        QFile tempFile(tempFileName);
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            textEdit->setPlainText(QString::fromLocal8Bit(tempFile.readAll()));
            tempFile.close();
        }
        
        // 임시 파일 삭제
        QFile::remove(tempFileName);
    }
    
    viewDialog->exec();
    delete viewDialog;
}

void MainWindow::handlePs()
{
    // 프로세스 목록을 보여줄 새 다이얼로그 생성
    QDialog *psDialog = new QDialog(this);
    psDialog->setWindowTitle(tr("프로세스 목록"));
    psDialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(psDialog);
    QTableWidget *processTable = new QTableWidget(psDialog);
    layout->addWidget(processTable);
    
    // ps 명령 실행 결과를 캡처하기 위한 임시 파일 사용
    QString tempFileName = QDir::temp().filePath("ps_output.tmp");
    
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    if (temp) {
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        call_ps("-ef");  // 상세 옵션으로 ps 실
        
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
        // 결과를 테이블에 표시
        QFile tempFile(tempFileName);
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&tempFile);
            QStringList headers = in.readLine().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            
            processTable->setColumnCount(headers.size());
            processTable->setHorizontalHeaderLabels(headers);
            
            while (!in.atEnd()) {
                QStringList fields = in.readLine().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
                int row = processTable->rowCount();
                processTable->insertRow(row);
                for (int i = 0; i < fields.size() && i < headers.size(); ++i) {
                    processTable->setItem(row, i, new QTableWidgetItem(fields[i]));
                }
            }
            tempFile.close();
        }
        QFile::remove(tempFileName);
    }
    
    psDialog->exec();
    delete psDialog;
}

void MainWindow::handleKill()
{
    bool ok;
    QString pid = QInputDialog::getText(this, tr("프로세스 종료"),
                                      tr("PID:"), QLineEdit::Normal,
                                      QString(), &ok);
    if (!ok || pid.isEmpty()) return;
    
    QString signal = QInputDialog::getText(this, tr("시그널 전송"),
                                         tr("그널 번호 (기본: 15):"),
                                         QLineEdit::Normal,
                                         "15", &ok);
    if (!ok) return;
    
    call_kill(pid.toLocal8Bit().constData(),
              signal.toLocal8Bit().constData());
}

void MainWindow::refreshFileList()
{
    // 현재 디렉토리의 모델 새로고침
    fileSystemModel->setRootPath(QDir::currentPath());
    listView->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    QDir currentDir(QDir::currentPath());
    qint64 totalSize = 0;
    int fileCount = 0;
    int dirCount = 0;
    
    // 현재 디렉토리의 항목들을 계산
    QFileInfoList entries = currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &info : entries) {
        if (info.isFile()) {
            totalSize += info.size();
            fileCount++;
        } else if (info.isDir()) {
            dirCount++;
        }
    }
    
    // 상태바 업데이트
    QString status = tr("파일 %1개, 디렉토리 %2개, 총 크기: %3")
                    .arg(fileCount)
                    .arg(dirCount)
                    .arg(formatSize(totalSize));
    statusBar()->showMessage(status);
}

QString MainWindow::formatSize(qint64 size)
{
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double number = size;
    
    while (number >= 1024.0 && unit < 4) {
        number /= 1024.0;
        unit++;
    }
    
    return QString("%1 %2").arg(number, 0, 'f', 2).arg(units[unit]);
}

void MainWindow::createNewFolder()
{
    bool ok;
    QString folderName = QInputDialog::getText(this, tr("새 폴더"), 
                                             tr("폴더 이름:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !folderName.isEmpty()) {
        QString path = QString::fromStdString(currentPath) + "/" + folderName;
        if (QDir().mkdir(path)) {
            fileSystemModel->setRootPath(QString::fromStdString(currentPath));
            refreshFileList();
        } else {
            QMessageBox::warning(this, tr("오류"), tr("폴더를 생성할 수 없습니다."));
        }
    }
}

void MainWindow::deleteSelected()
{
    QModelIndexList selected = treeView->hasFocus() ? 
                             treeView->selectionModel()->selectedRows() :
                             listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("삭제 확인"),
        tr("선택한 항목을 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (const QModelIndex &index : selected) {
            QString path = fileSystemModel->filePath(index);
            QFileInfo fileInfo(path);
            if (fileInfo.isDir()) {
                QDir dir(path);
                dir.removeRecursively();
            } else {
                QFile::remove(path);
            }
        }
        refreshFileList();
    }
}

void MainWindow::copySelected()
{
    selectedIndexes = treeView->hasFocus() ? 
                     treeView->selectionModel()->selectedRows() :
                     listView->selectionModel()->selectedRows();
    pasteAction->setEnabled(!selectedIndexes.isEmpty());
}

void MainWindow::pasteToCurrentDir()
{
    for (const QModelIndex &sourceIndex : selectedIndexes) {
        QString sourcePath = fileSystemModel->filePath(sourceIndex);
        QString fileName = fileSystemModel->fileName(sourceIndex);
        QString destPath = QString::fromStdString(currentPath) + "/" + fileName;
        
        QFileInfo sourceInfo(sourcePath);
        if (sourceInfo.isDir()) {
            QDir().mkpath(destPath);
            copyDirectory(sourcePath, destPath);
        } else {
            QFile::copy(sourcePath, destPath);
        }
    }
    refreshFileList();
}

// 렉토리 복사를 위한 헬퍼 함수
void MainWindow::copyDirectory(const QString &sourcePath, const QString &destPath)
{
    QDir sourceDir(sourcePath);
    QDir destDir(destPath);
    
    foreach(const QFileInfo &info, sourceDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString srcItemPath = sourcePath + "/" + info.fileName();
        QString dstItemPath = destPath + "/" + info.fileName();
        if (info.isDir()) {
            QDir().mkpath(dstItemPath);
            copyDirectory(srcItemPath, dstItemPath);
        } else {
            QFile::copy(srcItemPath, dstItemPath);
        }
    }
}

MainWindow::~MainWindow()
{
    // 필요한 정리 작업이 있다면 여기서 수행
}

void MainWindow::handleCd(const QString& path)
{
    char new_dir[MAX_PATH_SIZE];
    call_cd(currentPath.c_str(), path.toLocal8Bit().constData(), new_dir);
    setCurrentDirectory(new_dir);
    refreshFileList();
}

void MainWindow::setCurrentDirectory(const std::string& dir)
{
    currentPath = dir;
    QString qPath = QString::fromStdString(dir);
    fileSystemModel->setRootPath(qPath);
    treeView->setRootIndex(fileSystemModel->index(qPath));
    listView->setRootIndex(fileSystemModel->index(qPath));
    updateStatusBar();
}
  