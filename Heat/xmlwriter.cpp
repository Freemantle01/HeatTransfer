#include "xmlwriter.h"
/*!
 * \brief default constructor
 */
XmlWriter::XmlWriter()
{

}
/*!
 * \brief adds row to the table
 */
void XmlWriter::addRow(){
    if(wasRowAdded){
        tableText+="</Row>";
        tableText+="<Row>";
    }
    else{
        tableText+="<Row>";
        wasRowAdded=true;
    }
}
/*!
 * \brief adds cell data
 * \param text text of cell data
 * \param type type of cell data
 */
void XmlWriter::addCellData(const QString &text,const XmlDataType type){
    switch (type) {
    case XmlDataType::String:
        tableText+=R"(<Cell><Data ss:Type="String">)"+text+R"(</Data></Cell>)";
        break;
    case XmlDataType::Number:
        QString temp=text;
        for (int i = 0; i < temp.size(); ++i) {
            if(temp[i]==','){
                temp[i]='.';
            }
        }
        tableText+=R"(<Cell><Data ss:Type="Number">)"+temp+R"(</Data></Cell>)";
        break;
    }
}
/*!
 * \brief saves table data in file as .xml
 * \param fileName name of file
 */
void XmlWriter::saveTableDataInFile(const QString &fileName){
   QFile file(fileName);
   if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
       QMessageBox::warning(nullptr, "Problem while saving", file.errorString());
   }
   else{
       QTextStream out(&file);
       out.setCodec("UTF-8");
       if(wasRowAdded){
           tableText+="</Row>";
           wasRowAdded=false;
       }
       out<<frontXmlText;
       out<<tableText;
       out<<endXmlText;
   }
}
