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
#include <QDebug>
#include <QHeaderView>

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
    // 파일   및 설정
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    fileSystemModel->setReadOnly(false);  // 편집 가능하도록 설정

    // 뷰 생성
    QSplitter *splitter = new QSplitter(this);
    treeView = new QTreeView(splitter);
    listView = new QListView(splitter);

    // 모델 설정
    treeView->setModel(fileSystemModel);
    listView->setModel(fileSystemModel);

    // 선택 모드 설정
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 초기 디렉토리 설정
    QString rootPath = QString::fromStdString(currentPath);
    fileSystemModel->setRootPath(rootPath);
    QModelIndex rootIndex = fileSystemModel->index(rootPath);
    
    treeView->setRootIndex(rootIndex);
    listView->setRootIndex(rootIndex);

    // 불필요한 열 숨기기
    treeView->hideColumn(1); // Size
    treeView->hideColumn(2); // Type
    treeView->hideColumn(3); // Modified Date

    setCentralWidget(splitter);
    statusBar()->showMessage(tr("준비"));

    // 선택 변경 시그널 연결
    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::handleSelectionChanged);

    // 컨텍스트 메뉴 활성화
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    listView->setContextMenuPolicy(Qt::CustomContextMenu);

    // 컨텍스트 메뉴 시그널 연결
    connect(treeView, &QTreeView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);
    connect(listView, &QListView::customContextMenuRequested,
            this, &MainWindow::showContextMenu);
}

void MainWindow::createActions()
{
    // 파일 관련 액션들
    newFolderAction = new QAction(QIcon::fromTheme("folder-new"), tr("새 폴더"), this);
    deleteAction = new QAction(QIcon::fromTheme("edit-delete"), tr("삭제"), this);
    copyAction = new QAction(QIcon::fromTheme("edit-copy"), tr("복사"), this);
    pasteAction = new QAction(QIcon::fromTheme("edit-paste"), tr("붙여넣기"), this);
    renameAction = new QAction(QIcon::fromTheme("edit-rename"), tr("이름 변경"), this);
    chmodAction = new QAction(QIcon::fromTheme("document-properties"), tr("권한 변경"), this);
    exitAction = new QAction(QIcon::fromTheme("application-exit"), tr("종료"), this);

    // ls 액 추가
    lsAction = new QAction(QIcon::fromTheme("view-list-details"), tr("상세 보기"), this);
    lsAction->setStatusTip(tr("파일 목록 상세 보기"));
    connect(lsAction, &QAction::triggered, this, &MainWindow::handleLs);

    // rmdir 액션 추가
    rmdirAction = new QAction(QIcon::fromTheme("folder-remove"), tr("디렉토리 삭제"), this);
    rmdirAction->setStatusTip(tr("빈 디렉토리 삭제"));
    connect(rmdirAction, &QAction::triggered, this, &MainWindow::handleRmdir);

    // 액션들  상태 팁 설정
    newFolderAction->setStatusTip(tr("새 폴더 만들기"));
    deleteAction->setStatusTip(tr("선택한 항목 삭제"));
    copyAction->setStatusTip(tr("선택한 항목 복사"));
    pasteAction->setStatusTip(tr("복사한 항목 붙여넣기"));
    renameAction->setStatusTip(tr("선택한 항목의 이름 변경"));
    chmodAction->setStatusTip(tr("선택한 항목의 권한 변경"));
    exitAction->setStatusTip(tr("프로그램 종료"));

    // 단축키 설정
    exitAction->setShortcut(tr("Ctrl+Q"));

    // 시그널 연결
    connect(newFolderAction, &QAction::triggered, this, &MainWindow::createNewFolder);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelected);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copySelected);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::pasteToCurrentDir);
    connect(renameAction, &QAction::triggered, this, &MainWindow::handleRename);
    connect(chmodAction, &QAction::triggered, this, &MainWindow::handleChmod);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 초기 태 정
    deleteAction->setEnabled(false);
    copyAction->setEnabled(false);
    pasteAction->setEnabled(false);
    renameAction->setEnabled(false);
    chmodAction->setEnabled(false);

    // 프로세스 관련 액션 추가
    psAction = new QAction(QIcon::fromTheme("system-run"), tr("프로세스 목록"), this);
    psAction->setStatusTip(tr("실행 중인 프로세스 목록 보기"));
    connect(psAction, &QAction::triggered, this, &MainWindow::handlePs);

    killAction = new QAction(QIcon::fromTheme("process-stop"), tr("프로세스 종료"), this);
    killAction->setStatusTip(tr("프로세스 종료(kill)"));
    connect(killAction, &QAction::triggered, this, [this]() {
        handleKill(QString());  // 빈 PID로 호출하면 사용자에게 입력받음
    });
}

