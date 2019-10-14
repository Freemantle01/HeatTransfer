#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPixmap>
#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include "InputData.h"
#include "HeatTransferSolver.h"
#include <QDoubleValidator>
#include <QComboBox>
#include "ThermalProperties.h"
#include "OutputData.h"
#include "tableoffluids.h"
#include <QTableWidget>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDataStream>
#include <QTextStream>
#include <iostream>
#include <QChar>
#include <exception>
#include <QList>
#include "xmlwriter.h"
/*!
 * \brief The LineEditUserInput class
 * used for holding data
 * \author Łukasz Dyraga
 * \version 1.0
 */
class LineEditUserInput{
public:
    /*!
     * \brief holds the object that contains the input data entered in user interface
     */
    QLineEdit *object;
    /*!
     * \brief holds the input data value
     */
    double *value;
};
namespace Ui {
class MainWindow;
}
/*!
 * \brief The MainWindow class
 * that deals with user interface
 * \author Łukasz Dyraga
 * \version 1.2
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButtonSolveTask_clicked();
    void on_actionSave_results_triggered();  
    void on_actionLoad_data_test_triggered();
private:
    void saveResultsToFile(QFile *file);
    void saveInputData(QTextStream *output);
    void saveOutputData(QTextStream *output);
    void saveInputAndOutputDataToXml(XmlWriter *xmlTable);
    void saveResultsToXmlFile(const QString &fileName);
    std::vector<QString> loadTestDataFromFile(QFile *file);
    void loadLineEditUserData();
    void loadComboBoxes();
    void loadLiquidsProperties();
    void setInputData();
    void setInputDataFromLineEdit(LineEditUserInput &input);
    void setTableWidgetsOfFluids();
    void setPropertiesOfFluidsInTableWidget();
    void setPictureOfTask();
    void setLineEditValidator();
    void setEmissivityOfIsolator();
    void setForcedConvectionsConstValues();
    void setResultsInLabels(const OutputData *results);
    void insertTestDataIntoInputData(std::vector<QString> &testData);
    void insertTextIntoInputData(const QString &text, const unsigned int &indexOfInputData);
    bool find(const QString &text ,const QString &search);
    /*!
     * \brief stores many objects connected to user interface
     */
    Ui::MainWindow *ui;
    /*!
     * \brief stores the sketch image
     */
    QPixmap taskSketch;
    /*!
     * \brief stores the file path to the sketch image
     */
    QString filePathOfTaskSketch="task_sketch.png";
    /*!
     * \brief stores the input data from user
     */
    InputData dataFromUser;
    /*!
     * \brief stores the input data entered in UI
     */
    LineEditUserInput *lineEditUserData;
    /*!
     * \brief quantity of QLineEdit
     */
    const int quantityOfLineEdit{8};
    /*!
     * \brief quantity of liquids
     */
    const int quantityOfLiquids{5};
    /*!
     * \brief stores the value of emissivity of isolator inputed by user
     */
    QComboBox *boxEmissivityOfIsolator;
    /*!
     * \brief stores the values of forced convection parameters inputed by user
     */
    QComboBox *boxForcedConvection;
    /*!
     * \brief stores the type of liquid chosen by user
     */
    QComboBox *boxTypeOfLiquid;
    /*!
     * \brief stores the tables of fluids
     */
    QTableWidget *tableOfFluidWidgets[5];
    /*!
     * \brief stores the address of liquid object
     */
    ThermalProperties *liquid;
    /*!
     * \brief stores all file paths for liquids properties
     */
    std::string filePathLiquids[5]={"fluids_properties/water.txt",
                              "fluids_properties/engine_oil_unused.txt",
                              "fluids_properties/glycerin.txt",
                              "fluids_properties/isobutane.txt",
                              "fluids_properties/methanol.txt"};
    /*!
     * \brief stores the address of object that is used for solving the heat transfer problem
     */
    HeatTransferSolver *solveTask;
    /*!
     * \brief stores the front html text that is used for saving files
     */
    QString frontHtmlText={R"(
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=UTF-8"/>
    <meta http-equiv="description" content="Heat transfer results"/>
    <title>Heat transfer results</title>
</head>
<body>
    <div class="page">
        <p>)"};
    /*!
     * \brief stores the end html text that is used for saving files
     */
    QString endHtmlText={R"(
        </p>
    </div>
</body>
</html>)"
    };
    /*!
     * \brief stores the path of test data file
     */
    QString pathOfTestData="test_data/data.txt";
};

#endif // MAINWINDOW_H
