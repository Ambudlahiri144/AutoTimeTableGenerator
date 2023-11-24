#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QVBoxLayout>
#include "ruleform.h"
#include <vector>
#include <memory>
#include "parameterwidget.h"
#include "GeneticCore/ga/ga.h"
#include <QThread>
#include <memory>
#include <chrono>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


/**
 * @brief The GAThread class is used to run evolution and not block main program
 */
class GAThread : public QThread {
    Q_OBJECT

public:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime; ///< start time of thread
    std::chrono::milliseconds lastExecutionTime;  ///< last evolution time
    std::unique_ptr<Population> population;  ///< population to evolve
    long iterations{0}; ///< iterations to evolve
    const long step = 200; ///< signal step

    /**
     * @brief run starts the thread
     */
    void run(void);

signals:
    /**
     * @brief progressSignal emits when step is run
     */
    void progressSignal(long );

    /**
     * @brief finishedSignal emits when finished
     */
    void finishedSignal(void );

};

/**
 * @brief The MainWindow class used for working in main window
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief removeRule used for deleting whole line subject
     * @param objectIndex is int number of this subject
     */
    void removeRule(int objectIndex);

    /**
     * @brief createParameterWidgets used for creating new sliders
     * @param name is string parameter of slider name
     * @param value is int default value of slider
     * @param start is int default start value of slider
     * @param end is int default end value of slider
     * @param step is float default step value of slider
     * @param target is pointer to layout
     * @param suffix is string parameter name of slider value
     */
    void createParameterWidgets(QString name, int value, int start, int end, float step, QVBoxLayout* target, QString suffix="x");

    /**
     * @brief parameters is pointer's array of slider's parameters
     */
    std::vector<std::unique_ptr<ParameterWidget>> parameters;


private slots:
    void nextButtonClicked();
    void backButtonClicked();
    void addButtonClicked();
    void pageChanged(int index);
    void importClicked();
    void saveConfigurationClicked();
    void startGeneration();
    void generationProgress(long generation);
    void generationFinished();
    void viewResult(int position);
    void saveTable();

private:
    Ui::MainWindow *ui;

    /**
     * @brief addedRules is pointer's array of subject's information(name, teacher, amount)
     */
    std::vector<std::unique_ptr<RuleForm>> addedRules;
    GAThread workerThread;  ///< used thread
    std::vector<Timetable> bestResultBuffer;  ///< buffer of best GA results

    /**
     * @brief setTimetableOutput draw timetable to output
     * @param table Timetable to draw
     */
    void setTimetableOutput(Timetable* table);

    /**
     * @brief exportCurrentTable exports the selected timetable
     * @param filePath path to save
     */
    void exportCurrentTable(const QString& filePath);

    QStandardItemModel outputTableModel; ///< output data model

};
#endif // MAINWINDOW_H
