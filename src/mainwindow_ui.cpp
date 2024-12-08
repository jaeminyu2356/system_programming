#include <QtWidgets>
#include "../include/mainwindow_ui.h"
#include "../include/mainwindow_file_actions.h"
#include "../include/config.h"

MainWindowUI::MainWindowUI(QObject *parent) : QObject(parent) {}

void MainWindowUI::setupUI(MainWindow* window)
{
    // 인 레이아웃 설정
    QWidget *centralWidget = new QWidget(window);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 왼쪽 패널: 파일 탐색기
    QVBoxLayout *leftPanelLayout = new QVBoxLayout();
    
    // 검색 위젯 추가
    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLineEdit *searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText(QObject::tr("파일 검색..."));
    QPushButton *searchButton = new QPushButton(QObject::tr("검색"));
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    leftPanelLayout->addLayout(searchLayout);

    // 파일 시스템 모델 설정
    window->fileSystemModel = new QFileSystemModel(window);
    window->fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    window->fileSystemModel->setReadOnly(false);

    // 트리 ���를 포함하는 스플리터 생성
    QSplitter *viewSplitter = new QSplitter(Qt::Vertical);
    window->treeView = new QTreeView(viewSplitter);
    window->listView = new QListView(viewSplitter);

    // 트리뷰 설정
    window->treeView->setModel(window->fileSystemModel);
    window->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    window->treeView->setDragEnabled(true);
    window->treeView->setAcceptDrops(true);
    window->treeView->setDropIndicatorShown(true);
    window->treeView->setDragDropMode(QAbstractItemView::DragDrop);
    window->treeView->hideColumn(1); // 크기 열 숨기기
    window->treeView->hideColumn(2); // 유형 열 숨기기
    window->treeView->hideColumn(3); // 수정 날짜 열 숨기기

    // 리스트뷰 설정
    window->listView->setModel(window->fileSystemModel);
    window->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    window->listView->setDragEnabled(true);
    window->listView->setAcceptDrops(true);
    window->listView->setDropIndicatorShown(true);
    window->listView->setDragDropMode(QAbstractItemView::DragDrop);
    window->listView->setViewMode(QListView::IconMode);
    window->listView->setGridSize(QSize(80, 80));
    window->listView->setSpacing(10);
    window->listView->setResizeMode(QListView::Adjust);

    viewSplitter->addWidget(window->treeView);
    viewSplitter->addWidget(window->listView);
    leftPanelLayout->addWidget(viewSplitter);

    // 디스크 사용량 표시 위젯
    QProgressBar *diskUsageBar = new QProgressBar();
    QStorageInfo storage(QString::fromStdString(window->currentPath));
    qint64 available = storage.bytesAvailable();
    qint64 total = storage.bytesTotal();
    int percentage = (int)(100.0 * (total - available) / total);
    diskUsageBar->setValue(percentage);
    diskUsageBar->setFormat(QObject::tr("디스크 사용량: %p% (%1 중 %2 사용)")
        .arg(MainWindowFileActions::formatSize(total))
        .arg(MainWindowFileActions::formatSize(total - available)));
    leftPanelLayout->addWidget(diskUsageBar);

    // 파일 미보기 패널 추가
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightPanelLayout = new QVBoxLayout(rightPanel);
    
    // 미리보기 영역
    QLabel *previewLabel = new QLabel(QObject::tr("미리보기"));
    rightPanelLayout->addWidget(previewLabel);
    
    QTextEdit *previewText = new QTextEdit();
    previewText->setReadOnly(true);
    rightPanelLayout->addWidget(previewText);

    // 파일 정보 표시 영역
    QGroupBox *infoGroup = new QGroupBox(QObject::tr("파일 정보"));
    QFormLayout *infoLayout = new QFormLayout();
    QLabel *nameLabel = new QLabel();
    QLabel *sizeLabel = new QLabel();
    QLabel *typeLabel = new QLabel();
    QLabel *modifiedLabel = new QLabel();
    infoLayout->addRow(QObject::tr("이름:"), nameLabel);
    infoLayout->addRow(QObject::tr("크기:"), sizeLabel);
    infoLayout->addRow(QObject::tr("유형:"), typeLabel);
    infoLayout->addRow(QObject::tr("수정일:"), modifiedLabel);
    infoGroup->setLayout(infoLayout);
    rightPanelLayout->addWidget(infoGroup);

    // 메인 스플리터에 패널 추가
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    QWidget *leftPanel = new QWidget();
    leftPanel->setLayout(leftPanelLayout);
    mainSplitter->addWidget(leftPanel);
    mainSplitter->addWidget(rightPanel);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 1);
    
    mainLayout->addWidget(mainSplitter);
    window->setCentralWidget(centralWidget);

    // 파일 선택 시 미리보기 업데이트
    QObject::connect(window->listView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [window, previewText, nameLabel, sizeLabel, typeLabel, modifiedLabel](const QItemSelection &selected) {
            if (selected.indexes().isEmpty()) {
                previewText->clear();
                nameLabel->clear();
                sizeLabel->clear();
                typeLabel->clear();
                modifiedLabel->clear();
                return;
            }

            QModelIndex index = selected.indexes().first();
            QString filePath = window->fileSystemModel->filePath(index);
            QFileInfo fileInfo(filePath);

            // 파일 정보 업데이트
            nameLabel->setText(fileInfo.fileName());
            sizeLabel->setText(MainWindowFileActions::formatSize(fileInfo.size()));
            typeLabel->setText(window->fileSystemModel->type(index));
            modifiedLabel->setText(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));

            // 텍스트 파일인 경우 내용 미리보기
            if (fileInfo.isFile()) {
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly)) {
                    // 바이너리 파인지 확인
                    QByteArray header = file.read(4096);
                    bool isBinary = false;
                    for (char c : header) {
                        if (c == 0 || (unsigned char)c > 127) {
                            isBinary = true;
                            break;
                        }
                    }

                    if (!isBinary && fileInfo.size() < 1024 * 1024) { // 1MB 미만 텍스트 파일만
                        file.seek(0);
                        QTextStream in(&file);
                        previewText->setPlainText(in.read(4096)); // 최대 4KB까지만 읽기
                    } else {
                        previewText->setPlainText(QObject::tr("바이너리 파일은 미리보기를 지원하지 않습니다."));
                    }
                    file.close();
                } else {
                    previewText->setPlainText(QObject::tr("파일을 열 수 없습니다."));
                }
            } else if (fileInfo.isDir()) {
                // 디렉토리 내용 표시
                QDir dir(filePath);
                QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
                previewText->setPlainText(QObject::tr("디디디렉토리 내용:\n\n") + entries.join("\n"));
            }
    });

    // 검색 기능 구현
    QObject::connect(searchButton, &QPushButton::clicked, [window, searchEdit]() {
        QString searchText = searchEdit->text();
        if (searchText.isEmpty()) {
            window->statusBar()->showMessage(QObject::tr("검색어를 입력하세요."), 3000);
            return;
        }

        // 현재 디렉토리에서 검색 시작
        QString currentDir = QString::fromStdString(window->currentPath);
        QDirIterator it(currentDir, 
                       QStringList() << "*" + searchText + "*",
                       QDir::AllEntries | QDir::NoDotAndDotDot,
                       QDirIterator::Subdirectories);

        QStringList foundFiles;
        while (it.hasNext()) {
            foundFiles << it.next();
        }

        if (!foundFiles.isEmpty()) {
            // 첫 번째 검색 결과로 이동
            QModelIndex index = window->fileSystemModel->index(foundFiles.first());
            window->listView->setCurrentIndex(index);
            window->listView->scrollTo(index);
            
            // 검색 결과 표시
            window->statusBar()->showMessage(
                QObject::tr("'%1' 검색 결과: %2개 항목 발견")
                .arg(searchText)
                .arg(foundFiles.size()), 
                3000
            );
        } else {
            window->statusBar()->showMessage(
                QObject::tr("'%1'에 대한 검색 결과가 없습니다.")
                .arg(searchText), 
                3000
            );
        }
    });

    // Enter 키로도 검색 가능하도록 설정
    QObject::connect(searchEdit, &QLineEdit::returnPressed, searchButton, &QPushButton::click);

    // 초기 상태 설정
    window->statusBar()->showMessage(QObject::tr("준비"));

    // 컨텍스트 메뉴 활성화
    window->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    window->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    qDebug() << "Context menu policy set for views";

    // 컨텍스트 메뉴 시그널 연결
    QObject::connect(window->treeView, &QTreeView::customContextMenuRequested,
            [window](const QPoint &pos) {
        qDebug() << "Tree view context menu requested";
        MainWindowFileActions::showContextMenu(window, pos);
    });

    QObject::connect(window->listView, &QListView::customContextMenuRequested,
            [window](const QPoint &pos) {
        qDebug() << "List view context menu requested";
        MainWindowFileActions::showContextMenu(window, pos);
    });

    // 더블클릭으로 디렉토리 진입
    QObject::connect(window->listView, &QListView::doubleClicked,
            [window](const QModelIndex &index) {
        QString path = window->fileSystemModel->filePath(index);
        if (QFileInfo(path).isDir()) {
            window->setCurrentDirectory(path.toStdString(), true);  // true로 히스토리에 추가
        }
    });

    QObject::connect(window->treeView, &QTreeView::doubleClicked,
            [window](const QModelIndex &index) {
        QString path = window->fileSystemModel->filePath(index);
        if (QFileInfo(path).isDir()) {
            window->setCurrentDirectory(path.toStdString(), true);  // true로 히스토리에 추가
        }
    });
}

