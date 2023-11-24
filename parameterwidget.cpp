#include "parameterwidget.h"
#include "ui_parameterwidget.h"

ParameterWidget::ParameterWidget(QWidget *parent, QString title, QString name, int parameter, float newMinimum, float newMaximum, float newStep):
    QWidget(parent),
    ui(new Ui::ParameterWidget)
{
    ui->setupUi(this);

    step = newStep;
    this->name = name;
    ui->labelTitle->setText(title);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(chooseParameter(int)));
    ui->horizontalSlider->setMinimum(newMinimum / newStep);
    ui->horizontalSlider->setMaximum(newMaximum / newStep);
    ui->horizontalSlider->setValue(parameter);

}

float ParameterWidget::getCurrentValue() {
    return step * ui->horizontalSlider->value();
}

ParameterWidget::~ParameterWidget()
{
    delete ui;
}

void ParameterWidget::chooseParameter(int value)
{
    ui->labelValue->setText(QString::number(getCurrentValue()) + name);
}

void ParameterWidget::onMinimumChanged(int newMinimum)
{
    ui->horizontalSlider->setMinimum(newMinimum);
}

void ParameterWidget::onMaximumChanged(int newMaximum)
{
    ui->horizontalSlider->setMaximum(newMaximum);
}

void ParameterWidget::stepChanged(double newStep)
{
    ui->horizontalSlider->setSingleStep(newStep);
}

void ParameterWidget::valueChanged(int parameter)
{
    ui->horizontalSlider->setValue(parameter);
}

float ParameterWidget::setCurrentValue(float value)
{
    ui->horizontalSlider->setValue(value / step);
}
