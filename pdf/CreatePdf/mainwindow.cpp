#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtPrintSupport/QPrinter>

#include <QPainter>
#include <QProgressDialog>
#include <QApplication>
#include <QTextDocument>
#include <QTextTableCell>
#include <QTextBlockFormat>
#include <QTextCursor>
#include <QDebug>

#include <QImageReader>
#include <QPrintDialog>
#include <QPdfWriter>
#include "testdata.h"


static const int textMargins = 12; // in millimeters
static const int borderMargins = 10; // in millimeters

static double mmToPixels(QPrinter& printer, int mm)
{
    return mm * 0.039370147 * printer.resolution();
}

static void paintPage(QPrinter& printer, int pageNumber, int pageCount,
                      QPainter* painter, QTextDocument* doc,
                      const QRectF& textRect, qreal footerHeight)
{
    //qDebug() << "Printing page" << pageNumber;
    const QSizeF pageSize = printer.paperRect().size();
    qDebug() << "pageSize=" << pageSize;

    const double bm = mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    painter->drawRect(borderRect);

    painter->save();
    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(), doc->pageSize().width(), doc->pageSize().height());
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    // Footer: page number or "end"
    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    if (pageNumber == pageCount - 1)
        painter->drawText(footerRect, Qt::AlignCenter, QObject::tr("Fin du Bordereau de livraison"));
    else
        painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
}

