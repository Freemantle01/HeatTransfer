#include "mainwindow.h"
#include "ui_mainwindow.h"
/*!
 * \brief constructor
 * \param parent provides a main application window
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadLiquidsProperties();
    setPropertiesOfFluidsInTableWidget();
    loadLineEditUserData();
    loadComboBoxes();
    setPictureOfTask();
    setLineEditValidator();
}
/*!
 * \brief
 * loads the sketch image if succeed it will be displayed on application window
 * otherwise the warning dialog box will be shown
 */
void MainWindow::setPictureOfTask(){
    if(taskSketch.load(filePathOfTaskSketch)){
     int width=591, height=459;
     ui->labelTaskPicture->setMinimumWidth(width);
     ui->labelTaskPicture->setMinimumHeight(height);
     ui->labelTaskPicture->setPixmap(taskSketch.scaled(width,height,Qt::KeepAspectRatio));
    }
    else{
        QMessageBox::warning(this,tr("Load problem"),
                             tr("Couldn't load the sketch image"));
    }
}
/*!
 * \brief
 * loads the data inputted by user and starts the solver of heat transfer;
 * function triggered when user click the "solve task" button
 */
void MainWindow::on_pushButtonSolveTask_clicked()
{
    setInputData();
    setEmissivityOfIsolator();
    setForcedConvectionsConstValues();
    //liquid=new ThermalProperties{filePathLiquids[boxTypeOfLiquid->currentIndex()]};
    int indexOfLiquid=boxTypeOfLiquid->currentIndex();
    solveTask=new HeatTransferSolver{dataFromUser,liquid[indexOfLiquid]};
    solveTask->runTheSolver();
    setResultsInLabels(solveTask->getResults());
    delete solveTask;
}
/*!
 * \brief displays the results from solver on application window
 * \param results results values obtained from solver
 */
void MainWindow::setResultsInLabels(const OutputData *results){
    ui->labelTemperatureOnIsolator->setText(QString::number(results->temperatureOnIsolator));
    ui->labelConvectionCoefficient1->setText(QString::number(results->convectionCoefficient1));
    ui->labelConvectionCoefficient2->setText(QString::number(results->convectionCoefficient2));
    ui->labelRadiationCoefficient2->setText(QString::number(results->radiationCoefficient2));
    ui->labelHeatFlowByConvection2->setText(QString::number(results->heatFlowByConvection2));
    ui->labelHeatFlowByRadiation2->setText(QString::number(results->heatFlowByRadiation2));
    ui->labelHeatFlow1->setText(QString::number(results->heatFlow1));
    ui->labelHeatFlow2->setText(QString::number(results->heatFlow2));
}
/*!
 * \brief sets the values of forced convection parameters based on user choice
 */
void MainWindow::setForcedConvectionsConstValues(){
    int choice=boxForcedConvection->currentIndex();
    switch(choice){
    case 0://low viscosity
        dataFromUser.forcedConvectionConstValueC=0.023;
        dataFromUser.forcedConvectionConstValueA=0.8;
        dataFromUser.forcedConvectionConstValueB=0.4;
        break;
    case 1://high viscosity
        dataFromUser.forcedConvectionConstValueC=0.027;
        dataFromUser.forcedConvectionConstValueA=0.8;
        dataFromUser.forcedConvectionConstValueB=0.33;
        break;
    case 2://perpendicular flow
        dataFromUser.forcedConvectionConstValueC=0.283;
        dataFromUser.forcedConvectionConstValueA=0.6;
        dataFromUser.forcedConvectionConstValueB=0.31;
        break;
    }
}
/*!
 * \brief sets the value of emissivity of isolator based on user choice
 */
void MainWindow::setEmissivityOfIsolator(){
    int choice=boxEmissivityOfIsolator->currentIndex();
    switch(choice){
    case 0://low
        dataFromUser.emissivityOfIsolator=0.3;
        break;
    case 1://medium
        dataFromUser.emissivityOfIsolator=0.6;
        break;
    case 2://high
        dataFromUser.emissivityOfIsolator=0.9;
        break;
    }
}
/*!
 * \brief sets the values of properties of fluids in table that is display on application window
 */
