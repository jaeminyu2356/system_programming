# 파일 시스템 GUI 구현 보고서

## 1. 개요
이 프로젝트는 파일 시스템 관리를 위한 그래픽 사용자 인터페이스(GUI)를 구현한 것입니다. Qt 프레임워크를 사용하여 직관적이고 사용하기 쉬운 인터페이스를 제공합니다.

## 2. 구현 환경
- **개발 환경**
  - 운영체제: Linux (Ubuntu)
  - 개발 도구: Qt 5.15.2
  - 컴파일러: GCC 9.4.0
  - 빌드 시스템: CMake 3.16.3

## 3. 클래스 구조 및 설계

### 3.1 핵심 클래스
1. **MainWindow 클래스**
   - GUI의 메인 윈도우를 담당
   - 기본 프레임워크 제공
   - 다른 모든 컴포넌트들의 컨테이너 역할
   - 주요 멤버:
     ```cpp
     QFileSystemModel *fileSystemModel;  // 파일 시스템 모델
     QTreeView *treeView;               // 트리 뷰
     QListView *listView;               // 리스트 뷰
     std::string currentPath;           // 현재 경로
     QStack<QString> directoryHistory;  // 디렉토리 히스토리
     ```

2. **MainWindowUI 클래스**
   - UI 컴포넌트 초기화 및 관리
   - 레이아웃 구성
   - 시그널/슬롯 연결
   - 주요 메서드:
     ```cpp
     static void setupUI();          // UI 초기화
     static void createActions();    // 액션 생성
     static void createToolBar();    // 툴바 생성
     static void createMenuBar();    // 메뉴바 생성
     ```

3. **MainWindowFileActions 클래스**
   - 파일 시스템 작업 처리
   - 파일 및 디렉토리 관리
   - 주요 메서드:
     ```cpp
     static void handleLs();        // 디렉토리 내용 표시
     static void handleMkdir();     // 디렉토리 생성
     static void handleRm();        // 파일/디렉토리 삭제
     static void handleChmod();     // 권한 변경
     static void handleCp();        // 파일 복사
     ```

### 3.2 주요 기능 구현 상세

#### 3.2.1 파일 시스템 모델 구현

## 4. 주요 기능 구현

### 4.1 기본 인터페이스 구성
- **분할 뷰 구현**
  - 좌측: 디렉토리 트리 뷰
  - 우측: 파일/폴더 리스트 뷰
  - 수직 분할을 통한 효율적인 공간 활용
  - 드래그 앤 드롭으로 크기 조절 가능

- **파일 시스템 모델**
  - QFileSystemModel을 사용한 실시간 파일 시스템 표현
  - 파일 및 디렉토리 정보의 실시간 업데이트
  - 숨김 파일 필터링 기능

### 4.2 파일 관리 기능
- **기본 파일 작업**
  - 새 폴더 생성 (mkdir)
  - 파일/폴더 삭제 (rm)
  - 복사/붙여넣기 (cp)
  - 이름 변경 (rename)
  - 권한 변경 (chmod)
  - 심볼릭 링크 생성 (ln)

- **고급 파일 작업**
  - 디렉토리 상세 정보 보기 (ls)
    - 시간순/크기순 정렬
    - 역순 정렬 옵션
    - 상세 시간 정보 표시
  - 파일 내용 보기 (cat)
  - 빈 디렉토리 삭제 (rmdir)

### 4.3 사용자 인터페이스 개선
- **컨텍스트 메뉴**
  - 우클릭으로 접근 가능한 상황별 메뉴
  - 파일/폴더별 적절한 작업 옵션 제공

- **파일 미리보기**
  - 텍스트 파일 내용 미리보기
  - 이미지 파일 썸네일 표시
  - 파일 상세 정보 표시
    - 파일 크기
    - 수정 날짜
    - 권한 정보
    - 소유자/그룹 정보

- **검색 기능**
  - 실시간 파일 검색
  - 파일명 기반 검색
  - 대소문자 구분 없는 검색
  - 검색 결과 하이라이트

### 4.4 프로세스 관리
- **프로세스 목록 보기**
  - 실행 중인 프로세스 정보 표시
  - PID, 상태, CPU 사용량 등 표시
  - 실시간 업데이트

- **프로세스 제어**
  - 프로세스 종료 (kill)
  - 시그널 전송
  - 권한 검사 및 안전장치

### 4.5 네비게이션 기능
- **디렉토리 탐색**
  - 더블클릭으로 디렉토리 진입
  - 뒤로 가기 버튼
  - 디렉토리 히스토리 관리
  - 단축키 지원 (Alt+Left)

### 4.6 시각적 피드백
- **상태바 정보**
  - 현재 디렉토리 정보
  - 선택된 항목 수
  - 작업 결과 메시지
  - 디스크 사용량 표시

- **진행 상태 표시**
  - 파일 작업 진행률
  - 검색 진행 상태
  - 오류 메시지 표시

## 5. 기술적 구현 세부사항

### 5.1 코드 구조
- **모듈화된 설계**
  - UI 관련 코드 분리 (MainWindowUI)
  - 파일 작업 관련 코드 분리 (MainWindowFileActions)
  - 프로세스 관련 코드 분리 (MainWindowProcessActions)
  - 테스트 관련 코드 분리 (MainWindowTestActions)

- **이벤트 처리**
  - 시그널/슬롯 메커니즘 활용
  - 비동기 작업 처리
  - 사용자 입력 유효성 검사

### 5.2 성능 최적화
- **리소스 관리**
  - 메모리 사용 최적화
  - 대용량 파일 처리
  - 캐시 활용

- **반응성**
  - 비동기 파일 작업
  - UI 스레드 블로킹 방지
  - 부드러운 애니메이션

### 5.3 오류 처리
- **예외 처리**
  - 파일 시스템 오류 처리
  - 권한 관련 오류 처리
  - 사용자 피드백 제공

- **복구 메커니즘**
  - 작업 실패 시 롤백
  - 자동 저장 기능
  - 오류 로깅

## 6. 사용자 경험 개선사항
- 직관적인 아이콘 사용
- 키보드 단축키 지원
- 툴팁으로 기능 설명 제공
- 사용자 설정 저장/복원
- 다크/라이트 테마 지원
- 다국어 지원 (한글/영어)

## 7. 향후 개선 계획
- 파일 필터링 기능 추가
- 북마크 기능 구현
- 파일 압축/해제 기능
- 네트워크 드라이브 지원
- 플러그인 시스템 도입
- 클라우드 저장소 연동

## 8. 결론
이 GUI 구현을 통해 파일 시스템 관리를 더욱 효율적이고 사용자 친화적으로 만들었습니다. 모듈화된 설계와 확장 가능한 구조를 통해 향후 기능 추가와 유지보수가 용이하도록 구현했습니다.
