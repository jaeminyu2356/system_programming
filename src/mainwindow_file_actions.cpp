#include <QtWidgets>
#include "../include/mainwindow.h"
#include "../include/mainwindow_file_actions.h"
#include "../include/config.h"
#include "../include/commands.h"

MainWindowFileActions::MainWindowFileActions(QObject *parent) : QObject(parent) {}

void MainWindowFileActions::handleLs(MainWindow* window)
{
    QDialog *dialog = new QDialog(window);
    dialog->setWindowTitle(QObject::tr("디렉토리 상세 정보"));
    dialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // 현재 경로 표시
    QLabel *pathLabel = new QLabel(QObject::tr("현재 경로: %1").arg(QString::fromStdString(window->currentPath)));
    layout->addWidget(pathLabel);
    
    // 구분선 추가
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
    
    // 옵션 체크박스들
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QCheckBox *timeSort = new QCheckBox(QObject::tr("-t (시간순 정렬)"));
    QCheckBox *sizeSort = new QCheckBox(QObject::tr("-S (크기순 정렬)"));
    QCheckBox *reverseSort = new QCheckBox(QObject::tr("-r (역순 정렬)"));
    QCheckBox *showAllTimes = new QCheckBox(QObject::tr("-T (모든 시간 표시)"));
    
    optionsLayout->addWidget(timeSort);
    optionsLayout->addWidget(sizeSort);
    optionsLayout->addWidget(reverseSort);
    optionsLayout->addWidget(showAllTimes);
    layout->addLayout(optionsLayout);
    
    // 결과를 표시할 텍스트 에디터
    QTextEdit *resultText = new QTextEdit(dialog);
    resultText->setReadOnly(true);
    resultText->setFont(QFont("Monospace"));
    layout->addWidget(resultText);
    
    // 새로고침 버튼
    QPushButton *refreshButton = new QPushButton(QObject::tr("새로고침"), dialog);
    layout->addWidget(refreshButton);
    
    // 새로고침 함수 정의
    auto refreshListing = [=]() {
        QString tempFileName = QDir::temp().filePath("ls_output.tmp");
        FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
        
        if (temp) {
            int stdout_fd = dup(STDOUT_FILENO);
            dup2(fileno(temp), STDOUT_FILENO);
            
            struct ls_options opts = {
                timeSort->isChecked(),
                sizeSort->isChecked(),
                reverseSort->isChecked(),
                showAllTimes->isChecked(),
                false
            };
            
            call_ls(window->currentPath.c_str(), &opts);
            
            dup2(stdout_fd, STDOUT_FILENO);
            ::close(stdout_fd);
            fclose(temp);
            
            QFile tempFile(tempFileName);
            if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString content = QString::fromLocal8Bit(tempFile.readAll());
                resultText->setPlainText(content);
                tempFile.close();
            }
            
            QFile::remove(tempFileName);
        }
    };
    
    // 체크박스 상태 변경 시 자동 새로고침
    connect(timeSort, &QCheckBox::stateChanged, refreshListing);
    connect(sizeSort, &QCheckBox::stateChanged, refreshListing);
    connect(reverseSort, &QCheckBox::stateChanged, refreshListing);
    connect(showAllTimes, &QCheckBox::stateChanged, refreshListing);
    
    // 새로고침 버튼 클릭 시
    connect(refreshButton, &QPushButton::clicked, refreshListing);
    
    // 초기 목록 표시
    refreshListing();
    
    dialog->exec();
    delete dialog;
}