void MainWindow::setPropertiesOfFluidsInTableWidget(){
    setTableWidgetsOfFluids();
    for (int i = 0; i < quantityOfLiquids; ++i) {
        TableOfFluids table(*(tableOfFluidWidgets[i]),liquid[i]);
    }
}
/*!
 * \brief sets the address of tables widgets
 */
void MainWindow::setTableWidgetsOfFluids(){
    tableOfFluidWidgets[0]=ui->tableWidgetWater;
    tableOfFluidWidgets[1]=ui->tableWidgetEngineOil;
    tableOfFluidWidgets[2]=ui->tableWidgetGlycerin;
    tableOfFluidWidgets[3]=ui->tableWidgetIsobutane;
    tableOfFluidWidgets[4]=ui->tableWidgetMethanol;
}
/*!
 * \brief loads the address of combo boxes:
 * emissivity of isolator,
 * forced convection,
 * liquid type
 */
void MainWindow::loadComboBoxes(){
   boxEmissivityOfIsolator=ui->comboBoxEmissivityOfIsolator;
   boxForcedConvection=ui->comboBoxForcedConvection;
   boxTypeOfLiquid=ui->comboBoxLiquidType;
}
/*!
 * \brief sets the input data
 */
void MainWindow::setInputData(){
    for (int i=0;i<quantityOfLineEdit;++i) {
        setInputDataFromLineEdit(lineEditUserData[i]);
    }
}
/*!
 * \brief sets the input data written on application window by user
 * \param input input data wrote by user
 */
void MainWindow::setInputDataFromLineEdit(LineEditUserInput &input){
    QString valueText=input.object->text();
    for (int i=0;i<valueText.length();++i) {
        if(valueText[i]==','){
            valueText[i]='.';
        }
    }
   *(input.value)=valueText.toDouble();
}
/*!
 * \brief
 * loads and sets the address of objects
 * that contains the input data written by user
 */
void MainWindow::loadLineEditUserData(){
    lineEditUserData=new LineEditUserInput[quantityOfLineEdit]{
             LineEditUserInput{ui->lineEditLengthOfPipe, &dataFromUser.lengthOfPipe},
             LineEditUserInput{ui->lineEditThicknessOfPipe, &dataFromUser.thicknessOfPipe},
             LineEditUserInput{ui->lineEditInnerDiameterOfPipe,&dataFromUser.innerDiameterOfPipe},
             LineEditUserInput{ui->lineEditThicknessOfIsolator, &dataFromUser.thicknessOfIsolator},
             LineEditUserInput{ui->lineEditMeanVelocityOfLiquid, &dataFromUser.meanVelocityOfLiquid},
             LineEditUserInput{ui->lineEditMeanTemperatureOfLiquid, &dataFromUser.meanTemperatureOfLiquid},
             LineEditUserInput{ui->lineEditTemperatureOfEnvironment, &dataFromUser.temperatureOfEnvironment},
             LineEditUserInput{ui->lineEditThermalConductivityOfIsolator, &dataFromUser.thermalConductivityOfIsolator}
};
}
/*!
 * \brief loads all of the liquids properties
 */
void MainWindow::loadLiquidsProperties(){
    liquid=new ThermalProperties[quantityOfLiquids]{
        ThermalProperties(filePathLiquids[0]),
        ThermalProperties(filePathLiquids[1]),
        ThermalProperties(filePathLiquids[2]),
        ThermalProperties(filePathLiquids[3]),
        ThermalProperties(filePathLiquids[4])
};
}
/*!
 * \brief sets validator which means that user can only inserts natural numbers
 */
void MainWindow::setLineEditValidator(){
    for (int i=0;i<quantityOfLineEdit;++i) {
        lineEditUserData[i].object->setValidator(new QDoubleValidator(0,100000,10,this));
    }
}
/*!
 * \brief destructor that release dynamic allocated data
 */
MainWindow::~MainWindow()
{
    delete[] lineEditUserData;
    delete[] liquid;
    delete ui;
}
/*!
 * \brief saves the input data into the file
 * \param output convenient object for writing text
 */
