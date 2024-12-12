# 파일 시스템 GUI 구현 소스코드 보고서

## 1. 프로젝트 개요
### 1.1 개발 목표
- 리눅스 파일 시스템 명령어의 GUI 구현
- 터미널 스타일의 인터페이스 제공
- 사용자 친화적인 파일 관리 기능 구현

### 1.2 개발 환경
- OS: Linux (Ubuntu)
- 개발 도구: Qt 5.15.2
- 컴파일러: GCC 9.4.0
- 빌드 시스템: CMake 3.16.3

## 2. 클래스 구조 및 구현

### 2.1 MainWindow 클래스
```cpp
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QFileSystemModel *fileSystemModel;  // 파일 시스템 모델
    QTreeView *treeView;               // 트리 뷰
    QListView *listView;               // 리스트 뷰
    std::string currentPath;           // 현재 경로
    QStack<QString> directoryHistory;  // 디렉토리 히스토리
    bool moveOperation;                // 이동 작업 여부
};
```
- 파일 시스템 모델과 뷰 관리
- 디렉토리 히스토리를 통한 뒤로 가기 기능
- 파일 이동/복사 작업 상태 관리

### 2.2 파일 시스템 작업 구현
```cpp
// 파일 삭제 구현
void MainWindowFileActions::deleteSelected(MainWindow* window)
{
    QModelIndexList selected = window->listView->selectionModel()->selectedRows();
    if (selected.isEmpty()) return;

    QMessageBox::StandardButton reply = QMessageBox::question(window, 
        QObject::tr("삭제 확인"),
        QObject::tr("선택한 항목을 삭제하시겠습니까?"),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (const QModelIndex &index : selected) {
            QString path = window->fileSystemModel->filePath(index);
            QString fileName = QFileInfo(path).fileName();
            
            // 디렉토리인 경우 재귀적 삭제
            if (QFileInfo(path).isDir()) {
                removeDirectory(path);
            } else {
                // 일반 파일 삭제
                call_rm(window->currentPath.c_str(),
                       fileName.toLocal8Bit().constData(),
                       false);
            }
        }
        refreshFileList(window);
    }
}

// 파일/디렉토리 복사 구현
void MainWindowFileActions::copyDirectory(const QString &sourcePath, 
                                        const QString &destPath)
{
    QDir sourceDir(sourcePath);
    QDir destDir(destPath);
    
    // 대상 디렉토리 생성
    if (!destDir.exists()) {
        destDir.mkpath(".");
    }

    // 디렉토리 내용물 복사
    QFileInfoList entries = sourceDir.entryInfoList(
        QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        QString srcPath = entry.filePath();
        QString destFilePath = destDir.filePath(entry.fileName());
        
        if (entry.isDir()) {
            // 디렉토리인 경우 재귀적 복사
            copyDirectory(srcPath, destFilePath);
        } else {
            // 파일인 경우 call_cp 사용
            call_cp(QFileInfo(destFilePath).path().toLocal8Bit().constData(),
                   srcPath.toLocal8Bit().constData(),
                   destFilePath.toLocal8Bit().constData());
        }
    }
}
```

### 2.3 프로세스 관리 구현
```cpp
void MainWindowProcessActions::handlePs(MainWindow* window)
{
    QDialog *psDialog = new QDialog(window);
    psDialog->setWindowTitle(QObject::tr("프로세스 목록"));
    psDialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(psDialog);
    QTableWidget *processTable = new QTableWidget(psDialog);
    layout->addWidget(processTable);
    
    // 프로세스 목록 표시
    refreshProcessList(processTable);
    
    // 프로세스 종료 버튼과 새로고침 버튼
    QPushButton *killButton = new QPushButton(QObject::tr("프로세스 종료"));
    QPushButton *refreshButton = new QPushButton(QObject::tr("새로고침"));
    
    // 프로세스 종료 기능 구현
    QObject::connect(killButton, &QPushButton::clicked, [window, processTable]() {
        QList<QTableWidgetItem*> selectedItems = processTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            QString pid = processTable->item(row, 1)->text();
            
            // 현재 프로세스 종료 방지
            if (pid.toInt() == getpid()) {
                QMessageBox::warning(window, QObject::tr("오류"),
                    QObject::tr("현재 실행 중인 프로그램은 종료할 수 없습니다."));
                return;
            }
            
            // 시그널 전송
            bool ok;
            QString signal = QInputDialog::getText(window, 
                QObject::tr("시그널 전송"),
                QObject::tr("PID %1에 전송할 시그널 번호:").arg(pid),
                QLineEdit::Normal, "9", &ok);
                
            if (ok) {
                call_kill(pid.toLocal8Bit().constData(),
                         signal.toLocal8Bit().constData());
                refreshProcessList(processTable);
            }
        }
    });
}
```

