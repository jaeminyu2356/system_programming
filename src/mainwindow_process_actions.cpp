#include <QtWidgets>
#include "../include/mainwindow.h"
#include "../include/mainwindow_process_actions.h"
#include "../include/commands.h"

MainWindowProcessActions::MainWindowProcessActions(QObject *parent) : QObject(parent) {}

void MainWindowProcessActions::handlePs(MainWindow* window)
{
    QDialog *psDialog = new QDialog(window);
    psDialog->setWindowTitle(QObject::tr("프로세스 목록"));
    psDialog->resize(800, 600);
    
    QVBoxLayout *layout = new QVBoxLayout(psDialog);
    QTableWidget *processTable = new QTableWidget(psDialog);
    layout->addWidget(processTable);
    
    // 프로세스 목록 표시
    refreshProcessList(window, processTable);
    
    // 프로세스 종료 버튼과 새로고침 버튼
    QPushButton *killButton = new QPushButton(QObject::tr("프로세스 종료"), psDialog);
    QPushButton *refreshButton = new QPushButton(QObject::tr("새로고침"), psDialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(killButton);
    buttonLayout->addWidget(refreshButton);
    layout->addLayout(buttonLayout);
    
    // 프로세스 종료 버튼 클릭 시
    QObject::connect(killButton, &QPushButton::clicked, [window, processTable]() {
        QList<QTableWidgetItem*> selectedItems = processTable->selectedItems();
        if (!selectedItems.isEmpty()) {
            int row = selectedItems.first()->row();
            QString pid = processTable->item(row, 1)->text();
            
            if (pid.toInt() == getpid()) {
                QMessageBox::warning(window, QObject::tr("오류"),
                                   QObject::tr("현재 실행 중인 프로그램은 종료할 수 없습니다."));
                return;
            }
            
            bool ok;
            QString signal = QInputDialog::getText(window, QObject::tr("시그널 전송"),
                                                QObject::tr("PID %1에 전송할 시그널 번호 (기본: 9):").arg(pid),
                                                QLineEdit::Normal,
                                                "9", &ok);
            if (ok) {
                if (call_kill(pid.toLocal8Bit().constData(),
                             signal.toLocal8Bit().constData()) == 0) {
                    QMessageBox::information(window, QObject::tr("성공"),
                                          QObject::tr("프로세스가 종료되었습니다."));
                    refreshProcessList(window, processTable);
                } else {
                    QMessageBox::warning(window, QObject::tr("오류"),
                                      QObject::tr("프로세스를 종료할 수 없습니다."));
                }
            }
        }
    });
    
    // 새로고침 버튼 클릭 시
    QObject::connect(refreshButton, &QPushButton::clicked, [window, processTable]() {
        refreshProcessList(window, processTable);
    });
    
    psDialog->exec();
    delete psDialog;
}

void MainWindowProcessActions::handleKill(MainWindow* window, const QString &pid)
{
    bool ok;
    QString signal = QInputDialog::getText(window, QObject::tr("시그널 전송"),
                                         QObject::tr("PID %1에 전송할 시그널 번호 (기본: 15):").arg(pid),
                                         QLineEdit::Normal,
                                         "15", &ok);
    if (ok) {
        call_kill(pid.toLocal8Bit().constData(),
                 signal.toLocal8Bit().constData());
    }
}

void MainWindowProcessActions::refreshProcessList(MainWindow* window, QTableWidget *processTable)
{
    processTable->setRowCount(0);
    
    QString tempFileName = QDir::temp().filePath("ps_output.tmp");
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    
    if (temp) {
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        call_ps("-ef");
        
        dup2(stdout_fd, STDOUT_FILENO);
        ::close(stdout_fd);
        fclose(temp);
        
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
    
    processTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

// 프로세스 관련 액션들의 구현 