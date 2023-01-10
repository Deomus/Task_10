#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_clear->click();
    ui->tableWidget_search->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_search->clear();
    rowCount = 1;
    array = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// функция вывода массива
void MainWindow::outputArray()
{
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(array[i]));
        ui->tableWidget->setItem(i, 0, item);
    }

}

// функция создания массива
bool MainWindow::createArray()
{
    // удаляем все что было до этого
    delete [] array;
    array = nullptr;

    try
    {
        array = new double[rowCount];
    }
    catch (std::bad_alloc&)
    {
        QMessageBox::information(this,"Ошибка","Невозможно выделить память",QMessageBox::Ok);
        return false;
    }

    bool error = false;
    for (int i = 0; i < rowCount; ++i)
    {
       // проверка на айтем
       QTableWidgetItem *item = new QTableWidgetItem;
       if (ui->tableWidget->item(i, 0) == nullptr)
           ui->tableWidget->setItem(i, 0, item);
       else
           item = ui->tableWidget->item(i, 0);
       // проверка числа
       if (!checkValue(item))
       {
           ui->tableWidget->item(i, 0)->setBackground(Qt::red);
           error = true;
           continue;
       }
       else if (error)
           continue;
       array[i] = item->text().toDouble();
    }
    if (error)
    {
        QMessageBox::information(this, "Ошибка", "Заполните элементы правильно", QMessageBox::Ok);
        return false;
    }
    return true;
}

// предупреждение при большом количестве элементов
void MainWindow::warn(int rowCount)
{
    if (rowCount >= 300000)
        QMessageBox::information(this, "Внимание", "Операции необходимо время.", QMessageBox::Ok);
}

// проверка числа
bool MainWindow::checkValue(QTableWidgetItem *item)
{
    if (item != nullptr)
    {
        bool flag;
        double value = item->text().toDouble(&flag);
        if (flag && !qIsInf(value) && !qIsNaN(value))
            return true;
    }
    return false;
}

// изменение цвета при неправильном вводе
void MainWindow::changeColor(int row, int column)
{
    QTableWidgetItem *item = new  QTableWidgetItem;
    item = ui->tableWidget->item(row, column);

    if (!checkValue(item))
        item->setBackground(Qt::red);
    else
        item->setBackground(Qt::white);
}