void MainWindow::saveInputData(QTextStream *output){
    *output<<QString::fromUtf8("<h1>Dane wejściowe</h1>");
    QStringList nameOfVariable={
        ui->labelLengthOfPipe->text(),
        ui->labelThicknessOfPipe->text(),
        ui->labelInnerDiametOfPipe->text(),
        ui->labelThicknessOfIsolator->text(),
        ui->labelMeanVelocityOfLiquid->text(),
        ui->labelTemperatureOfLiquid->text(),
        ui->labelTemperatureOfEnvironment->text(),
        ui->labelThermalConductivityOfIsolator->text(),
    };
    QStringList variableUnit={
        "[m]","[m]","[m]","[m]",
        "[m/s]",
        "[K]","[K]",
        "[W/(m K)]"
    };
    for (int i = 0; i < nameOfVariable.size(); ++i) {
        *output<<nameOfVariable[i]<<"&ensp;"; //&ensp; = \t
        *output<<lineEditUserData[i].object->text()<<"&ensp;";
        *output<<variableUnit[i];
        *output<<"<br>";//new line
    }
    QStringList nameOfVariableComboBox{
        ui->labelForcedConvection->text(),
        ui->labelLiquidType->text(),
        ui->labelEmissivityOfIsolator->text()
    };
    QStringList valueOfComboBox{
        boxForcedConvection->currentText(),
        boxTypeOfLiquid->currentText(),
        boxEmissivityOfIsolator->currentText()
    };
    for (int i = 0; i < nameOfVariableComboBox.size(); ++i) {
        *output<<nameOfVariableComboBox[i]<<"&ensp;"; //&ensp; = \t
        *output<<valueOfComboBox[i];
        *output<<"<br>";//new line
    }
}
/*!
 * \brief saves the output data into the file
 * \param output convenient object for writing text
 */
void MainWindow::saveOutputData(QTextStream *output){
    *output<<QString::fromUtf8("<h1>Dane wyjściowe</h1>");
    QStringList nameOfVariable={
        ui->labelOutputText1->text(),
        ui->labelOutputText2->text(),
        ui->labelOutputText3->text(),
        ui->labelOutputText4->text(),
        ui->labelOutputText5->text(),
        ui->labelOutputText6->text(),
        ui->labelOutputText7->text(),
        ui->labelOutputText8->text(),
    };
    QStringList outputValue={
        ui->labelTemperatureOnIsolator->text(),
        ui->labelConvectionCoefficient1->text(),
        ui->labelConvectionCoefficient2->text(),
        ui->labelRadiationCoefficient2->text(),
        ui->labelHeatFlowByRadiation2->text(),
        ui->labelHeatFlowByConvection2->text(),
        ui->labelHeatFlow1->text(),
        ui->labelHeatFlow2->text()
    };
    QStringList unit={
        "[K]",
        "[W/(m^2 K)]","[W/(m^2 K)]","[W/(m^2 K)]",
        "[W/m^2]","[W/m^2]","[W/m^2]","[W/m^2]"
    };
    for (int i = 0; i < nameOfVariable.size(); ++i) {
        *output<<nameOfVariable[i]<<"&ensp;";
        *output<<outputValue[i]<<"&ensp;";
        *output<<unit[i];
        *output<<"<br>";
    }
}
/*!
 * \brief saves the input data and output data obtained from solver
 * \param file file to which the data will be saved
 */
void MainWindow::saveResultsToFile(QFile *file){
    QTextStream out(file);
    out.setCodec("UTF-8");
    out<<frontHtmlText;
    saveInputData(&out);
    saveOutputData(&out);
    out<<endHtmlText;
}
/*!
 * \brief
 * saves the input data and results into the file (.* .odt .doc .html);
 * function triggered when user click the "save results" button
 */
void MainWindow::on_actionSave_results_triggered()
{
    QString filter="Hypertext Markup Language (*.html) ;; OpenDocument (*.odt) ;; Document file format (*.doc)";
    filter+=" ;; XML Spreadsheet (*.xml)";
    QString currentPath=QDir::currentPath()+"/results";
    QString fileName=QFileDialog::getSaveFileName(this, tr("Save results"),currentPath,filter);
    if(fileName.isEmpty()){
        return;
    }
    else{
        if(find(fileName,".xml")){
            saveResultsToXmlFile(fileName);
            return;
        }
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            saveResultsToFile(&file);
            file.close();
        }
        else{
            QMessageBox::warning(this,"Unable to save results",file.errorString());
        }
    }
}
/*!
 * \brief
 * loads the test data (/test_data/data.txt)
 */