void MainWindowUI::createActions(MainWindow* window)
{
    // 파일 관련 액션들
    window->newFolderAction = new QAction(QIcon::fromTheme("folder-new"), QObject::tr("새 폴더"), window);
    window->deleteAction = new QAction(QIcon::fromTheme("edit-delete"), QObject::tr("삭제"), window);
    window->copyAction = new QAction(QIcon::fromTheme("edit-copy"), QObject::tr("복사"), window);
    window->pasteAction = new QAction(QIcon::fromTheme("edit-paste"), QObject::tr("붙여넣기"), window);
    window->renameAction = new QAction(QIcon::fromTheme("edit-rename"), QObject::tr("이름 변경"), window);
    window->chmodAction = new QAction(QIcon::fromTheme("document-properties"), QObject::tr("권한 변경"), window);
    window->exitAction = new QAction(QIcon::fromTheme("application-exit"), QObject::tr("종료"), window);

    // ls 액션 추가
    window->lsAction = new QAction(QIcon::fromTheme("view-list-details"), QObject::tr("상세 보기"), window);
    window->lsAction->setStatusTip(QObject::tr("파일 목록 상세 보기"));

    // rmdir 액션 추가
    window->rmdirAction = new QAction(QIcon::fromTheme("folder-remove"), QObject::tr("디렉토리 삭제"), window);
    window->rmdirAction->setStatusTip(QObject::tr("빈 디렉토리 삭제"));

    // 액션들 상태 팁 설정
    window->newFolderAction->setStatusTip(QObject::tr("새 폴더 만들기"));
    window->deleteAction->setStatusTip(QObject::tr("선택한 항목 삭제"));
    window->copyAction->setStatusTip(QObject::tr("선택한 항목 복사"));
    window->pasteAction->setStatusTip(QObject::tr("복사한 항목 붙여넣기"));
    window->renameAction->setStatusTip(QObject::tr("선택한 항목의 이름 변경"));
    window->chmodAction->setStatusTip(QObject::tr("선택한 항목의 권한 변경"));
    window->exitAction->setStatusTip(QObject::tr("프로그램 종료"));

    // 단축키 설정
    window->exitAction->setShortcut(QObject::tr("Ctrl+Q"));
    window->deleteAction->setShortcut(QObject::tr("Delete"));
    window->copyAction->setShortcut(QObject::tr("Ctrl+C"));
    window->pasteAction->setShortcut(QObject::tr("Ctrl+V"));
    window->renameAction->setShortcut(QObject::tr("F2"));

    // 파일 관련 시그널 연결
    QObject::connect(window->newFolderAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::createNewFolder(window); });
    QObject::connect(window->deleteAction, &QAction::triggered, window, [window]() {
        qDebug() << "Delete action triggered from UI";
        window->deleteSelected();
    });
    QObject::connect(window->copyAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::copySelected(window); });
    QObject::connect(window->pasteAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::pasteToCurrentDir(window); });
    QObject::connect(window->renameAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::handleRename(window); });
    QObject::connect(window->chmodAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::handleChmod(window); });
    QObject::connect(window->lsAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::handleLs(window); });
    QObject::connect(window->rmdirAction, &QAction::triggered, 
                    [window]() { MainWindowFileActions::handleRmdir(window); });
    QObject::connect(window->exitAction, &QAction::triggered, window, &QWidget::close);

    // 프로세스 관련 액션 추가
    window->psAction = new QAction(QIcon::fromTheme("system-run"), QObject::tr("프로세스 목록"), window);
    window->psAction->setStatusTip(QObject::tr("실행 중인 프로세스 목록 보기"));
    QObject::connect(window->psAction, &QAction::triggered, 
                    [window]() { MainWindowProcessActions::handlePs(window); });

    window->killAction = new QAction(QIcon::fromTheme("process-stop"), QObject::tr("프로세스 종료"), window);
    window->killAction->setStatusTip(QObject::tr("프로세스 종료(kill)"));
    QObject::connect(window->killAction, &QAction::triggered, 
                    [window]() { MainWindowProcessActions::handleKill(window, QString()); });

    // mmap_test 액션 추가
    window->mmapTestAction = new QAction(QIcon::fromTheme("text-x-generic"), QObject::tr("메모리 매핑 테스트"), window);
    window->mmapTestAction->setStatusTip(QObject::tr("파일의 메모리 매핑 테스트"));
    QObject::connect(window->mmapTestAction, &QAction::triggered, 
                    [window]() { MainWindowTestActions::handleMmapTest(window); });

    // execute program 액션 추가
    window->execProgramAction = new QAction(QIcon::fromTheme("application-x-executable"), QObject::tr("프로그램 실행"), window);
    window->execProgramAction->setStatusTip(QObject::tr("외부 프로그램 실행"));
    QObject::connect(window->execProgramAction, &QAction::triggered, 
                    [window]() { MainWindowTestActions::handleExecuteProgram(window); });

    // 초기 상태 설정
    window->deleteAction->setEnabled(false);
    window->copyAction->setEnabled(false);
    window->pasteAction->setEnabled(false);
    window->renameAction->setEnabled(false);
    window->chmodAction->setEnabled(false);
    window->rmdirAction->setEnabled(false);

    // 뒤로 가기 액션 추가
    window->backAction = new QAction(QIcon::fromTheme("go-previous"), QObject::tr("뒤로 가기"), window);
    window->backAction->setStatusTip(QObject::tr("이전 디렉토리로 이동"));
    window->backAction->setShortcut(QObject::tr("Alt+Left"));
    window->backAction->setEnabled(false);  // 초기에는 비활성화

    QObject::connect(window->backAction, &QAction::triggered, [window]() {
        if (!window->directoryHistory.isEmpty()) {
            QString prevDir = window->directoryHistory.pop();
            window->setCurrentDirectory(prevDir.toStdString(), false);  // false는 히스토리에 추가하지 않음을 의미
        }
        window->backAction->setEnabled(!window->directoryHistory.isEmpty());
    });
}