// сортировка пузырьком
void MainWindow::on_pushButton_bubble_clicked()
{
    warn(rowCount);

    // создаем массив
    if (!createArray())
        return;

    // сортируем
    double temp;
    for (int i = 0; i < rowCount - 1; i++)
    {
        for (int j = 0; j < rowCount - i - 1; j++)
        {
            if (array[j] > array[j + 1])
            {
                // меняем элементы местами
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    outputArray();
}

// гномья сортировка
void MainWindow::on_pushButton_gnome_clicked()
{
    warn(rowCount);

    // создаем массив
    if (!createArray())
        return;

    // сортировка
    int i = 1; // счётчик
    while (i < rowCount/*если мы не в конце*/)
    {
        if (i == 0)
            i = 1;
        if (array[i-1] <= array[i])
            ++i; // идём вперед
        else
        {
            // меняем местами
            double tmp = array[i];
            array[i] = array[i-1];
            array[i-1] = tmp;
            // идём назад
            --i;
        }
    }

    outputArray();
}

// Разделение по схеме Lomuto
int MainWindow::calculateQuickSort(int start, int end)
{
    // Выбираем крайний правый элемент в качестве опорного элемента массива
    double pivot = array[end];

    // элементы, меньшие точки поворота, будут перемещены влево от `pIndex`
    // элементы больше, чем точка поворота, будут сдвинуты вправо от `pIndex`
    // равные элементы могут идти в любом направлении
    int pIndex = start;

    // каждый раз, когда мы находим элемент, меньший или равный опорному, `pIndex`
    // увеличивается, и этот элемент будет помещен перед опорной точкой.
    for (int i = start; i < end; i++)
    {
        if (array[i] <= pivot)
        {
            double temp = array[i];
            array[i] = array[pIndex];
            array[pIndex] = temp;
            pIndex++;
        }
    }


    // поменять местами `pIndex` с пивотом
    double temp = array[end];
    array[end] = array[pIndex];
    array[pIndex] = temp;

    // вернуть `pIndex` (индекс опорного элемента)
    return pIndex;
}

// быстрая сортировка
void MainWindow::quickSort(int start, int end)
{
    // базовое условие
    if (start >= end)
        return;
    // переставить элементы по оси
    int pivot = calculateQuickSort( start, end);
    // повторяем подмассив, содержащий элементы, меньшие опорной точки
    quickSort(start, pivot - 1);
    // повторяем подмассив, содержащий элементы, превышающие точку опоры
    quickSort(pivot + 1, end);
}

// быстрая сортировка
void MainWindow::on_pushButton_quick_clicked()
{
    warn(rowCount);

    // создаем массив
    if (!createArray())
        return;

    quickSort(0, rowCount - 1);

    outputArray();
}

// cортировка расческой
void MainWindow::on_pushButton_comb_clicked()
{
    warn(rowCount);

    // создаем массив
    if (!createArray())
        return;

    const double factor = 1.247; // Фактор уменьшения
    int step = rowCount - 1;

    while (step >= 1)
    {
        for (int i = 0; i + step < rowCount; ++i)
        {
            if (array[i] > array[i + step])
            {
              double temp = array[i];
              array[i] = array[i + step];
              array[i + step] = temp;
            }
        }
        step /= factor;
    }

    // сортировка пузырьком
    for (int idx_i = 0; idx_i + 1 < rowCount; ++idx_i)
    {
        for (int idx_j = 0; idx_j + 1 < rowCount - idx_i; ++idx_j)
        {
            if (array[idx_j + 1] < array[idx_j])
            {
                double temp = array[idx_j];
                array[idx_j] = array[idx_j + 1];
                array[idx_j + 1] = temp;
            }
        }
    }

    outputArray();
}

// перемешивает
void MainWindow::shuffle()
{
    double temp;
    int random;
       for(int i=0; i < rowCount; i++)
       {
          random = rand() % rowCount;
          temp = array[i];
          array[i] = array[random];
          array[random] = temp;
          outputArray();
       }
}

// пhоверяет на сортировку
bool MainWindow::isSorted()
{
    int rows = rowCount - 1;
    while (--rows > 0)
        if (array[rows - 1] > array[rows])
           return false;
    return true;
}

// сортировка обезьяной
void MainWindow::on_pushButton_monkey_clicked()
{
    if (rowCount > 5)
    {
        QMessageBox::information(this, "Ошибка", "Больше 5 элементов сортировать нельзя", QMessageBox::Ok);
        return;
    }
    // создаем массив
    if (!createArray())
        return;

    while (!isSorted())
        shuffle();

    outputArray();
}

// вывод минимального, среднего и максимального значения
void MainWindow::on_pushButton_output_clicked()
{
    // создаем массив
    if (!createArray())
        return;

    double min = array[0], max = 0, sumOfValues = 0;
    for (int i = 0; i < rowCount; ++i) {
        if (array[i] < min)
            min = array[i];
        if (array[i] > max)
            max = array[i];
        sumOfValues += array[i];
    }
    ui->label_max->setNum(max);
    ui->label_avg->setNum(sumOfValues / rowCount);
    ui->label_min->setNum(min);
}

// заполнение рандомными числами
void MainWindow::on_pushButton_random_clicked()
{

    warn(rowCount);
    for (int i = 0; i < rowCount; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(rand() % 20001 - 10000));
        ui->tableWidget->setItem(i, 0, item);
    }
}

// очистка
void MainWindow::on_pushButton_clear_clicked()
{
    ui->tableWidget->clearContents();
}

// количество строк
void MainWindow::on_spinBox_rowCount_valueChanged(int arg1)
{
    ui->tableWidget->setRowCount(arg1);
    rowCount = arg1;
}

// заполнение одним числом
void MainWindow::on_pushButton_filler_clicked()
{
    warn(rowCount);
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(ui->doubleSpinBox_filler->value()));
        ui->tableWidget->setItem(i, 0, item);
    }
}

