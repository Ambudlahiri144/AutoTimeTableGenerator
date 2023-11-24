#ifndef SELECTORDIALOG_H
#define SELECTORDIALOG_H

#include <QDialog>
#include "mycheckbox.h"
#include <vector>
#include <QLabel>
#include "ruleform.h"

namespace Ui {
class SelectorDialog;
}

/**
 * @brief The SelectorDialog class used for working with check boxes to establish preferable days and pairs
 */

class SelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectorDialog(RuleForm *parent = nullptr, int days=2, int pairs=2);
    ~SelectorDialog();
    /**
     * @brief changeState change the state in checkbox
     * @param x is int coordinate of day
     * @param y is int coordinate of pair
     * @param newState is bool new state for checkbox
     */
    void changeState(int x, int y, bool newState);

    /**
     * @brief selected is bool array of checkboxes
     */
    std::vector<std::vector<bool>> selected;

private:
    Ui::SelectorDialog *ui;
    RuleForm* parentForm{nullptr};

    /**
     * @brief table is array of pointers to checkboxes
     */
    std::vector<std::unique_ptr<MyCheckBox>> table;

    /**
     * @brief labelFirst is array of pointers to labels number days and pairs
     */
    std::vector<std::unique_ptr<QLabel>> labelFirst;

    /**
     * @brief addCheckBox used for adding checkbox
     * @param x is int coordinate of day
     * @param y is int coordinate of pair
     */
    void addCheckBox(int x, int y);

    /**
     * @brief addLabel used for adding labels
     * @param x is int coordinate of day
     * @param y is int coordinate of pair
     * @param label is string name parameter
     */
    void addLabel(int x, int y, QString label);
};

#endif // SELECTORDIALOG_H