void MainWindow::on_actionLoad_data_test_triggered()
{
    QFile file(pathOfTestData);
    if(file.open(QIODevice::ReadOnly)){
        std::vector<QString> testData=loadTestDataFromFile(&file);
        insertTestDataIntoInputData(testData);
        on_pushButtonSolveTask_clicked();
        file.close();
    }
    else{
        QMessageBox::warning(this,"Problem while loading",file.errorString());
    }
}
/*!
 * \brief loads test data from file
 * \param file file which contains test data
 * \return text from the test data file
 */
std::vector<QString> MainWindow::loadTestDataFromFile(QFile *file){
    QTextStream input(file);
    std::vector<QString> textFromFile(11);
    QString bin="";
    bool isValue=false;
    /* The line in file looks that: "name_of_variable value"
     * the first word is not needed, the second is important,
     * thats why its saved in textFromFile
     * we need 11 values from file so we reads 22 words
     */
    QString value{};
    for (size_t var = 0, i=0; var < 22; ++var) {
        if(isValue){
            input>>textFromFile[i];
            isValue=false;//the next word
            ++i;
        }
        else{
            input>>bin;
            isValue=true;//the next word
        }
    }
    return textFromFile;
}
/*!
 * \brief inserts test data into the input data object
 * \param testData data that will be inserted as input data
 */
void MainWindow::insertTestDataIntoInputData(std::vector<QString> &testData){
    try {
        for (size_t i = 0; i < testData.size(); ++i) {
            for (int j = 0; j < testData[i].size(); ++j) {
                QChar sign=testData[i][j];
                if(!sign.isNumber()) {
                    if(sign=='.'){
                        testData[i][j]=',';
                    }
                    else if(sign!=','){
                       throw std::invalid_argument("");
                    }
                }
            }
            insertTextIntoInputData(testData[i],i);
        }
    } catch (std::invalid_argument error) {
        QMessageBox::warning(this,"Fail while inserting test data",
            "Fail while inserting test data into input data. Check test data file.");
    }
}
/*!
 * \brief inserts text into input data
 * \param text text for line edit
 * \param indexOfInputData index of input data
 */
void MainWindow::insertTextIntoInputData(const QString &text, const unsigned int &indexOfInputData){
    switch (indexOfInputData) {
    case 0:
        ui->lineEditInnerDiameterOfPipe->setText(text);
        break;
    case 1:
        ui->lineEditThicknessOfPipe->setText(text);
        break;
    case 2:
        ui->lineEditMeanVelocityOfLiquid->setText(text);
        break;
    case 3:
        ui->lineEditMeanTemperatureOfLiquid->setText(text);
        break;
    case 4:{
        int index=text.toInt();
        if(index>=0 && index<=4){
            ui->comboBoxLiquidType->setCurrentIndex(index);
        }else{
            ui->comboBoxLiquidType->setCurrentIndex(0);
        }
        break;
    }
    case 5:{
        int index=text.toInt();
        if(index>=0 && index<=2){
            ui->comboBoxForcedConvection->setCurrentIndex(index);
        }else{
            ui->comboBoxForcedConvection->setCurrentIndex(0);
        }
        break;
    }
    case 6:
        ui->lineEditThermalConductivityOfIsolator->setText(text);
        break;
    case 7:
        ui->lineEditThicknessOfIsolator->setText(text);
        break;
    case 8:
        ui->lineEditTemperatureOfEnvironment->setText(text);
        break;
    case 9:{
        int index=text.toInt();
        if(index>=0 && index<=2){
            ui->comboBoxEmissivityOfIsolator->setCurrentIndex(index);
        }else{
            ui->comboBoxEmissivityOfIsolator->setCurrentIndex(0);
        }
        break;
    }
    case 10:
        ui->lineEditLengthOfPipe->setText(text);
        break;
    default:
        break;
    }
}
/*!
 * \brief checks if text contains another text
 * \param text the text in which the word is searched
 * \param search text for search
 * \return true if search text was find otherwise false
 */
