#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    userInput = new QLineEdit(this);
    productsTable = new QTableWidget(this);
    resize(550, 300); // Установка размера окна
    // Установка количества столбцов и заголовков для таблицы
    productsTable->setColumnCount(5); // 5 столбов: чекбоксы, название, количество, цена, спинбоксы
    productsTable->setHorizontalHeaderLabels(QStringList() << "" << "Название" << "Количество" << "Цена" << "Заказать");

    orderButton = new QPushButton("Заказать", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(userInput);
    layout->addWidget(productsTable);
    layout->addWidget(orderButton);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(orderButton, &QPushButton::clicked, this, &MainWindow::onOrderClicked);

    // Очистка cheque.txt при запуске
    QFile chequeFile("C:/Users/User/Documents/FinalProgect/cheque.txt");
    if (chequeFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        chequeFile.close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось очистить файл 'cheque.txt'.");
    }

    loadProducts(); // Загрузка данных о продуктах
}

void MainWindow::updateProductsFile() {
    QFile file("C:/Users/User/Documents/FinalProgect/products.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл 'products.txt' для обновления.");
        return;
    }

    QTextStream out(&file);

    // Очистка таблицы и удаление старых виджетов
    productsTable->clearContents();
    productsTable->setRowCount(0);
    qDeleteAll(checkBoxes);
    checkBoxes.clear();
    qDeleteAll(spinBoxes);
    spinBoxes.clear();

    for (int row = 0; row < productsTable->rowCount(); ++row) {
        // Получаем данные из каждой строки таблицы
        QString name = productsTable->item(row, 1)->text(); // Название
        QString quantity = productsTable->item(row, 2)->text(); // Количество
        QString price = productsTable->item(row, 3)->text(); // Цена

        // Записываем данные обратно в файл
        out << name << " - " << quantity << " | " << price << "\n";
    }

    file.close();
}


void MainWindow::loadProducts() {
    QFile file("C:/Users/User/Documents/FinalProgect/products.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл 'products.txt' для чтения.");
        qDebug() << "Не удалось открыть файл.";
        return;
    }

    QTextStream in(&file);
    productsTable->setRowCount(0);
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        qDebug() << "Считанная строка:" << line;

        // Сплит строк
        QStringList parts = line.split(" - ");
        if (parts.size() == 2) {
            QStringList secondPart = parts[1].split(" | ");
            if (secondPart.size() == 2) {
                productsTable->insertRow(row);

                // Добавление чекбокса
                QCheckBox *checkBox = new QCheckBox(this);
                checkBoxes.append(checkBox);
                productsTable->setCellWidget(row, 0, checkBox);

                // Добавление имени
                productsTable->setItem(row, 1, new QTableWidgetItem(parts[0].trimmed()));

                // Добавление количества
                int quantity = secondPart[0].toInt();
                productsTable->setItem(row, 2, new QTableWidgetItem(QString::number(quantity)));

                // Добавление цены
                productsTable->setItem(row, 3, new QTableWidgetItem(secondPart[1].trimmed()));

                // Добавление спинбокса для выбора количества
                QSpinBox *spinBox = new QSpinBox(this);
                spinBox->setMinimum(0);
                spinBox->setMaximum(quantity); // Максимум - количество товара на складе
                spinBoxes.append(spinBox);
                productsTable->setCellWidget(row, 4, spinBox);

                row++;
            }
        } else {
            qDebug() << "Некорректный формат строки:" << line;
        }
    }

    file.close();
}

void MainWindow::onOrderClicked() {
    // Структура для хранения информации о заказе
    struct OrderInfo {
        QString name;
        int quantityOrdered;
        int price;
    };
    QList<OrderInfo> orders;

    // Сбор информации о заказанных товарах и обновление количества
    for (int row = 0; row < productsTable->rowCount(); ++row) {
        if (checkBoxes[row]->isChecked()) {
            QString name = productsTable->item(row, 1)->text();
            int quantityAvailable = productsTable->item(row, 2)->text().toInt();
            int price = productsTable->item(row, 3)->text().toInt();
            int quantityOrdered = spinBoxes[row]->value();

            if (quantityOrdered > 0 && quantityOrdered <= quantityAvailable) {
                quantityAvailable -= quantityOrdered;
                productsTable->item(row, 2)->setText(QString::number(quantityAvailable));
                orders.append({name, quantityOrdered, price});
            }
        }
    }

    // Запись обновленной информации о продуктах в файл products.txt
    QFile file("C:/Users/User/Documents/FinalProgect/products.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл 'products.txt' для записи.");
        return;
    }
    QTextStream out(&file);
    for (int row = 0; row < productsTable->rowCount(); ++row) {
        QString name = productsTable->item(row, 1)->text();
        QString quantity = productsTable->item(row, 2)->text();
        QString price = productsTable->item(row, 3)->text();
        out << name << " - " << quantity << " | " << price << "\n";
    }
    file.close();

    // Запись информации о заказе в файл cheque.txt
    QFile chequeFile("C:/Users/User/Documents/FinalProgect/cheque.txt");
    if (!chequeFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл 'cheque.txt' для записи.");
        return;
    }
    QTextStream chequeOut(&chequeFile);
    chequeOut << "Пользователь: " << userInput->text() << "\n";
    int totalSum = 0;
    for (const OrderInfo &order : orders) {
        int sum = order.quantityOrdered * order.price;
        totalSum += sum;
        chequeOut << order.name << " - " << order.quantityOrdered << " - " << sum << "\n";
    }
    chequeOut << "Итоговая сумма: " << totalSum << "\n\n";
    chequeFile.close();

    QMessageBox::information(this, "Заказ оформлен", "Ваш заказ успешно оформлен и сохранен в файле 'cheque.txt'.");
    loadProducts(); // Перезагрузка продуктов для обновления таблицы
}