void MainWindowUI::createToolBar(MainWindow* window)
{
    // 기존 파일 툴바
    QToolBar *fileToolBar = window->addToolBar(QObject::tr("파일"));
    fileToolBar->addAction(window->backAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->newFolderAction);
    fileToolBar->addAction(window->deleteAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->copyAction);
    fileToolBar->addAction(window->pasteAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->renameAction);
    fileToolBar->addAction(window->chmodAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->lsAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->rmdirAction);
    fileToolBar->addSeparator();
    fileToolBar->addAction(window->exitAction);

    // 프로세스 관련 툴바
    QToolBar *processToolBar = window->addToolBar(QObject::tr("프프로세스"));
    processToolBar->addAction(window->psAction);
    processToolBar->addAction(window->killAction);

    // 테스트 툴바 추가
    QToolBar *testToolBar = window->addToolBar(QObject::tr("테스트"));
    testToolBar->setObjectName("testToolBar");
    testToolBar->addAction(window->mmapTestAction);
    testToolBar->addAction(window->execProgramAction);
}

void MainWindowUI::createMenuBar(MainWindow* window)
{
    // 파일 메뉴
    QMenu *fileMenu = window->menuBar()->addMenu(QObject::tr("파일(&F)"));
    fileMenu->addAction(window->newFolderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->copyAction);
    fileMenu->addAction(window->pasteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->renameAction);
    fileMenu->addAction(window->chmodAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->lsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->rmdirAction);
    fileMenu->addSeparator();
    fileMenu->addAction(window->exitAction);

    // 프로세스 메뉴
    QMenu *processMenu = window->menuBar()->addMenu(QObject::tr("프로세스(&P)"));
    processMenu->addAction(window->psAction);
    processMenu->addAction(window->killAction);

    // 테스트 메뉴 추가
    QMenu *testMenu = window->menuBar()->addMenu(QObject::tr("테스트(&T)"));
    testMenu->addAction(window->mmapTestAction);
    testMenu->addAction(window->execProgramAction);
}