### 2.4 파일 시스템 뷰 구현
```cpp
void MainWindowUI::setupUI(MainWindow* window)
{
    // 파일 시스템 모델 설정
    window->fileSystemModel = new QFileSystemModel(window);
    window->fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    window->fileSystemModel->setReadOnly(false);

    // 트리뷰/리스트뷰 설정
    QSplitter *splitter = new QSplitter(window);
    window->treeView = new QTreeView(splitter);
    window->listView = new QListView(splitter);

    // 드래그 앤 드롭 설정
    window->treeView->setDragEnabled(true);
    window->treeView->setAcceptDrops(true);
    window->treeView->setDropIndicatorShown(true);
    window->treeView->setDragDropMode(QAbstractItemView::DragDrop);
    
    window->listView->setDragEnabled(true);
    window->listView->setAcceptDrops(true);
    window->listView->setDropIndicatorShown(true);
    window->listView->setDragDropMode(QAbstractItemView::DragDrop);

    // 컨텍스트 메뉴 설정
    window->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    window->listView->setContextMenuPolicy(Qt::CustomContextMenu);
}
```

## 3. 주요 기능 구현 상세

### 3.1 파일 시스템 작업
- **파일/디렉토리 생성**
  - mkdir 명령어 사용
  - 권한 설정 지원
  - 중복 이름 체크

- **파일/디렉토리 삭제**
  - rm/rmdir 명령어 사용
  - 재귀적 삭제 지원
  - 삭제 전 확인 대화상자

- **파일/디렉토리 복사/이동**
  - cp 명령어 사용
  - 드래그 앤 드롭 지원
  - 진행 상태 표시

### 3.2 프로세스 관리
- **프로세스 목록 표시**
  - ps 명령어 결과 파싱
  - 실시간 업데이트
  - 정렬 기능

- **프로세스 종료**
  - kill 명령어 사용
  - 시그널 번호 선택
  - 권한 체크

### 3.3 사용자 인터페이스
- **파일 브라우저**
  - 트리뷰/리스트뷰 동기화
  - 아이콘 모드 지원
  - 파일 필터링

- **컨텍스트 메뉴**
  - 상황별 메뉴 항목
  - 단축키 지원
  - 툴팁 표시

## 4. 성능 최적화

### 4.1 메모리 관리
- **지연 로딩**
  ```cpp
  fileSystemModel->setRootPath(QString::fromStdString(currentPath));
  ```
  - 필요한 시점에 파일 정보 로드
  - 대용량 디렉토리 처리 최적화

- **리소스 해제**
  ```cpp
  // 임시 파일 자동 삭제
  QFile::remove(tempFileName);
  ```
  - 메모리 누수 방지
  - 임시 파일 정리

### 4.2 UI 반응성
- **비동기 작업**
  - 파일 작업 중 UI 블로킹 방지
  - 진행 상태 표시
  - 작업 취소 지원

## 5. 오류 처리
- **예외 처리**
  ```cpp
  try {
      // 파일 시스템 작업 수행
      if (result != 0) {
          throw std::runtime_error("작업 실패");
      }
  } catch (const std::exception& e) {
      QMessageBox::critical(window, QObject::tr("오류"),
          QObject::tr("작업 중 오류가 발생했습니다: %1").arg(e.what()));
  }
  ```

- **사용자 피드백**
  - 상태바 메시지
  - 에러 대화상자
  - 작업 결과 표시

## 6. 결론
- 리눅스 명령어의 GUI 구현 완료
- 터미널 스타일 인터페이스 유지
- 사용자 친화적 기능 추가
- 안정적인 파일 시스템 작업 처리
``` 