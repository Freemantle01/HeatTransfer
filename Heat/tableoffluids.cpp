#include "tableoffluids.h"


/*!
 * \brief constructor, sets attributes and starts the all of the private functions
 * \param tableInput table widget
 * \param fluidInput fluid properties
 */
TableOfFluids::TableOfFluids(QTableWidget &tableInput, ThermalProperties &fluidInput):
    table{&tableInput},fluid{&fluidInput}
{
    setTriggersOption();
    setRowsAndColumnsCount();
    setHeaders();
    setItems();
    setViewProperties();
}
/*!
 * \brief disable edit mode in table
 */
void TableOfFluids::setTriggersOption(){
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
/*!
 * \brief sets quantity of rows and columns of the table
 */
void TableOfFluids::setRowsAndColumnsCount(){
    table->setRowCount(fluid->temperature.size());
    table->setColumnCount(verticalHeadersText.size());
}
/*!
 * \brief sets the headers of the table
 */
void TableOfFluids::setHeaders(){
    table->setHorizontalHeaderLabels(verticalHeadersText);
}
/*!
 * \brief sets the values in the table
 */
void TableOfFluids::setItems(){
    for (size_t i=0;i<table->rowCount();++i) {
        for (size_t j=0;j<table->columnCount();++j) {
            switch(j){
            case 0:
                table->setItem(i,j,new QTableWidgetItem(QString::number(fluid->temperature[i])));
                break;
            case 1:
                table->setItem(i,j,new QTableWidgetItem(QString::number(fluid->thermalConductivity[i])));
                break;
            case 2:
                table->setItem(i,j,new QTableWidgetItem(QString::number(fluid->kinematicViscosity[i])));
                break;
            case 3:
                table->setItem(i,j,new QTableWidgetItem(QString::number(fluid->prandtlNumber[i])));
                break;
            }
            table->item(i,j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}
/*!
 * \brief sets the view properties of the table
 */
void TableOfFluids::setViewProperties(){
table->verticalHeader()->hide();
table->horizontalHeader()->setMinimumHeight(80);
}