bool MainWindow::find(const QString &text, const QString &search){
    if( search.size()==0 || text.size()==0){
        return false;
    }
    int correctLetters{};
    for (int i = 0,j=0; i < text.size(); ++i) {
        if(text[i]==search[j]){
            ++j;
            ++correctLetters;
        }
        else{
            j=0;
            correctLetters=0;
        }
        if(correctLetters==search.size()){
            return true;
        }
    }
    return false;
}
/*!
 * \brief saves results to file with .xml extension
 * \param fileName name of the file
 */
void MainWindow::saveResultsToXmlFile(const QString &fileName){
    XmlWriter *xmlTable=new XmlWriter();
    saveInputAndOutputDataToXml(xmlTable);
    xmlTable->saveTableDataInFile(fileName);
    delete xmlTable;
}
/*!
 * \brief saves input and output data into xml table
 * \param xmlTable xml table
 */
void MainWindow::saveInputAndOutputDataToXml(XmlWriter *xmlTable){
    QString *symbol=new QString[19]{
        "dw", "g1", "w", "T1",
            "lam2", "g2", "T2","l",
            "rodzaj cieczy","konwekcja wymuszona","emisyjność izolacji",
            "T3","a1k","a2k","a2r","q2r",
            "q2k","q1","q2"
    };
    QString *variableName=new QString[19]{
        "średnica wew. przewodu", "grubość ścianki przewodu",
        "średnia prekość cieczy", "średnia temperatura cieczy",
        "wspólcz. przewodzenia ciepla izolacji","grubość ścianki izolacji",
        "temperatura otoczenia", "długość przewodu",
        ui->comboBoxLiquidType->currentText(), ui->comboBoxForcedConvection->currentText(),
        ui->comboBoxEmissivityOfIsolator->currentText(),
        "temperatura na powierzchni zew. izolacji","wspołcz. wnikania ciepła przez konwekcję",
        "wspołcz. wnikania ciepła przez konwekcję", "współcz. wnikania ciepła przez promieniowanie",
        "strumień ciepła traconego przez promieniowanie","strumień ciepła traconego przez konwekcję",
        "strumień ciepła dopływający do pow. izolacji","strumień ciepła odpływający z pow. izolacji"
    };
    QString *value= new QString[19]{
            ui->lineEditInnerDiameterOfPipe->text(),
            ui->lineEditThicknessOfPipe->text(),
            ui->lineEditMeanVelocityOfLiquid->text(),
            ui->lineEditMeanTemperatureOfLiquid->text(),
            ui->lineEditThermalConductivityOfIsolator->text(),
            ui->lineEditThicknessOfIsolator->text(),
            ui->lineEditTemperatureOfEnvironment->text(),
            ui->lineEditLengthOfPipe->text(),
            "","","",
            ui->labelTemperatureOnIsolator->text(),
            ui->labelConvectionCoefficient1->text(),
            ui->labelConvectionCoefficient2->text(),
            ui->labelRadiationCoefficient2->text(),
            ui->labelHeatFlowByRadiation2->text(),
            ui->labelHeatFlowByConvection2->text(),
            ui->labelHeatFlow1->text(),
            ui->labelHeatFlow2->text()
};
    QString *convectionText=new QString[3]{
            "mała lepkość płynu",
            "duża lepkość płynu",
            "przepływ prostopadły"
};
    xmlTable->addRow();
    xmlTable->addCellData("Dane wejściowe",XmlDataType::String);
    for (int i = 0; i < 19; ++i) {
        if(i==11){
            xmlTable->addRow();
            xmlTable->addRow();
            xmlTable->addCellData("Dane wyjściowe",XmlDataType::String);
        }
        xmlTable->addRow();
        if(i==8 || i==9 || i==10){
            xmlTable->addCellData(symbol[i],XmlDataType::String);
            if(i==9){
                int index=ui->comboBoxForcedConvection->currentIndex();
                xmlTable->addCellData(convectionText[index],XmlDataType::String);
            }
            else{
                xmlTable->addCellData(variableName[i],XmlDataType::String);
            }
            xmlTable->addCellData("",XmlDataType::String);
        }
        else{
            xmlTable->addCellData(variableName[i],XmlDataType::String);
            xmlTable->addCellData(symbol[i],XmlDataType::String);
            xmlTable->addCellData(value[i],XmlDataType::Number);
        }

    }
    delete[] convectionText;
    delete[] value;
    delete[] variableName;
    delete[] symbol;
}



