#include <QtWidgets>
#include "../include/mainwindow.h"
#include "../include/mainwindow_test_actions.h"
#include "../include/commands.h"

MainWindowTestActions::MainWindowTestActions(QObject *parent) : QObject(parent) {}

void MainWindowTestActions::handleMmapTest(MainWindow* window)
{
    QString fileName = QFileDialog::getSaveFileName(window, 
        QObject::tr("메모리 매핑 테스트 파일 선택"),
        window->getCurrentDirectory(),
        QObject::tr("모든 파일 (*)"));

    if (fileName.isEmpty()) {
        return;
    }

    QDialog *resultDialog = new QDialog(window);
    resultDialog->setWindowTitle(QObject::tr("메모리 매핑 테스트 결과"));
    resultDialog->resize(600, 400);

    QVBoxLayout *layout = new QVBoxLayout(resultDialog);
    
    QTextEdit *resultText = new QTextEdit(resultDialog);
    resultText->setReadOnly(true);
    layout->addWidget(resultText);

    QString tempFileName = QDir::temp().filePath("mmap_output.tmp");
    FILE *temp = fopen(tempFileName.toLocal8Bit().constData(), "w+");
    
    if (temp) {
        int stdout_fd = dup(STDOUT_FILENO);
        dup2(fileno(temp), STDOUT_FILENO);
        
        call_mmap_test(fileName.toLocal8Bit().constData());
        
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

    QPushButton *closeButton = new QPushButton(QObject::tr("닫기"), resultDialog);
    layout->addWidget(closeButton);
    QObject::connect(closeButton, &QPushButton::clicked, resultDialog, &QDialog::accept);

    resultDialog->exec();
    delete resultDialog;
}

void MainWindowTestActions::handleExecuteProgram(MainWindow* window)
{
    QDialog dialog(window);
    dialog.setWindowTitle(QObject::tr("프로그램 실행"));
    dialog.resize(400, 200);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QHBoxLayout *pathLayout = new QHBoxLayout;
    QLineEdit *programPath = new QLineEdit(&dialog);
    QPushButton *browseButton = new QPushButton(QObject::tr("찾아보기"), &dialog);
    pathLayout->addWidget(new QLabel(QObject::tr("프로그램:")));
    pathLayout->addWidget(programPath);
    pathLayout->addWidget(browseButton);
    layout->addLayout(pathLayout);

    QLineEdit *argsEdit = new QLineEdit(&dialog);
    layout->addWidget(new QLabel(QObject::tr("명령줄 인자:")));
    layout->addWidget(argsEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    layout->addWidget(buttonBox);

    QObject::connect(browseButton, &QPushButton::clicked, [window, &programPath]() {
        QString fileName = QFileDialog::getOpenFileName(window,
            QObject::tr("프로그램 선택"), window->getCurrentDirectory());
        if (!fileName.isEmpty()) {
            programPath->setText(fileName);
        }
    });

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString program = programPath->text();
        QStringList arguments = argsEdit->text().split(' ', Qt::SkipEmptyParts);
        
        if (program.isEmpty()) {
            QMessageBox::warning(window, QObject::tr("오류"), 
                QObject::tr("프로그램 경로를 입력하세요."));
            return;
        }

        QByteArray programBytes = program.toLocal8Bit();
        QVector<QByteArray> argumentBytes;
        QVector<char*> args;
        
        args.push_back(programBytes.data());
        for (const QString &arg : arguments) {
            argumentBytes.push_back(arg.toLocal8Bit());
            args.push_back(argumentBytes.last().data());
        }
        args.push_back(nullptr);

        execute_program(programBytes.data(), args.data());
        
        window->statusBar()->showMessage(QObject::tr("프로그램 실행 완료"), 3000);
    }
}