void MainWindowUI::handleSelectionChanged(MainWindow* window)
{
    // 현재 포커스를 가진 뷰의 선택 모델 가져오기
    QItemSelectionModel *selectionModel = nullptr;
    if (window->treeView->hasFocus()) {
        selectionModel = window->treeView->selectionModel();
    } else if (window->listView->hasFocus()) {
        selectionModel = window->listView->selectionModel();
    }

    bool hasSelection = selectionModel && selectionModel->hasSelection();

    // 액션 활성화/비활성화
    if (hasSelection) {
        QModelIndexList selectedIndexes = selectionModel->selectedRows();
        if (selectedIndexes.isEmpty()) {
            selectedIndexes = selectionModel->selectedIndexes();
        }

        // 선택된 항목이 있으면 모든 관련 액션 활성화
        window->deleteAction->setEnabled(true);
        window->copyAction->setEnabled(true);
        window->renameAction->setEnabled(true);
        window->chmodAction->setEnabled(true);

        // 디렉토리인 경우 rmdir 액션 활성화
        if (!selectedIndexes.isEmpty()) {
            QString selectedPath = window->fileSystemModel->filePath(selectedIndexes.first());
            QFileInfo fileInfo(selectedPath);
            window->rmdirAction->setEnabled(fileInfo.isDir());
        }

        // 상태 업데이트
        QString statusMessage = QObject::tr("%1개 항목 선택됨").arg(selectedIndexes.count());
        window->statusBar()->showMessage(statusMessage);
    } else {
        // 선택된 항목이 없으면 관련 액션 비활성화
        window->deleteAction->setEnabled(false);
        window->copyAction->setEnabled(false);
        window->renameAction->setEnabled(false);
        window->chmodAction->setEnabled(false);
        window->rmdirAction->setEnabled(false);
        window->statusBar()->clearMessage();
    }

    // 붙여넣기 액션은 복사된 항목이 ���을 때만 활성화
    window->pasteAction->setEnabled(!window->selectedIndexes.isEmpty());
}