void MainWindowFileActions::handleMkdir(MainWindow* window)
{
    bool ok;
    QString folderName = QInputDialog::getText(window, QObject::tr("새 폴더 만들기"),
                                             QObject::tr("폴더 이름:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !folderName.isEmpty()) {
        call_mkdir(window->currentPath.c_str(), 
                  folderName.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::handleRm(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(window, QObject::tr("삭제 확인"),
        QObject::tr("선택한 항목을 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (const QModelIndex &index : selected) {
            QString path = window->fileSystemModel->filePath(index);
            call_rm(window->currentPath.c_str(),
                   path.toLocal8Bit().constData(),
                   QFileInfo(path).isDir());
        }
        refreshFileList(window);
    }
}

void MainWindowFileActions::handleChmod(MainWindow* window)
{
    qDebug() << "MainWindowFileActions::handleChmod called";
    
    QModelIndexList selected;
    if (window->treeView->hasFocus()) {
        selected = window->treeView->selectionModel()->selectedIndexes();
        qDebug() << "Using tree view selection";
    } else {
        selected = window->listView->selectionModel()->selectedIndexes();
        qDebug() << "Using list view selection";
    }
    
    if (selected.isEmpty()) {
        qDebug() << "No items selected";
        return;
    }

    QString path = window->fileSystemModel->filePath(selected.first());
    QString fileName = QFileInfo(path).fileName();
    
    bool ok;
    QString mode = QInputDialog::getText(window, QObject::tr("권한 변경"),
                                       QObject::tr("새 권한 (예: 755 또는 u+x):"),
                                       QLineEdit::Normal,
                                       QString(), &ok);
    
    if (ok && !mode.isEmpty()) {
        qDebug() << "Changing permissions of" << fileName << "to" << mode;
        call_chmod(window->currentPath.c_str(),
                  fileName.toLocal8Bit().constData(),
                  mode.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::handleCp(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString sourcePath = window->fileSystemModel->filePath(selected.first());
    QString targetPath = QFileDialog::getSaveFileName(window, QObject::tr("복사할 파일 선택"),
                                                    window->getCurrentDirectory());
    
    if (!targetPath.isEmpty()) {
        call_cp(window->currentPath.c_str(),
               sourcePath.toLocal8Bit().constData(),
               targetPath.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::handleRename(MainWindow* window)
{
    qDebug() << "MainWindowFileActions::handleRename called";
    
    QModelIndexList selected;
    if (window->treeView->hasFocus()) {
        selected = window->treeView->selectionModel()->selectedIndexes();
        qDebug() << "Using tree view selection";
    } else {
        selected = window->listView->selectionModel()->selectedIndexes();
        qDebug() << "Using list view selection";
    }
    
    if (selected.isEmpty()) {
        qDebug() << "No items selected";
        return;
    }

    QString oldPath = window->fileSystemModel->filePath(selected.first());
    QString oldName = QFileInfo(oldPath).fileName();
    
    bool ok;
    QString newName = QInputDialog::getText(window, QObject::tr("이름 변경"),
                                          QObject::tr("새 이름:"), QLineEdit::Normal,
                                          oldName, &ok);
    
    if (ok && !newName.isEmpty() && newName != oldName) {
        qDebug() << "Renaming from" << oldName << "to" << newName;
        call_rename(window->currentPath.c_str(),
                   oldName.toLocal8Bit().constData(),
                   newName.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::handleLn(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString targetPath = window->fileSystemModel->filePath(selected.first());
    QString linkPath = QFileDialog::getSaveFileName(window, QObject::tr("링크 생성 위치 선택"),
                                                  window->getCurrentDirectory());
    
    if (!linkPath.isEmpty()) {
        call_ln(window->currentPath.c_str(),
               targetPath.toLocal8Bit().constData(),
               linkPath.toLocal8Bit().constData(),
               1);
        refreshFileList(window);
    }
}

void MainWindowFileActions::refreshFileList(MainWindow* window)
{
    QString qPath = window->getCurrentDirectory();
    window->fileSystemModel->setRootPath(qPath);
    QModelIndex index = window->fileSystemModel->index(qPath);
    
    window->treeView->setRootIndex(index);
    window->listView->setRootIndex(index);
    
    // 선택 초기화
    window->treeView->clearSelection();
    window->listView->clearSelection();
    
    // 상태바 업데이트
    updateStatusBar(window);
}

void MainWindowFileActions::handleCat(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString filePath = window->fileSystemModel->filePath(selected.first());
    
    QDialog *viewDialog = new QDialog(window);
    viewDialog->setWindowTitle(QObject::tr("파일 내용 - %1").arg(QFileInfo(filePath).fileName()));
    viewDialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(viewDialog);
    QTextEdit *textEdit = new QTextEdit(viewDialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);
    
    QString tempFileName = QDir::temp().filePath("cat_output.tmp");
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    
    if (temp) {
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        call_cat(window->currentPath.c_str(),
                filePath.toLocal8Bit().constData());
        
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
        QFile tempFile(tempFileName);
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            textEdit->setPlainText(QString::fromLocal8Bit(tempFile.readAll()));
            tempFile.close();
        }
        
        QFile::remove(tempFileName);
    }
    
    viewDialog->exec();
    delete viewDialog;
}

void MainWindowFileActions::createNewFolder(MainWindow* window)
{
    bool ok;
    QString folderName = QInputDialog::getText(window, QObject::tr("새 폴더"), 
                                             QObject::tr("폴더 이름:"), QLineEdit::Normal,
                                             QString(), &ok);
    if (ok && !folderName.isEmpty()) {
        call_mkdir(window->currentPath.c_str(), 
                  folderName.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::deleteSelected(MainWindow* window)
{
    // 현재 선택된 항목들의 목록을 가져옴
    QModelIndexList selected;
    if (window->treeView->hasFocus()) {
        selected = window->treeView->selectionModel()->selectedIndexes();
    } else {
        selected = window->listView->selectionModel()->selectedIndexes();
    }
    
    if (selected.isEmpty()) {
        return;
    }

    // 사용자에게 삭제 확인 메시지 표시
    QMessageBox::StandardButton reply = QMessageBox::question(window, QObject::tr("삭제 확인"),
        QObject::tr("선택한 항목을 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // 선택된 각 항목에 대해 삭제 수행
        for (const QModelIndex &index : selected) {
            QString path = window->fileSystemModel->filePath(index);
            QString fileName = QFileInfo(path).fileName();
            
            if (!fileName.isEmpty()) {
                // 파일 시스템에서 실제 삭제 수행
                call_rm(window->currentPath.c_str(),
                       fileName.toLocal8Bit().constData(),
                       QFileInfo(path).isDir());
            }
        }
        // 파일 목록 갱신
        refreshFileList(window);
    }
}

void MainWindowFileActions::copySelected(MainWindow* window, bool isMove)
{
    QModelIndexList selected;
    if (window->treeView->hasFocus()) {
        selected = window->treeView->selectionModel()->selectedRows();
    } else {
        selected = window->listView->selectionModel()->selectedRows();
    }
    
    if (!selected.isEmpty()) {
        window->selectedIndexes.clear();  // 기존 선택 항목 초기화
        window->selectedIndexes = selected;
        window->pasteAction->setEnabled(true);
        window->moveOperation = isMove;  // 이동 작업 여부 설정
    }
}

void MainWindowFileActions::pasteToCurrentDir(MainWindow* window)
{
    bool isMove = window->moveOperation;  // 이동 작업 여부 확인
    
    for (const QModelIndex &sourceIndex : window->selectedIndexes) {
        QString sourcePath = window->fileSystemModel->filePath(sourceIndex);
        QString fileName = window->fileSystemModel->fileName(sourceIndex);
        QString destPath = QString::fromStdString(window->currentPath) + "/" + fileName;
        
        QFileInfo sourceInfo(sourcePath);
        if (sourceInfo.isDir()) {
            // 디렉토리인 경우 재귀적으로 복사
            copyDirectory(sourcePath, destPath);
            if (isMove) {
                // 이동 작업인 경우 원본 디렉토리 삭제
                QDir(sourcePath).removeRecursively();
            }
        } else {
            // 파일인 경우 call_cp 사용
            call_cp(window->currentPath.c_str(),
                   sourcePath.toLocal8Bit().constData(),
                   destPath.toLocal8Bit().constData());
            
            if (isMove) {
                // 이동 작업인 경우 원본 파일 삭제
                call_rm(QFileInfo(sourcePath).path().toLocal8Bit().constData(),
                       fileName.toLocal8Bit().constData(),
                       false);  // false는 디렉토리가 아님을 의미
            }
        }
    }
    
    // 복사/이동 완료 후 화면 갱신
    refreshFileList(window);
    window->selectedIndexes.clear();  // 선택 항목 초기화
    window->pasteAction->setEnabled(false);
    window->moveOperation = false;  // 이동 작업 플래그 초기화
}

void MainWindowFileActions::updateStatusBar(MainWindow* window)
{
    QDir currentDir(window->getCurrentDirectory());
    qint64 totalSize = 0;
    int fileCount = 0;
    int dirCount = 0;
    
    QFileInfoList entries = currentDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &info : entries) {
        if (info.isFile()) {
            totalSize += info.size();
            fileCount++;
        } else if (info.isDir()) {
            dirCount++;
        }
    }
    
    QString status = QObject::tr("파일 %1개, 디렉토리 %2개, 총 크기: %3")
                    .arg(fileCount)
                    .arg(dirCount)
                    .arg(formatSize(totalSize));
    window->statusBar()->showMessage(status);
}

QString MainWindowFileActions::formatSize(qint64 size)
{
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double dsize = size;
    
    while (dsize >= 1024.0 && unit < 4) {
        dsize /= 1024.0;
        unit++;
    }
    
    return QString("%1 %2").arg(dsize, 0, 'f', 1).arg(units[unit]);
}

void MainWindowFileActions::showContextMenu(MainWindow* window, const QPoint &pos)
{
    qDebug() << "Context menu requested at position:" << pos;
    
    // 현재 마우스 위의 항목 선택
    QWidget* sender = qobject_cast<QWidget*>(window->sender());
    if (QTreeView* treeView = qobject_cast<QTreeView*>(sender)) {
        QModelIndex index = treeView->indexAt(pos);
        if (index.isValid()) {
            treeView->setCurrentIndex(index);
        }
    } else if (QListView* listView = qobject_cast<QListView*>(sender)) {
        QModelIndex index = listView->indexAt(pos);
        if (index.isValid()) {
            listView->setCurrentIndex(index);
        }
    }
    
    QMenu contextMenu(window);
    contextMenu.addAction(window->newFolderAction);
    contextMenu.addSeparator();
    contextMenu.addAction(window->deleteAction);
    contextMenu.addSeparator();
    contextMenu.addAction(window->copyAction);
    contextMenu.addAction(window->pasteAction);
    contextMenu.addSeparator();
    contextMenu.addAction(window->renameAction);
    contextMenu.addAction(window->chmodAction);
    
    qDebug() << "Context menu created with actions";
    contextMenu.exec(window->mapToGlobal(pos));
}

void MainWindowFileActions::showFileDetails(MainWindow* window, const QString &fileName)
{
    QString filePath = window->getCurrentDirectory() + "/" + fileName;
    QFileInfo fileInfo(filePath);
    
    QString details = QString(
        "이름: %1\n"
        "기: %2\n"
        "��유자: %3\n"
        "그룹: %4\n"
        "권한: %5\n"
        "수정일: %6")
        .arg(fileInfo.fileName())
        .arg(formatSize(fileInfo.size()))
        .arg(fileInfo.owner())
        .arg(fileInfo.group())
        .arg(fileInfo.permissions())
        .arg(fileInfo.lastModified().toString());
    
    QMessageBox::information(window, QObject::tr("파일 정보"), details);
}

void MainWindowFileActions::handleRmdir(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QString path = window->fileSystemModel->filePath(selected.first());
    QString fileName = QFileInfo(path).fileName();

    // 선택된 항목이 디렉토리인지 확인
    if (!QFileInfo(path).isDir()) {
        QMessageBox::warning(window, QObject::tr("오류"),
                           QObject::tr("선택한 항목이 디렉토가 아닙니다."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(window, QObject::tr("디렉토리 삭제"),
        QObject::tr("선택한 디렉토리를 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        call_rmdir(window->currentPath.c_str(), 
                  fileName.toLocal8Bit().constData());
        refreshFileList(window);
    }
}

void MainWindowFileActions::copyDirectory(const QString &sourcePath, const QString &destPath)
{
    QDir sourceDir(sourcePath);
    QDir destDir(destPath);
    
    // 대상 디렉토리가 없으면 생성
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }

    // 디렉토리 내용물 복사
    QFileInfoList entries = sourceDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        QString srcPath = entry.filePath();
        QString destFilePath = destDir.filePath(entry.fileName());
        
        if (entry.isDir()) {
            // 디렉토리인 경우 재귀적으로 복사
            copyDirectory(srcPath, destFilePath);
        } else {
            // 파일인 경우 call_cp 사용
            call_cp(QFileInfo(destFilePath).path().toLocal8Bit().constData(),
                   srcPath.toLocal8Bit().constData(),
                   destFilePath.toLocal8Bit().constData());
        }
    }
}

bool MainWindowFileActions::removeDirectory(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) return true;

    bool success = true;
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            success = removeDirectory(entry.filePath());
        } else {
            success = QFile::remove(entry.filePath());
        }
        
        if (!success) return false;
    }

    return dir.rmdir(dirPath);
}