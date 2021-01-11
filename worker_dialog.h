#ifndef WORKER_DIALOG_H
#define WORKER_DIALOG_H

#include <worker.h>
#include <worker_layout.h>
#include <references.h>
#include <references_table.h>

#include <QWidget>
#include <QDialog>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>

class WorkerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WorkerDialog(References *references, Worker worker, QWidget *parent = nullptr);

private slots:
    void handleBackButton();

private:
    QTabWidget *tabWidget;
    QPushButton *backButton;
};

#endif // WORKER_DIALOG_H