static void printDocument(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget)
{
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    qDebug() << "pageSize=" << pageSize;
    qDebug() << "pageSizeMM=" << printer.pageSizeMM();
    qDebug() << "pageSize=" << printer.pageSize();
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);
    qDebug() << "textRect=" << textRect;
    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();
    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
             break;

        if (!firstPage)
            printer.newPage();

        paintPage( printer, pageIndex, pageCount, &painter, doc, textRect, footerHeight );
        firstPage = false;
    }
}
static void addTable(QTextCursor& cursor)
{
    const int columns = 4;
    const int rows = 1;

    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount( 1 );
    QTextTable* textTable = cursor.insertTable( rows + 1,
                                                columns,
                                                tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#DADADA" ) );

    QStringList headers;
    headers << "Product" << "Reference" << "Price" << "Price with shipping";
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat( tableHeaderFormat );
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( headers[column] );
    }
    int row = 0;
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( row + 1, column );
        Q_ASSERT( cell.isValid() );
        QTextCursor cellCursor = cell.firstCursorPosition();
        const QString cellText = QString( "A 220.00" );
        cellCursor.insertText( cellText );
    }
    cursor.movePosition( QTextCursor::End );
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPrinter printer(QPrinter::HighResolution);
    QTextDocument * textDocument = new QTextDocument;

    QTextCursor cursor(textDocument);

    textDocument->setHtml(td.getBody());

    QString body =                "<html><body>"
                          "<div>"
                            "<h1>Szkoła Główna Służby Pożarniczej w Warszawie</h1>"
                            "<h1>Zakład Badania Przyczyn Pożarów</h1>"
                          "</div>"
                          "<div>"
                            "<p><b>Temat ćwiczenia</b><br/>"
                            "Badanie dolnej granicy wybuchowości par cieczy palnych</p>"
                          "</div>"
                          "<div><p>Data wykonania testu: YYYY-MM-DD HH:mm:DD<br/>"
                          "<ol>Skład zespołu przeprowadzającego test:"
                               "<li>XXXX</li>"
                                "<li>XXXX</li>"
                                "<li>XXXX</li>"
                          "</ol></p><div>"
                          "<div><p>Nazwa badanej cieczy: XXXX</p>"
                          "<p>Temperatura wewnątrz komory w momencie rozpoczęcia testu: XXXX &#x2103;</p>"
                          "<p>Wilgotność w pomieszczeniu: XXXX %RH</p>"
                          "</div>"
                          "<hr>"
                          "<div><h2>Próba 1</h2>"
                          "<h3>Wynik: negatywny</h3>"
                          "<p>Źródło zapłonu: XXXX </p>"
                          "<p>Ilość dozowanej cieczy: XXXX ml </p></div>"
                          "<div><h4>Warunki początkowe:</h4>"
                          "<p>Temperatura parownika cieczy: XXXX &#x2103;</p>"
                          "<p>Temperatura wewnątrz komory: XXXX  &#x2103;</p>"
                          "<p>Ciśnienie wewnątrz komory: XXXX mbar </p></div>"
                          "<div><h4>Warunki przed zapłonem:</h4>"
                          "<p>Temperatura wewnątrz komory: XXXX &#x2103;</p>"
                          "<p>Ciśnienie wewnątrz komory: XXXX mbar </p>"
                          "<p>Koncentracja par cieczy wyliczona metodą ciśnień cząstkowych: XXXX %</p>"
                          "<p>Odczyty z przetworników stężenia: <br/>"
                          "VOC1 = XXXX %,</br>"
                          "VOC2 = XXXX %,</br>"
                          "O2 = XXXX %,</br>"
                          "CO2 = XXXX %,</br>"
                          "Czujnik 8 = XXXX %.</br></p></div>"
                          "<div>#################################################################</div>"
                          "<div><h2>Próba 2</h2>"
                          "<h3>Wynik: negatywny</h3>"
                          "<p>Źródło zapłonu: XXXX </p></div>"
                          "<div>#################################################################</div>"
                          "<div><h2>Próba 3</h2>"
                          "<h3>Wynik: Pozytywny</h3>"
                          "<p>Źródło zapłonu: XXXX </p>"
                          "<p>Ilość dozowanej cieczy: XXXX + YYYY = ZZZZ ml</p>"
                          "</div><div><h4>Warunki początkowe:</h4>"
                          "<p>Temperatura parownika cieczy: XXXX &#x2103;</p>"
                          "<p>Temperatura wewnątrz komory: XXXX  &#x2103;</p>"
                          "<p>Ciśnienie wewnątrz komory: XXXX mbar</p></div>"
                          "<div><h4>Warunki przed próbą zapłonu:</h4>"
                          "<p>Temperatura wewnątrz komory: XXXX &#x2103;</p>"
                          "<p>Ciśnienie wewnątrz komory: XXXX mbar </p>"
                          "<p>Koncentracja par cieczy wyliczona metodą ciśnień cząstkowych: XXXX %</p>"
                          "<p>!!! UWAGA: w tym momencie wyliczona wartość koncentracji par cieczy, może być "
                          "obarczona błędem, wynikającym ze zmiany temp. wew. komory.</p>"
                          "<p><ol>Odczyty z przetworników stężenia: "
                          "<li>VOC1 = XXXX %,</li>"
                          "<li>VOC2 = XXXX %,</li>"
                          "<li>O2 = XXXX %,</li>"
                          "<li>CO2 = XXXX %,</li>"
                          "<li>Czujnik 8 = XXXX %.</li></p></div>"
                          "<div><h4>Warunki po udanej próbie zapłonu mieszaniny par cieczy z powietrzem</h4>"
                          "<p>Temperatura wewnątrz komory: XXXX  &#x2103;</p>"
                          "<p>Ciśnienie wewnątrz komory: XXXX mbar </p>"
                          "<p><ol>Odczyty z przetworników stężenia: "
                          "<li>VOC1 = XXXX %,</li>"
                          "<li>VOC2 = XXXX %,</li>"
                          "<li>O2 = XXXX %,</li>"
                          "<li>CO2 = XXXX %,</li>"
                          "<li>Czujnik 8 = XXXX %.</li></p></div>"
                          "</body></html>";

    ui->textEdit->setHtml(td.getBody());
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/home/marcin/git.devel.marcin/qt/StanowiskoChemiczne/pdf/CreatePdf/test.pdf");
    printer.setPageSize(QPrinter::A4);
    qDebug() << printer.paperSize(QPrinter::Point);
    qDebug() << printer.paperSize(QPrinter::Millimeter);
    qDebug() << printer.paperSize(QPrinter::DevicePixel);
    printer.setFullPage(true);

    textDocument->print(&printer);
   //QPrintDialog*dlg = new QPrintDialog(&printer,this);
   //dlg->setWindowTitle(QObject::tr("Print Document"));

   //printDocument(printer, textDocument, this);
   //if(dlg->exec() == QDialog::Accepted) {
       //printDocument(printer, textDocument, this);
   //}
   //delete dlg;

   qDebug("Done");

}

MainWindow::~MainWindow()
{
    delete ui;
}

