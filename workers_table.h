#ifndef WORKERSTABLE_H
#define WORKERSTABLE_H

#include <departments.h>
#include <workers.h>
#include <references.h>

#include <QVBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QItemDelegate>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QCheckBox>
#include <QComboBox>

class WorkersTable : public QWidget
{
    Q_OBJECT
public:
    explicit WorkersTable(bool isAdmin, Workers *workers, Departments *departments, References *references, QWidget *parent = nullptr);
    ~WorkersTable();

private slots:
    void handleClickReferencesButton();
    void handleClickAddButton();
    void handleClickRemoveButton();

private:
    QTableView *workersTable;
    QPushButton *referencesButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    Workers *workers;
    References *references;
};

class WorkerRowDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    WorkerRowDelegate(Departments* departments);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    Departments *departments;
};

#endif // WORKERSTABLE_H
