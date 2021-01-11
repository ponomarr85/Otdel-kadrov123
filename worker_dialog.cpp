#include "worker_dialog.h"
#include "auth_dialog.h"

WorkerDialog::WorkerDialog(References *references, Worker worker, QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;

    backButton = new QPushButton(tr("Выйти"));
    connect(backButton, &QPushButton::released, this, &WorkerDialog::handleBackButton);
    mainLayout->addWidget(backButton);

    tabWidget = new QTabWidget;
    tabWidget->addTab(new WorkerLayout(worker, this), tr("Информация по сотруднику"));
    tabWidget->addTab(new ReferencesTable(false, worker.serialNumber(), references, this), tr("Справки"));
    mainLayout->addWidget(tabWidget);

    resize(640, 400); //Задаем размер окна
    setLayout(mainLayout);
    setWindowTitle(tr("Окно работника"));
}

void WorkerDialog::handleBackButton()
{
    AuthDialog *authDialog = new AuthDialog(this);
    authDialog->show();
    hide();
}
