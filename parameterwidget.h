#ifndef PARAMETERWIDGET_H
#define PARAMETERWIDGET_H

#include <QWidget>

namespace Ui {
class ParameterWidget;
}

/**
 * @brief The ParameterWidget class used for all parameters (sliders)
 */

class ParameterWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief ParameterWidget is a constructor
     * @param parent is pointer to parent widget
     * @param title is string name of slider
     * @param name is string name of units of measurement
     * @param parameter is int parameter of slider fixible value
     * @param newMinimum is float parameter of slider start value
     * @param newMaximum is float parameter of slider end value
     * @param newStep is float parameter of slider step value
     */
    ParameterWidget(QWidget *parent = nullptr, QString title = "Empty", QString name = " cm", int parameter = 28, float newMinimum = 0, float newMaximum = 10, float newStep = 2);

    /**
     * @brief getCurrentValue used for slider value
     * @return float value of slider
     */
    float getCurrentValue();

    /**
     * @brief setCurrentValue used for filling slider value
     * @param value is float parameter of current value
     * @return -
     */
    float setCurrentValue(float value);
    ~ParameterWidget();

private slots:
    /**
     * @brief chooseParameter used for filling the label
     * @param value is int parameter of id
     */
    void chooseParameter(int value);

    /**
     * @brief onMinimumChanged used for setting the minimum of slider
     * @param newMinimum is int parameter of slider minimum
     */
    void onMinimumChanged(int newMinimum);

    /**
     * @brief onMaximumChanged used for setting the maximum of slider
     * @param newMinimum is int parameter of slider maximum
     */
    void onMaximumChanged(int newMinimum);

    /**
     * @brief stepChanged used for setting the step of slider
     * @param newStep is double parameter of slider step
     */
    void stepChanged(double newStep);

    /**
     * @brief valueChanged used for setting the value of slider
     * @param parameter is int parameter of slider value
     */
    void valueChanged(int parameter);

private:
    Ui::ParameterWidget *ui;
    float step;
    QString name;
};

#endif // PARAMETERWIDGET_H
