#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->table = this->ui->tab_warenliste;

    this->setTabOrder(this->ui->ledit_warennr, this->ui->ledit_bezeichnung);
    this->setTabOrder(this->ui->ledit_bezeichnung, this->ui->ledit_preis);
    this->setTabOrder(this->ui->ledit_preis, this->ui->btn_add);
    this->setTabOrder(this->ui->btn_add, this->ui->ledit_warennr);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_add_clicked()
{
    //Lesen der Warennummer
    auto warennummer = this->ui->ledit_warennr->text().trimmed();
    //Überprüfen auf die richtige Länge
    if(warennummer.length()==4)
    {
        //Löschen der Fehlermeldung
        this->ui->statusbar->clearMessage();

        //Die aktuelle Anzahl der Reihen auslesen und eine Hinzufügen
        int i = this->table->rowCount();
        this->table->setRowCount(i+1);     

        //Lesen der restlichen Eingaben
        auto bezeichnung = this->ui->ledit_bezeichnung->text().trimmed();
        auto preis = this->ui->ledit_preis->text().trimmed();

        //Ein QtableWidgetItem erstellen und in die Tabelle schreiben
        auto newItemWnr = new QTableWidgetItem(warennummer);
        auto newItemBz = new QTableWidgetItem(bezeichnung);
        auto newItemPr = new QTableWidgetItem(preis);

        //Warennummer ist nicht Editierbar
        newItemWnr->setFlags(Qt::ItemIsEditable);

        //Aktuelle Zeile ist i da von 0 gezählt wird
        this->table->setItem(i, 0, newItemWnr);
        this->table->setItem(i, 1, newItemBz);
        this->table->setItem(i, 2, newItemPr);

        //Die Eingabe Felder wieder zurücksetzen
        this->ui->ledit_preis->clear();
        this->ui->ledit_warennr->clear();
        this->ui->ledit_bezeichnung->clear();
    }
    else            //Ausgabe der Fehlermeldung
    {
        this->ui->statusbar->showMessage(tr("ERROR! Die Warennummer enthählt nicht 4 Zeichen."));
    }
}

void MainWindow::on_btn_clear_clicked()
{
    //erstellen der Nachrichten Box
    auto mb_nachricht = new QMessageBox();

    //Eingeben vom Text, Anlegen der Buttons und Festlegen des Default buttons
    mb_nachricht->setText(tr("Wollen Sie den Inhalt Wirklich Löschen?"));
    mb_nachricht->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    mb_nachricht->setDefaultButton(QMessageBox::Yes);

    //Auslesen der Button auswahl
    int n = mb_nachricht->exec();

    //Ausführen der Buttons einmal alles Löschen Inhalt plus Zeilen und abbrechen
    switch (n)
    {
    case QMessageBox::Yes:

        //this->table->clear();
        this->table->setRowCount(0);

        break;

    case QMessageBox::Cancel:

        break;
    }

}

void MainWindow::on_actionSave_2_triggered()
{
    QFile file("Warenliste.txt");


    if(file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
    {
        QTextStream out(&file);

        for(int i = 0; i < this->table->rowCount(); i++)
        {
            for(int n = 0; n < 3; n++)
            {
                out << this->table->item(i, n) << ", ";
            }
            out << "\n";
        }
        file.flush();
        file.close();
    }
}

void MainWindow::on_actionOpen_2_triggered()
{

    auto filetable = QFileDialog::getOpenFileName(this, tr("Open Text"), "" , tr("Text files (*.txt)"));

    //Liste aus Datei laden
    QFile file(filetable);
    //Wenn öffnen der Datei erfolgreich war
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        int i = 0;
        //Liste Löschen
        this->table->setRowCount(0);
        //Lese aus Datei in Liste
        QTextStream in(&file);
        while(!in.atEnd())
        {
            this->table->setRowCount(i);
            for(int n = 0; n < 3; n++)
            {
                auto newItem = new QTableWidgetItem(in.readLine().trimmed());
                this->table->setItem(i, n, newItem);
            }
            i++;
        }
        //file schließen
        file.close();
    }//if ende

}
