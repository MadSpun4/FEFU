#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QCheckBox>
#include <QSpinBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onOrderClicked();

private:
    void loadProducts();
    void updateProductsFile();

    QLineEdit *userInput;
    QTableWidget *productsTable;
    QPushButton *orderButton;
    QVector<QCheckBox*> checkBoxes;
    QVector<QSpinBox*> spinBoxes;
};

#endif
