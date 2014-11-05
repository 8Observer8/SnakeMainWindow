#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->size() );

    m_statusLabel = new QLabel;
    m_statusLabel->setText( "4/20" );
    ui->statusBar->addPermanentWidget( m_statusLabel, 1 );

    connect( ui->widget, SIGNAL(signalSetStatus( QString ) ),
             this, SLOT( slotShowStatus( QString ) ) );

    connect( this, SIGNAL( signalNewGame() ),
             ui->widget, SLOT( initSnake() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotShowStatus( const QString &status )
{
    m_statusLabel->setText( QString( "%1/20" ).arg( status ) );
    int num = status.toInt();
    if ( num == 20 ) {
        QMessageBox::information( this, tr("Congratulation!"), tr( "You win!" ) );
    }

}

void MainWindow::on_actionNew_triggered()
{
    emit signalNewGame();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
