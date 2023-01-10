#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTableWidget *table;
    QTableWidget *tableSearch;
    int rowCount;
    double *array;

private slots:
    void addElementInTableSearch(int value, int row);

    bool isEqual(double a, double b);

    void shuffle();

    bool isSorted();

    int calculateQuickSort(int start, int end);

    void quickSort(int start, int end);

    void outputArray();

    bool createArray();

    void warn(int rowCount);

    bool checkValue(QTableWidgetItem *item);

    void changeColor(int row, int column);

    void on_pushButton_random_clicked();

    void on_pushButton_clear_clicked();

    void on_spinBox_rowCount_valueChanged(int arg1);

    void on_pushButton_filler_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_output_clicked();

    void on_pushButton_bubble_clicked();

    void on_pushButton_clear_2_clicked();

    void on_pushButton_gnome_clicked();

    void on_pushButton_quick_clicked();

    void on_pushButton_comb_clicked();

    void on_pushButton_monkey_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_del_dublicates_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
