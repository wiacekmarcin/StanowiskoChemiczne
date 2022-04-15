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
    QPdfWriter pdfwriter("/home/marcin/git.devel.marcin/qt/StanowiskoChemiczne/pdf/CreatePdf/test.pdf");
    pdfwriter.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdfwriter);
    painter.drawText(0,0, "testData");
#if 0


    QPrinter printer(QPrinter::PrinterResolution);
    QTextDocument * textDocument = new QTextDocument;

    QTextCursor cursor(textDocument);

    QTextCharFormat plainFormat(cursor.charFormat());

    QTextBlockFormat blockTitleFormat(cursor.blockFormat());
    blockTitleFormat.setAlignment(Qt::AlignCenter);
    blockTitleFormat.setHeadingLevel(1);
    blockTitleFormat.setLineHeight(2.0, QTextBlockFormat::ProportionalHeight);
    cursor.insertBlock();
    cursor.mergeBlockFormat(blockTitleFormat);
    cursor.insertText(tr("Szkoła Główna Służby Pożarniczej w Warszawie") /*,
                  headingFormat*/);
    cursor.insertBlock();
    cursor.insertText(tr("Szkoła Główna Służby Pożarniczej w Warszawie") /*,
                  headingFormat*/);
    cursor.insertBlock();
    cursor.insertText(tr("Zakład Badania Przyczyn Pożarów") /*,
                headingFormat*/);


    //textDocument->setHtml("<html><body><h1>Szkoła Główna Służby Pożarniczej w Warszawie</h1>"\
    "<h1>Zakład Badania Przyczyn Pożarów</h1><p>Jakiś tekst</p></body></html>");
    /*


    QTextCursor cursor(textDocument);

    QTextCharFormat plainFormat(cursor.charFormat());

    QTextBlockFormat blockTitleFormat(cursor.blockFormat());
    blockTitleFormat.setAlignment(Qt::AlignCenter);
    blockTitleFormat.setHeadingLevel(1);
    blockTitleFormat.setLineHeight(2.0, QTextBlockFormat::ProportionalHeight);
    
    QTextCharFormat headingFormat = plainFormat;
    headingFormat.setFontWeight(QFont::Bold);
    headingFormat.setFontPointSize(16);

    //QTextCharFormat emphasisFormat = plainFormat;
    //emphasisFormat.setFontItalic(true);   

    cursor.movePosition(QTextCursor::Start); 

    cursor.mergeBlockFormat(blockTitleFormat);
    cursor.insertBlock(blockTitleFormat);

    cursor.insertText(tr("Szkoła Główna Służby Pożarniczej w Warszawie"),
                  headingFormat);

    cursor.insertBlock(); // Single character

    cursor.insertText(tr("Zakład Badania Przyczyn Pożarów"),
                headingFormat);

    cursor.insertBlock(); // Single character

    //cursor.insertText(tr("a"), emphasisFormat);
    //cursor.insertText(tr("b"), headingFormat);
    //cursor.insertBlock();

    
    cursor.insertText(tr("Text can be displayed in a variety of "
                                "different character formats. "), plainFormat);
    cursor.insertText(tr("We can emphasize text by "));
    //cursor.insertText(tr("making it italic"), emphasisFormat);
*/
    /*
    QTextBlockFormat blockTitleFormat(cursor.blockFormat());
    blockTitleFormat.setAlignment(Qt::AlignCenter);
    blockTitleFormat.setHeadingLevel(1);
    blockTitleFormat.setLineHeight(2.0, QTextBlockFormat::ProportionalHeight);
    
    QTextCharFormat charTitleFormat(cursor.charFormat());
    charTitleFormat.setFontWeight(QFont::Bold);
    charTitleFormat.setFontPointSize(14);

    cursor.setBlockFormat(blockTitleFormat);
    cursor.insertText(QString("Szkoła Główna Służby Pożarniczej w Warszawie\n"), charTitleFormat);
    cursor.movePosition( QTextCursor::End );

    cursor.setBlockFormat(blockTitleFormat);
    cursor.insertText(QString("Zakład Badania Przyczyn Pożarów\n"), charTitleFormat);
    cursor.movePosition( QTextCursor::End );
*/
    //QTextBlockFormat blockSubTitleFormat;
    //blockSubTitleFormat.setAlignment(Qt::AlignCenter);
    //blockTitleFormat.setHeadingLevel(2);

    //cursor.setBlockFormat(blockTitleFormat);
    //cursor.insertText(QString("Szkoła Główna Służby Pożarniczej w Warszawie"));

    //QTextCursor cursorSubTitle(textDocument);
    //cursor.insertText("Zakład Badania Przyczyn Pożarów");


    qDebug("%d", printer.isValid());

    //addTable(cursor);

       //QTextBlockFormat blockFormat;
       //blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
       //cursor.insertBlock(blockFormat);
       //cursor.insertText("This is the second page");


       //QTextBlockFormat blockFormat2;
       //blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
       //cursor.insertBlock(blockFormat2);
       //cursor.insertText("This is the third page");

       //QUrl Uri ( QString ( "file:///home/marcin/Obrazy/mini/Mini/Dzień 15 (17.06)/Dzień 15.01 Plivickie Jeziora/IMG_5885.JPG" ) );
       //QImage image = QImageReader ( "/home/marcin/Obrazy/mini/Mini/Dzień 15 (17.06)/Dzień 15.01 Plivickie Jeziora/IMG_5885.JPG" ).read();



       //textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );

       //QTextBlockFormat blockFormat3;
       //blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
       //cursor.insertBlock(blockFormat3);

       //QTextImageFormat imageFormat;
       //imageFormat.setWidth( image.width() );
       //imageFormat.setHeight( image.height() );
       //imageFormat.setName( Uri.toString() );
       //cursor.insertImage(imageFormat);








    printer.setOutputFormat(QPrinter::PdfFormat);
   printer.setOutputFileName("/home/marcin/git.devel.marcin/qt/StanowiskoChemiczne/pdf/CreatePdf/test.pdf");
   printer.setPaperSize(QPrinter::A4);
   printer.setFullPage(true);

   //QPrintDialog*dlg = new QPrintDialog(&printer,this);
   //dlg->setWindowTitle(QObject::tr("Print Document"));

   printDocument(printer, textDocument, this);
   //if(dlg->exec() == QDialog::Accepted) {
   //    printDocument(printer, textDocument, this);
   //}
   //delete dlg;

   qDebug("Done");
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