void MainWindow::createToolBar()
{
    fileToolBar = addToolBar(tr("파일"));
    fileToolBar->addAction(newFolderAction);
    fileToolBar->addAction(deleteAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(copyAction);
    fileToolBar->addAction(pasteAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(renameAction);
    fileToolBar->addAction(chmodAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(lsAction);  // ls 액션 추가
    fileToolBar->addSeparator();
    fileToolBar->addAction(rmdirAction);  // 툴바에 추가
    fileToolBar->addSeparator();
    fileToolBar->addAction(exitAction);

    // 프로세스 관련 툴바 추가
    QToolBar *processToolBar = addToolBar(tr("프로세스"));
    processToolBar->addAction(psAction);
    processToolBar->addAction(killAction);
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
    fileMenu->addSeparator();
    fileMenu->addAction(renameAction);
    fileMenu->addAction(chmodAction);
    fileMenu->addSeparator();
    fileMenu->addAction(lsAction);  // ls 액션 가
    fileMenu->addSeparator();
    fileMenu->addAction(rmdirAction);  // 메뉴에 추가
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // 프로세스 메뉴 추가
    QMenu *processMenu = menuBar()->addMenu(tr("프로세스(&P)"));
    processMenu->addAction(psAction);
    processMenu->addAction(killAction);
}

void MainWindow::handleSelectionChanged()
{
    // 현재 포커스를 가진 뷰의 선택 모델 가져오기
    QItemSelectionModel *selectionModel = nullptr;
    if (treeView->hasFocus()) {
        selectionModel = treeView->selectionModel();
    } else if (listView->hasFocus()) {
        selectionModel = listView->selectionModel();
    }

    bool hasSelection = selectionModel && selectionModel->hasSelection();

    // 액션 활성화/비활성화
    deleteAction->setEnabled(hasSelection);
    copyAction->setEnabled(hasSelection);
    renameAction->setEnabled(hasSelection);
    chmodAction->setEnabled(hasSelection);

    // 상태바 업데이트
    if (hasSelection) {
        QModelIndexList selectedIndexes = selectionModel->selectedRows();
        if (selectedIndexes.isEmpty()) {
            selectedIndexes = selectionModel->selectedIndexes();
        }
        QString statusMessage = tr("%1개 항목 선택됨").arg(selectedIndexes.count());
        statusBar()->showMessage(statusMessage);
        
        // 디버그 메시지 추가
        qDebug() << "Selected file:" << fileSystemModel->fileName(selectedIndexes.first());
    } else {
        statusBar()->clearMessage();
    }
}

void MainWindow::handleLs()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("디렉토리 상세 정보"));
    dialog->resize(800, 600);  // 더 큰 창으로 수정
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // 현재 경로 표시
    QLabel *pathLabel = new QLabel(tr("현재 경로: %1").arg(QString::fromStdString(currentPath)));
    layout->addWidget(pathLabel);
    
    // 구분선 추가
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
    
    // 옵션 체크박스들
    QHBoxLayout *optionsLayout = new QHBoxLayout();
    QCheckBox *timeSort = new QCheckBox(tr("-t (시간순 정렬)"));
    QCheckBox *sizeSort = new QCheckBox(tr("-S (크기순 정렬)"));
    QCheckBox *reverseSort = new QCheckBox(tr("-r (역순 정렬)"));
    QCheckBox *showAllTimes = new QCheckBox(tr("-T (모든 시간 표시)"));
    
    optionsLayout->addWidget(timeSort);
    optionsLayout->addWidget(sizeSort);
    optionsLayout->addWidget(reverseSort);
    optionsLayout->addWidget(showAllTimes);
    layout->addLayout(optionsLayout);
    
    // 결과를 표시할 텍스트 에디터
    QTextEdit *resultText = new QTextEdit(dialog);
    resultText->setReadOnly(true);
    resultText->setFont(QFont("Monospace"));  // 고정폭 폰트 사용
    layout->addWidget(resultText);
    
    // 새로고침 버튼
    QPushButton *refreshButton = new QPushButton(tr("새로고침"), dialog);
    layout->addWidget(refreshButton);
    
    // 새로고침 함수 정의
    auto refreshListing = [=]() {
        // 임시 파일 생성
        QString tempFileName = QDir::temp().filePath("ls_output.tmp");
        FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
        
        if (temp) {
            // 표준 출력을 임시 파일로 리디렉션
            int stdout_fd = dup(STDOUT_FILENO);
            dup2(fileno(temp), STDOUT_FILENO);
            
            // ls 명령 실행
            struct ls_options opts = {
                timeSort->isChecked(),     // sort_by_time
                sizeSort->isChecked(),     // sort_by_size
                reverseSort->isChecked(),  // reverse_sort
                showAllTimes->isChecked()  // show_all_times
            };
            
            call_ls(currentPath.c_str(), &opts);
            
            // 표준 출력 복구
            dup2(stdout_fd, STDOUT_FILENO);
            ::close(stdout_fd);
            fclose(temp);
            
            // 결과 읽기
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
        tr("선택한 ���목을 삭제하시겠습니까?"),
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
    if (selected.isEmpty()) {
        selected = treeView->selectionModel()->selectedRows();
        if (selected.isEmpty()) return;
    }

    QString path = fileSystemModel->filePath(selected.first());
    QString fileName = QFileInfo(path).fileName();
    
    bool ok;
    QString mode = QInputDialog::getText(this, tr("권한 변경"),
                                       tr("새 권한 (예: 755 또는 u+x):"),
                                       QLineEdit::Normal,
                                       QString(), &ok);
    
    if (ok && !mode.isEmpty()) {
        // 디버그 출력 추가
        qDebug() << "Current Path:" << currentPath.c_str();
        qDebug() << "File Name:" << fileName;
        qDebug() << "New Mode:" << mode;
        
        // call_chmod 함수 호출
        int result = call_chmod(currentPath.c_str(),
                              fileName.toLocal8Bit().constData(),
                              mode.toLocal8Bit().constData());
        
        if (result == 0) {
            statusBar()->showMessage(tr("파일 권한이 변경되었습니다."), 3000);
        } else {
            QMessageBox::warning(this, tr("오류"),
                               tr("권한을 변경할 수 없습니다."));
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
    if (selected.isEmpty()) {
        selected = treeView->selectionModel()->selectedRows();
        if (selected.isEmpty()) return;
    }

    QString oldPath = fileSystemModel->filePath(selected.first());
    QString oldName = QFileInfo(oldPath).fileName();
    
    bool ok;
    QString newName = QInputDialog::getText(this, tr("이름 변경"),
                                          tr("새 이:"), QLineEdit::Normal,
                                          oldName, &ok);
    
    if (ok && !newName.isEmpty() && newName != oldName) {
        // 디버그 출력 추가
        qDebug() << "Current Path:" << currentPath.c_str();
        qDebug() << "Old Name:" << oldName;
        qDebug() << "New Name:" << newName;
        
        // call_rename 함수 호출
        int result = call_rename(currentPath.c_str(), 
                               oldName.toLocal8Bit().constData(),
                               newName.toLocal8Bit().constData());
        
        if (result == 0) {
            statusBar()->showMessage(tr("파일 이름이 변경되었습니다."), 3000);
        } else {
            QMessageBox::warning(this, tr("오류"),
                               tr("이름을 변경할 수 없습니다."));
        }
        
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
    
    // 파일 용을 보여줄  다이얼로그 생성
    QDialog *viewDialog = new QDialog(this);
    viewDialog->setWindowTitle(tr("파일 내용 - %1").arg(QFileInfo(filePath).fileName()));
    viewDialog->resize(600, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(viewDialog);
    QTextEdit *textEdit = new QTextEdit(viewDialog);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);
    
    // cat 명령 실 결과를 캡처하기 위한 임시 파일 사용
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
        
        // 임시 파일에 용 읽기
        QFile tempFile(tempFileName);
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            textEdit->setPlainText(QString::fromLocal8Bit(tempFile.readAll()));
            tempFile.close();
        }
        
        // 시 파일 삭제
        QFile::remove(tempFileName);
    }
    
    viewDialog->exec();
    delete viewDialog;
}

void MainWindow::handlePs()
{
    QDialog *psDialog = new QDialog(this);
    psDialog->setWindowTitle(tr("프로세스 목록"));
    psDialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(psDialog);
    QTableWidget *processTable = new QTableWidget(psDialog);
    layout->addWidget(processTable);
    
    // 프로세스 목록 표시 함수 호출
    refreshProcessList(processTable);
    
    // 프로세스 종료 버튼과 새로고침 버튼 추가
    QPushButton *killButton = new QPushButton(tr("프로세스 종료"), psDialog);
    QPushButton *refreshButton = new QPushButton(tr("새로고침"), psDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(killButton);
    buttonLayout->addWidget(refreshButton);
    layout->addLayout(buttonLayout);
    
    // 프로세스 종료 버튼 클릭 시
    connect(killButton, &QPushButton::clicked, [this, processTable]() {
        QList<QTableWidgetItem*> selectedItems = processTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            QString pid = processTable->item(row, 1)->text();
            
            // 현재 프로세스의 PID와 비교
            if (pid.toInt() == getpid()) {
                QMessageBox::warning(this, tr("오류"),
                                   tr("현재 실행 중인 프로그램은 종료할 수 없습니다."));
                return;
            }
            
            bool ok;
            QString signal = QInputDialog::getText(this, tr("시그널 전송"),
                                                tr("PID %1에 전송할 시그널 번호 (기본: 9):").arg(pid),
                                                QLineEdit::Normal,
                                                "9", &ok);
            if (ok) {
                // call_kill 함수 호출 및 반환값 확인
                if (call_kill(pid.toLocal8Bit().constData(),
                             signal.toLocal8Bit().constData()) == 0) {
                    QMessageBox::information(this, tr("성공"),
                                          tr("프로세스가 종료되었습니다."));
                    // 프로세스 목록 갱신
                    refreshProcessList(processTable);
                } else {
                    QMessageBox::warning(this, tr("오류"),
                                      tr("프로세스를 종료할 수 없습니다."));
                }
            }
        }
    });
    
    // 새로고침 버튼 클릭 시
    connect(refreshButton, &QPushButton::clicked, [this, processTable]() {
        refreshProcessList(processTable);
    });
    
    psDialog->exec();
    delete psDialog;
}

void MainWindow::refreshProcessList(QTableWidget *processTable)
{
    // 테이블 초기화
    processTable->setRowCount(0);
    
    // 임시 파일 생성
    QString tempFileName = QDir::temp().filePath("ps_output.tmp");
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    
    if (temp) {
        // 표준 출력을 임시 파일로 리디렉션
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        // ps 명령 실행
        call_ps("-ef");
        
        // 표준 출력 복구
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
        // ��과를 테이블에 표시
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
    
    // 각 열의 너비비 자동 조정
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::handleKill(const QString &pid)
{
    bool ok;
    QString signal = QInputDialog::getText(this, tr("시그널 전송"),
                                         tr("PID %1에 전송할 시그널 번호 (기본: 15):").arg(pid),
                                         QLineEdit::Normal,
                                         "15", &ok);
    if (ok) {
        call_kill(pid.toLocal8Bit().constData(),
                 signal.toLocal8Bit().constData());
    }
}

void MainWindow::refreshFileList()
{
    QString qPath = QString::fromStdString(currentPath);
    fileSystemModel->setRootPath(qPath);
    QModelIndex index = fileSystemModel->index(qPath);
    
    treeView->setRootIndex(index);
    listView->setRootIndex(index);
    
    // 선택 초기화
    treeView->clearSelection();
    listView->clearSelection();
    
    // 상태바 업데이트
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
    QString status = tr("파일 %1, 디렉토리 %2개, 총 크기: %3")
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
        call_mkdir(currentPath.c_str(), 
                  folderName.toLocal8Bit().constData());
        refreshFileList();
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
            QString fileName = QFileInfo(path).fileName();
            
            call_rm(currentPath.c_str(),
                   fileName.toLocal8Bit().constData(),
                   QFileInfo(path).isDir()); // 디렉토리면 recursive true
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
        
        call_cp(currentPath.c_str(),
                sourcePath.toLocal8Bit().constData(),
                destPath.toLocal8Bit().constData());
    }
    refreshFileList();
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

// 컨텍스트 메뉴 표시 함수 추가
void MainWindow::showContextMenu(const QPoint &pos)
{
    QWidget *widget = qobject_cast<QWidget*>(sender());
    QModelIndex index;
    
    if (QTreeView *tree = qobject_cast<QTreeView*>(widget)) {
        index = tree->indexAt(pos);
    } else if (QListView *list = qobject_cast<QListView*>(widget)) {
        index = list->indexAt(pos);
    }

    if (!index.isValid()) return;

    QString filePath = fileSystemModel->filePath(index);
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    QMenu contextMenu(this);
    
    // 파일 상세 정보 보기 액션
    QAction *detailsAction = contextMenu.addAction(tr("상세 정보"));
    contextMenu.addSeparator();
    
    // 기본 파일 작업 메뉴 추가
    contextMenu.addAction(renameAction);
    contextMenu.addAction(chmodAction);
    contextMenu.addAction(deleteAction);
    
    // 메뉴 표시 및 액션 처리
    QAction *action = contextMenu.exec(widget->mapToGlobal(pos));
    
    if (action == detailsAction) {
        showFileDetails(fileName);
    }
}

// 파일 상세 정보 표 함수 추가
void MainWindow::showFileDetails(const QString &fileName)
{
    // 파일의 절대 경로 얻기
    QString filePath = QString::fromStdString(currentPath) + "/" + fileName;
    
    // 결과를 대화상자로 표시
    QDialog dialog(this);
    dialog.setWindowTitle(tr("파일 상세 정보 - %1").arg(fileName));
    dialog.resize(800, 400);  // 창 크기 증가
    
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    
    // 절대 경로 표시
    QLabel *pathLabel = new QLabel(tr("절대 경로: %1").arg(filePath));
    layout->addWidget(pathLabel);
    
    // 구분선 추가
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
    
    // 파일 상세 정보를 표시할 텍스트 에디터
    QTextEdit *detailsText = new QTextEdit(&dialog);
    detailsText->setReadOnly(true);
    detailsText->setFont(QFont("Monospace"));  // 고정폭 폰트 사용
    layout->addWidget(detailsText);

    // 임시 파일 생성
    QString tempFileName = QDir::temp().filePath("ls_details.tmp");
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    
    if (temp) {
        // 표준 출력을 임시 파일로 리디렉션
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        // ls 명령 실행 (모든 옵션 활성화)
        struct ls_options opts = {
            1,  // sort_by_time
            1,  // sort_by_size
            0,  // reverse_sort
            1,  // show_all_times
            1   // long_format
        };
        
        call_ls(currentPath.c_str(), &opts);
        
        // 표준 출력 복구
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
        // 임시 파일에서 결과 읽기
        QFile tempFile(tempFileName);
        if (tempFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = QString::fromLocal8Bit(tempFile.readAll());
            QStringList lines = content.split('\n', Qt::SkipEmptyParts);
            
            // 해당 파일에 대한 정보만 찾기
            for (const QString &line : lines) {
                if (line.endsWith(fileName) || line.endsWith(" " + fileName) || 
                    line.endsWith("/" + fileName) || line.contains(" " + fileName + " -> ")) {
                    detailsText->setPlainText(line);
                    break;
                }
            }
            tempFile.close();
        }
        
        // 임시 파일 삭제
        QFile::remove(tempFileName);
    }
    
    // 확인 버튼 추가
    QPushButton *okButton = new QPushButton(tr("확인"), &dialog);
    layout->addWidget(okButton);
    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    
    dialog.exec();
}

void MainWindow::handleRmdir()
{
    QModelIndexList selected = listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        selected = treeView->selectionModel()->selectedRows();
        if (selected.isEmpty()) return;
    }

    QString path = fileSystemModel->filePath(selected.first());
    QString fileName = QFileInfo(path).fileName();

    // 선택된 항목이 디렉토리인지 확인
    if (!QFileInfo(path).isDir()) {
        QMessageBox::warning(this, tr("오류"),
                           tr("선택한 항목이 디렉토리가 아닙니다."));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("디렉토리 삭제"),
        tr("선택한 디렉토리를 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // call_rmdir 함수 호출
        call_rmdir(currentPath.c_str(), 
                  fileName.toLocal8Bit().constData());
        
        refreshFileList();
    }
}
  