// действия при изменении ячейки
void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    changeColor(row, column);
}

// очистка от минимального, среднего и максимального значения
void MainWindow::on_pushButton_clear_2_clicked()
{
    ui->label_min->setNum(0);
    ui->label_max->setNum(0);
    ui->label_avg->setNum(0);
}

// сравнение
bool MainWindow::isEqual(double a, double b)
{
    if (fabs(fabs(a) - fabs(b)) <= 1e-10)
        return true;
    return false;
}

// добавление элементов в таблицу поиска
void MainWindow::addElementInTableSearch(int value, int row)
{
    ui->tableWidget_search->setRowCount(row + 1);
    QTableWidgetItem * item = new QTableWidgetItem;
    ui->tableWidget_search->setItem(row, 0, item);
    ui->tableWidget_search->item(row, 0)->setText(QString::number(value));
}

// поиск дубликатов
void MainWindow::on_pushButton_search_clicked()
{
    ui->tableWidget_search->clear();
    ui->tableWidget_search->setRowCount(0);

    const QString text = ui->lineEdit_search->text();
    if (!checkValue(new QTableWidgetItem(text)))
    {
        QMessageBox::information(this, "Ошибка", "Неверное число для поиска", QMessageBox::Ok);
        return;
    }
    double key = text.toDouble();

    if(!createArray())
        return;

    if (rowCount == 1)
    {
        if (isEqual(array[0], key))
        {
            addElementInTableSearch(1, 0);
            return;
        }
        else
        {
            ui->tableWidget_search->clear();
            ui->tableWidget_search->setRowCount(0);
            QMessageBox::information(this, "Ошибка", "В массиве нет данного числа.", QMessageBox::Ok);
            return;
        }
    }
    // проверка на отсортированость массива
    int counter = 0;
    if (!isSorted())
    {
        // перебор
        for (int i = 0; i < rowCount; i++)
        {
            if (isEqual(array[i], key))
            {
                addElementInTableSearch(i + 1, counter);
                counter++;
            }
        }
    }
    // бинарный поиск
    else
    {
// на этапе разработки
//        int leftBorder = 0; // левая граница
//        int rightBorder = rowCount - 1; // правая граница
//        int prevIndex = -1;

//        int mid;
//        while ((leftBorder <= rightBorder))
//        {
//            mid = (leftBorder + rightBorder) / 2; // считываем срединный индекс отрезка
//            if (isEqual(array[mid], key))
//            {
//                leftBorder= mid + 1;
//                prevIndex = mid;
//                addElementInTableSearch(mid, ++counter);
//            }
//            else if (array[mid] > key)
//                rightBorder = mid - 1; // проверяем, какую часть нужно отбросить
//            else
//                leftBorder= mid + 1;
//        }
        for (int i = 0; i < rowCount; i++)
        {
            if (isEqual(array[i], key))
            {
                addElementInTableSearch(i + 1, counter);
                counter++;
            }
        }
    }
    if (counter == 0)
    {
        ui->tableWidget_search->setRowCount(0);
        ui->tableWidget_search->clear();
        QMessageBox::information(this, "Ошибка", "В массиве нет данного числа.", QMessageBox::Ok);
        return;
    }
}

// удаление дубликатов
void MainWindow::on_pushButton_del_dublicates_clicked()
{
    for (int var = 0; var < rowCount; ++var)
    {

        if(!createArray())
            return;

        double key = array[var];

        bool dublicate = false;
        for (int i = 0; i < rowCount; i++)
        {
            if (isEqual(array[i], key))
            {
                if (!dublicate)
                {
                    dublicate = true;
                    continue;
                }
                for (int j = i + 1; j < rowCount; ++j)
                {
                    array[j - 1] = array[j];
                }
                i--;
                rowCount -= 1;
                ui->tableWidget->setRowCount(rowCount);
                ui->spinBox_rowCount->setValue(rowCount);
                outputArray();
            }
        }
    }
}
