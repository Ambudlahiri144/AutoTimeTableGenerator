#include "selectordialog.h"
#include "ui_selectordialog.h"
#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QCheckBox>
#include <QLabel>

SelectorDialog::SelectorDialog(RuleForm *parent, int days, int pairs) :
    QDialog(parent),
    ui(new Ui::SelectorDialog)
{
    ui->setupUi(this);
    this->parentForm = parent;
    for (int i=0; i < pairs+2; i++){
        for (int j=0; j < days+2; j++){
            if ((i == 1 && j == 0) || (j == 1 && i == 0) || (j == 1 && i == 1)){
                addLabel(i, j, QString(" "));
            }else if (i == 1 ){
                addLabel(i, j, QString().number(j - 1));
            }else if (j == 1 ){
                addLabel(i, j, QString().number(i - 1));
            }else{
                addCheckBox(i,j);
            }
        }
    }
    ui->gridLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

SelectorDialog::~SelectorDialog()
{
    delete ui;
}

void SelectorDialog::addCheckBox(int x, int y){
    table.push_back(std::make_unique<MyCheckBox>(x, y));
    table[table.size() - 1].get()->parentClass = this;
    table[table.size() - 1].get()->setChecked(this->parentForm->selected[y][x]);
    ui->gridLayout->addWidget(table[table.size() - 1].get(), x, y);
    table[table.size() - 1].get()->show();
}

void SelectorDialog::addLabel(int x, int y, QString label){
    labelFirst.push_back(std::make_unique<QLabel>(label));
    ui->gridLayout->addWidget(labelFirst[labelFirst.size() - 1].get(), x, y);
    labelFirst[labelFirst.size() - 1].get()->show();
}

void SelectorDialog::changeState(int x, int y, bool newState) {
    this->parentForm->selected[y][x]=newState;
    for (int i = 0; i < table.size(); i++){
        if (x == 0 && y == 0){
            table[i].get()->setChecked(newState);
            this->parentForm->selected[table[i].get()->y][table[i].get()->x]=newState;
        }else if(x == 0){
            if (table[i].get()->y==y){
                table[i].get()->setChecked(newState);
                this->parentForm->selected[table[i].get()->y][table[i].get()->x]=newState;
            }
        }else if (y == 0){
            if (table[i].get()->x==x){
                table[i].get()->setChecked(newState);
                this->parentForm->selected[table[i].get()->y][table[i].get()->x]=newState;
            }
        }
    }
}
