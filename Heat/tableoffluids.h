#ifndef TABLEOFFLUIDS_H
#define TABLEOFFLUIDS_H
#include "ThermalProperties.h"
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QTableWidgetItem>
/*!
 * \brief The TableOfFluids class
 * holds table with fluids properties which is display on application window
 * \author Łukasz Dyraga
 * \version 1.0
 */
class TableOfFluids
{
public:
    TableOfFluids(QTableWidget &table, ThermalProperties &fluid);
private:
    void setHeaders();
    void setRowsAndColumnsCount();
    void setItems();
    void setTriggersOption();
    void setViewProperties();
    /*!
      * \brief headers of the table
     */
    QStringList verticalHeadersText={
        "temperatura [K]",
        "współ.\nprzewodzenia\nciepła [W/m K]",
        "kinematyczny\nwspół.\nlepkosci [m^2/s]",
        "liczba Prandtl'a"
    };
    /*!
     * \brief holds table widget
     */
    QTableWidget *table;
    /*!
     * \brief holds fluid properties
     */
    ThermalProperties *fluid;
};

#endif // TABLEOFFLUIDS_H
