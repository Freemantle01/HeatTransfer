#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QFile>
#include <string>
#include <QString>
#include <QMessageBox>
#include <QIODevice>
#include <QTextStream>
/*!
 * \brief The XmlDataType enum
 * class stores types of xml data
 */
enum class XmlDataType{
    String, Number
};
/*!
 * \brief The XmlWriter class
 * creates xml file for excel/calc usage
 * \author Łukasz Dyraga
 * \version 1.0
 */
class XmlWriter
{
public:
    XmlWriter();
    void addRow();
    void addCellData(const QString &text,const XmlDataType type);
    void saveTableDataInFile(const QString &fileName);
private:
    /*!
     * \brief flag which stores true if row was added otherwise false
     */
    bool wasRowAdded{false};
    /*!
     * \brief stores table text
     */
    QString tableText{};
    /*!
     * \brief stores front xml text
     */
    const QString frontXmlText=R"(<?xml version="1.0" encoding="UTF-8"?>
<?mso-application progid="Excel.Sheet"?>
<Workbook xmlns="urn:schemas-microsoft-com:office:spreadsheet"
xmlns:x="urn:schemas-microsoft-com:office:excel"
xmlns:ss="urn:schemas-microsoft-com:office:spreadsheet"
xmlns:html="https://www.w3.org/TR/html401/">
<Worksheet ss:Name="Sheet1">
<Table>
<Column ss:Index="1" ss:Width="250"/>
<Column ss:Index="2" ss:Width="130"/>
<Column ss:Index="3" ss:Width="50"/>)";
    /*!
     * \brief stores end xml text
     */
    const QString endXmlText=R"(
</Table>
</Worksheet>
</Workbook>)";
};
#endif // XMLWRITER_H
