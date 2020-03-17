#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setFixedSize(550,550);
  boardWdget = new board(this,500,0);
  ui->verticalLayout->addWidget(boardWdget);
}

MainWindow::~MainWindow()
{
  delete ui;
}
