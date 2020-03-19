#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setFixedSize(250,250);
  boardWdget = new board(this,200,0);
  ui->verticalLayout->addWidget(boardWdget);
}

MainWindow::~MainWindow()
{
  delete ui;
}
