#include "workers_table.h"
#include "reference_dialog.h"

WorkersTable::WorkersTable(bool isAdmin, Workers *workers, Departments *departments, References *references, QWidget *parent) : QWidget(parent)
{
    this->workers = workers;
    this->references = references;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    workersTable = new QTableView;
    workersTable->setModel(workers);
    workersTable->setItemDelegate(new WorkerRowDelegate(departments));
    mainLayout->addWidget(workersTable);

    referencesButton = new QPushButton(tr("Справки"));
    connect(referencesButton, &QPushButton::released, this, &WorkersTable::handleClickReferencesButton);
    mainLayout->addWidget(referencesButton);

    addButton = new QPushButton(tr("Добавить работника"));
    connect(addButton, &QPushButton::released, this, &WorkersTable::handleClickAddButton);

    removeButton = new QPushButton(tr("Удалить работника"));
    connect(removeButton, &QPushButton::released, this, &WorkersTable::handleClickRemoveButton);

    if (isAdmin == true)
    {
        mainLayout->addWidget(addButton);
        mainLayout->addWidget(removeButton);
    } else
    {
        workersTable->hideColumn(11);
        workersTable->hideColumn(12);
        workersTable->hideColumn(13);
    }

    setLayout(mainLayout);
}

WorkersTable::~WorkersTable()
{
    delete workersTable;
    delete addButton;
    delete removeButton;
}

void WorkersTable::handleClickAddButton()
{
    int maxSerialNumber = 0;
    for (int i = 0; i < workers->rowCount(QModelIndex()); ++i)
    {
        Worker worker = workers->get(i);
        if (worker.serialNumber() > maxSerialNumber)
        {
            maxSerialNumber = worker.serialNumber();
        }
    }
    Worker worker;
    worker.setSerialNumber(maxSerialNumber + 1);
    worker.setIsFired(false);
    worker.setDepartmentId(0); // Неизвестный департамент
    worker.setNumberOfChildren(0);
    worker.setBirthday(QDate::currentDate().toString("dd.MM.yyyy"));

    workers->add(worker);
}

void WorkersTable::handleClickReferencesButton()
{
    QModelIndexList selectedRows = workersTable->selectionModel()->selectedRows();
    if (selectedRows.size() == 1)
    {
        Worker worker = workers->get(selectedRows[0].row());
        ReferenceDialog *referenceDialog = new ReferenceDialog(worker.serialNumber(), references, this);
        referenceDialog->exec();
    }
}

void WorkersTable::handleClickRemoveButton()
{
    int ask = QMessageBox::question(this,
                                          QString("Предупреждение"),
                                          QString("Вы действительно хотите удалить работника?"),
                                          QMessageBox::Yes | QMessageBox::No);
    if (ask == QMessageBox::Yes)
    {
        QModelIndexList selectedRows = workersTable->selectionModel()->selectedRows();
        for (const auto &row : selectedRows)
        {
            workers->remove(row.row());
        }
    }
}

WorkerRowDelegate::WorkerRowDelegate(Departments *departments)
{
    this->departments = departments;
}

QWidget *WorkerRowDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 3: // Колонка "Даты Рождения"
    {
        QDateTimeEdit *birthdayEditor = new QDateTimeEdit(parent);
        birthdayEditor->setDisplayFormat("dd.MM.yyyy"); // формат даты ДД.ММ.ГГГГ
        birthdayEditor->setCalendarPopup(true);
        birthdayEditor->setDate(QDate::currentDate()); // устанавливаем дату по умолчанию
        return birthdayEditor;
    }
    case 9: // Колонка "Уволен"
    {
        QCheckBox *isFiredEditor = new QCheckBox(parent);
        return isFiredEditor;
    }
    case 10: // Колонка "Департамент"
    {
        QComboBox *departmentEditor = new QComboBox(parent);
        departmentEditor->setModel(departments);
        departmentEditor->setModelColumn(1);
        return departmentEditor;
    }
    }
    QLineEdit *editor = new QLineEdit(parent);
    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();
    editor->setText(currentText);
    return editor;
}

void WorkerRowDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 3: // Колонка "Даты Рождения"
    {
        QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit *>(editor);
        if (dateEditor)
        {
            dateEditor->setDate(QDate::fromString(index.model()->data(index, Qt::EditRole).toString(),"dd.MM.yyyy"));
        }
        break;
    }
    case 9: // Колонка "Уволен"
    {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(editor);
        if (checkBox)
        {
            QString isFiredValue = index.model()->data(index, Qt::EditRole).toString();
            checkBox->setChecked(isFiredValue == "Да");
        }
        break;
    }
    case 10: // Колонка "Департамент"
    {
        QComboBox *departmentEditor = qobject_cast<QComboBox*>(editor);
        if (departmentEditor)
        {
            QString departmentName = index.model()->data(index, Qt::EditRole).toString();
            if (departmentName != "")
            {
                departmentEditor->setCurrentText(departmentName);
            }
        }
        break;
    }
    default:
        QLineEdit *textEditor = qobject_cast<QLineEdit *>(editor);
        if (textEditor)
        {
            textEditor->setText(index.model()->data(index, Qt::EditRole).toString());
        }
    }
}

void WorkerRowDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 3:
    {
        QDateTimeEdit *dateEditor = qobject_cast<QDateTimeEdit*>(editor);
        if (dateEditor)
        {
            model->setData(index, dateEditor->date().toString("dd.MM.yyyy")); // дату приводим к формату ДД.ММ.ГГГГ
        }
        break;
    }
    case 9:
    {
        QCheckBox *checkBox = qobject_cast<QCheckBox*>(editor);
        if (checkBox)
        {
            model->setData(index, checkBox->isChecked());
        }
        break;
    }
    case 10:
    {
        QComboBox *departmentEditor = qobject_cast<QComboBox*>(editor);
        if (departmentEditor)
        {
            departmentEditor->setModelColumn(0);
            int idx = departmentEditor->currentText().toInt();
            departmentEditor->setModelColumn(1);
            model->setData(index, idx);
        }
        break;
    }
    default:
        QLineEdit *textEditor = qobject_cast<QLineEdit*>(editor);
        if (textEditor)
        {
            model->setData(index, textEditor->text());
        }
    }
}
