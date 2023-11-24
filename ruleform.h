#ifndef RULEFORM_H
#define RULEFORM_H

#include <QWidget>
#include "GeneticCore/timetable/timetable.h"

class MainWindow;

namespace Ui {
class RuleForm;
}

/**
 * @brief The RuleForm class used for describing information about subjects
 */

class RuleForm : public QWidget
{
    Q_OBJECT

public:
    explicit RuleForm(QWidget *parent = nullptr, int days=2, int pairs=2);
    MainWindow* parentWindow = nullptr;
    int id;
    ~RuleForm();

    /**
     * @brief selected is dynamic array of checkboxes
     */
    std::vector<std::vector<bool>> selected;

    /**
     * @brief getSubject used for receiving subject name
     * @return string text name of subject
     */
    QString getSubject();

    /**
     * @brief getTeacher used for receiving teacher name
     * @return string text name of teacher
     */
    QString getTeacher();

    /**
     * @brief getAmount used for receiving number of pairs for the subject
     * @return int number of pairs
     */
    int getAmount();

    /**
     * @brief setValues used for imported configuration to fill parameters
     * @param subject is string text name of subject
     * @param teacher is string text name of teacher
     * @param amount is int number of pairs for the subject
     */
    void setValues(QString subject, QString teacher, int amount);

    /**
     * @brief getSubjectData contains id number of line, name of the subject and selected
     * @return pointers to the structure
     */
    Subject* getSubjectData();

private slots:
    /**
     * @brief deletePushButtonClicked used for delete line of the subject
     */
    void deletePushButtonClicked();

    /**
     * @brief editPushButtonClicked used for select pairs
     */
    void editPushButtonClicked();

private:
    int days;
    int pairs;
    Subject subjectData;
    Ui::RuleForm *ui;
};

#endif // RULEFORM_H
