#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include "game_engine.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setFixedSize(550,550);
  eng = new GameEngine(500,0);
  ui->verticalLayout->addWidget(eng->getBoard());
}

MainWindow::~MainWindow()
{
  delete ui;
}
