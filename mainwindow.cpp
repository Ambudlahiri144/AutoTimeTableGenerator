#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDebug>
#include <memory>
#include <vector>
#include <QFile>
#include <QTime>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->scrollAreaWidgetContents->layout()->setAlignment(Qt::AlignTop);

    // connect buttons
    connect(ui->pushButtonNext, SIGNAL(released()), this, SLOT(nextButtonClicked()));
    connect(ui->pushButtonBack, SIGNAL(released()), this, SLOT(backButtonClicked()));
    connect(ui->pushButtonAdd, SIGNAL(released()), this, SLOT(addButtonClicked()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(pageChanged(int)));
    connect(ui->importButton, SIGNAL(released()), this, SLOT(importClicked()));
    connect(ui->saveConfigurationButton, SIGNAL(released()), this, SLOT(saveConfigurationClicked()));
    connect(ui->runButton, SIGNAL(released()), this, SLOT(startGeneration()));
    connect(ui->resultSelector, SIGNAL(valueChanged(int)), this, SLOT(viewResult(int)));
    connect(ui->exportButton, SIGNAL(released()), this, SLOT(saveTable()));
    pageChanged(0);

    // setup my thread
    connect(&workerThread, SIGNAL(progressSignal(long)), this, SLOT(generationProgress(long)));
    connect(&workerThread, SIGNAL(finishedSignal()), this, SLOT(generationFinished()));

    // create all sliders
    createParameterWidgets( "Select time period", 3, 2, 12, 1, ui->mainLayout, " days");
    createParameterWidgets( "Select the number of pairs per day", 2, 2, 8, 1, ui->mainLayout, " pairs");

    createParameterWidgets( "Conflicts", 25, 10, 100, 1, ui->criteriaLayout);       // 2
    createParameterWidgets( "Time bounds", 20, 0, 10, 0.1, ui->criteriaLayout);     // 3
    createParameterWidgets( "Repeats", 10, -5, 5, 0.1, ui->criteriaLayout);         // 4
    createParameterWidgets( "Gaps", 20, 0, 10, 0.1, ui->criteriaLayout);            // 5
    createParameterWidgets( "Week balance", 10, 0, 10, 0.1, ui->criteriaLayout);    // 6
    createParameterWidgets( "Diversity", 10, -5, 5, 0.1, ui->criteriaLayout);       // 7
    // createParameterWidgets( "Preferred begin and End time", 10, 0, 10, 0.1, ui->criteriaLayout); // 8

    createParameterWidgets( "Generations", 20, 500, 100000, 500, ui->layoutParameters, "");     // 9
    createParameterWidgets( "Population size", 20, 50, 2500, 50, ui->layoutParameters, "");     // 10
    createParameterWidgets( "Crossover chance", 35, 0, 100, 1, ui->layoutParameters, "%");      // 11
    createParameterWidgets( "Mutation chance", 24, 0, 100, 1, ui->layoutParameters, "%");
    createParameterWidgets( "Mutation threshold", 10, 0, 1, 0.005, ui->layoutParameters);

    ui->mainLayout->setAlignment(Qt::AlignTop);
    ui->criteriaLayout->setAlignment(Qt::AlignTop);
    ui->layoutParameters->setAlignment(Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nextButtonClicked()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() + 1);
}

void MainWindow::backButtonClicked()
{
    ui->tabWidget->setCurrentIndex(ui->tabWidget->currentIndex() - 1);
}

void MainWindow::addButtonClicked()
{
    ui->scrollArea->hide();

    addedRules.push_back(std::make_unique<RuleForm>(nullptr, parameters[0].get()->getCurrentValue(), parameters[1].get()->getCurrentValue()));
    ui->scrollAreaWidgetContents->layout()->addWidget(addedRules[addedRules.size() - 1].get());
    addedRules[addedRules.size() - 1].get()->show();
    addedRules[addedRules.size() - 1].get()->id = addedRules.size() - 1;
    addedRules[addedRules.size() - 1].get()->parentWindow = this;
    ui->scrollArea->show();
}

void MainWindow::removeRule(int objectIndex) {
    addedRules.erase(addedRules.begin() + objectIndex);
    for (int currentIndex{objectIndex}; currentIndex < (int)this->addedRules.size(); currentIndex++) {
        addedRules[currentIndex].get()->id -= 1;
    }
}

void MainWindow::pageChanged(int index)
{
    if (index==4){
        ui->pushButtonNext->hide();
        // update progressbar
    }else{
        ui->pushButtonNext->show();
    }

    if (index<=1){
        ui->pushButtonBack->hide();
    }else{
        ui->pushButtonBack->show();
    }
}

void MainWindow::createParameterWidgets(QString name, int value, int start, int end, float step, QVBoxLayout* target, QString suffix){
    std::unique_ptr<ParameterWidget> testWidget = std::make_unique<ParameterWidget>(this, name, suffix, value, start, end, step);
    target->addWidget(testWidget.get());
    testWidget.get()->show();
    parameters.push_back(std::move(testWidget));
}

void MainWindow::importClicked()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Open JSON File", "", "JSON Files (*.json)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
            if (!jsonDoc.isNull()) {
                QJsonObject data = jsonDoc.object();

                for (int i=0; i<(int)parameters.size(); i++){
                    parameters[i].get()->setCurrentValue(data.value("Slider_" + QString::number(i)).toDouble());
                }

                for (const QJsonValue& value : data.value("rules").toArray()) {
                    QJsonObject object = value.toObject();
                    addedRules.push_back(std::make_unique<RuleForm>(nullptr, parameters[0].get()->getCurrentValue(), parameters[1].get()->getCurrentValue()));
                    RuleForm* addedRule = addedRules[addedRules.size() - 1].get();
                    ui->scrollAreaWidgetContents->layout()->addWidget(addedRule);
                    addedRule->show();
                    addedRule->id = addedRules.size() - 1;
                    addedRule->parentWindow = this;
                    addedRule->setValues(object.value("Subject").toString(), object.value("Teacher").toString(), object.value("Amount").toInt());

                    QJsonArray matrix = object.value("Selected").toArray();
                    int j=0;
                    for (const QJsonValue& rowValue : matrix){
                        int i=0;
                        QJsonObject object = value.toObject();
                        for (const QJsonValue& cell : rowValue.toArray()){
                            addedRule->selected[j][i]=cell.toBool();
                            i++;
                        }
                        j++;
                    }
                }
            } else {
                qDebug() << "Failed to parse JSON document.";
            }
        } else {
            qDebug() << "Failed to open file:" << file.errorString();
        }
    }
}

void MainWindow::saveConfigurationClicked()
{
    QJsonObject data;

    for (int i=0; i<(int)parameters.size(); i++){
        data.insert("Slider_" + QString::number(i), parameters[i].get()->getCurrentValue());
    }

    QJsonArray ruleArray;

    for (int i = 0; i < (int)addedRules.size() ; i++){
        QJsonObject ruleObject;
        ruleObject.insert("Subject", addedRules[i].get()->getSubject());
        ruleObject.insert("Teacher", addedRules[i].get()->getTeacher());
        ruleObject.insert("Amount", addedRules[i].get()->getAmount());

        std::vector<std::vector<bool>> matrix = addedRules[i].get()->selected;
        QJsonArray ruleMatrix;

        for (const auto& row : matrix)
        {
            QJsonArray jsonRow;
            for (const auto& value : row)
            {
                jsonRow.append(value);
            }
            ruleMatrix.append(jsonRow);
        }
        ruleObject.insert("Selected", ruleMatrix);
        ruleArray.append(ruleObject);
    }
    data.insert("rules", ruleArray);

    QJsonDocument result(data);

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Сохранить JSON файл", "", "JSON Files (*.json)");
    // if user choose file
    if (!fileName.isEmpty()) {
        // creation file for notes
        QFile file(fileName);

        // attemp to open file for notes
        if (file.open(QIODevice::WriteOnly)) {
            // note QJsonDocument into the file
            file.write(result.toJson());
            file.close();
            qDebug() << "File is saved:" << fileName;
        } else {
            qDebug() << "Error in opening file for notes";
        }
    }
}


void MainWindow::startGeneration() {
    this->ui->runButton->setEnabled(false);
    ui->pushButtonBack->setEnabled(false);

    // initalize vector of GA parameters
    std::vector<int> inputParams = {(int)parameters[10]->getCurrentValue(),
                                    (int)parameters[11]->getCurrentValue(),
                                    (int)parameters[12]->getCurrentValue()};

    // initialize vector of weights
    std::vector<float> weights;
    for (int wI{2}; wI<8; wI++) {
        weights.push_back(parameters[wI]->getCurrentValue());
    }

    std::map<Subject*, int> classesRules;
    // parse subject data
    for (int ruleI{0}; ruleI < (int)addedRules.size(); ruleI++) {
        classesRules[addedRules[ruleI].get()->getSubjectData()] = addedRules[ruleI].get()->getAmount();
        // qDebug() << classesRules[ruleI]
    }

    // create new population and set it to the thread
    workerThread.population = std::make_unique<Population>(parameters[9]->getCurrentValue(), parameters[0]->getCurrentValue(), parameters[1]->getCurrentValue(), inputParams, weights);
    workerThread.population.get()->setCrossoverMode(1);
    workerThread.population.get()->setClassesAmount(classesRules);

    workerThread.iterations = parameters[8].get()->getCurrentValue();
    workerThread.start();
}

void MainWindow::generationProgress(long generation) {
    int totalGenerations = parameters[8].get()->getCurrentValue();
    ui->progressBar->setValue((int)(100.0f * generation / totalGenerations));
    auto runTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - workerThread.startTime);

    // calulate time
    QTime execTime(0, 0, 0);
    QTime leftTime(0, 0, 0);
    execTime = execTime.addMSecs(runTime.count());
    leftTime = leftTime.addMSecs((totalGenerations - generation) * workerThread.lastExecutionTime.count() / workerThread.step);

    ui->labelTime->setText("Elasped: " + execTime.toString("h:mm:ss") + " / Left: " + leftTime.toString("h:mm:ss"));
}

void MainWindow::generationFinished() {
    qDebug() << "finished generation";
    ui->runButton->setEnabled(true);
    ui->progressBar->setValue(100);
    setTimetableOutput(workerThread.population.get()->getBestResult());

    bestResultBuffer.clear();
    while(!workerThread.population->bestResults.empty()) {
        bestResultBuffer.push_back(workerThread.population->bestResults.top());
        workerThread.population->bestResults.pop();
    }

    ui->resultSelector->setEnabled(true);
    ui->exportButton->setEnabled(true);
    ui->pushButtonBack->setEnabled(true);
    ui->resultSelector->setValue(1);

    viewResult(1);
}

void MainWindow::setTimetableOutput(Timetable* table) {
    int daysCount = parameters[0].get()->getCurrentValue();
    int slotsCount = parameters[1].get()->getCurrentValue();

    outputTableModel.clear();
    outputTableModel.setColumnCount(daysCount);
    outputTableModel.setRowCount(slotsCount);

    std::map<std::pair<int, int>, Subject*> structuredTimetable;

    // init table
    for (int dayI{0}; dayI < daysCount; dayI++) {
        for (int slotI{0}; slotI < slotsCount; slotI++) {
            structuredTimetable[std::pair<int, int>(dayI, slotI)] = nullptr;
        }
    }

    // add classes
    for (auto currentClass : table->classes) {
        structuredTimetable[std::pair<int, int>(currentClass.day, currentClass.order)] = currentClass.subject;
    }

    // output timetable
    for (int dayI{0}; dayI < daysCount; dayI++) {
        for (int slotI{0}; slotI < slotsCount; slotI++) {
            if (structuredTimetable[std::pair<int, int>(dayI, slotI)] == nullptr) {
                outputTableModel.setData(outputTableModel.index(slotI, dayI), "---");
            } else {
                outputTableModel.setData(outputTableModel.index(slotI, dayI), QString::fromStdString( structuredTimetable[std::pair<int, int>(dayI, slotI)]->name +
                                                                                                     " | " + structuredTimetable[std::pair<int, int>(dayI, slotI)]->teacher));
            }
        }
    }

    ui->outputTable->setModel(&outputTableModel);
    ui->outputTable->resizeColumnsToContents();
}

void MainWindow::viewResult(int position) {
    qDebug() << position;
    qDebug() << bestResultBuffer[bestResultBuffer.size() - position].currentScore;
    setTimetableOutput(&bestResultBuffer[bestResultBuffer.size() - position]);
}

void MainWindow::saveTable() {
    // receive path to file for saving with QFileDialog
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save CSV File", "", "CSV Files (*.csv)");

    if (!filePath.isEmpty()) {
        exportCurrentTable(filePath);
    }
}

void MainWindow::exportCurrentTable(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "file IO error";
        return;
    }

    QTextStream out(&file);

    // write data
    for (int row = 0; row < outputTableModel.rowCount(); ++row)
    {
        for (int column = 0; column < outputTableModel.columnCount(); ++column)
        {
            QString data = outputTableModel.data(outputTableModel.index(row, column)).toString();
            out << data;
            if (column < outputTableModel.columnCount() - 1)
                out << ';';
        }
        out << '\n';
    }
    file.close();
}

void GAThread::run(void) {
    startTime = std::chrono::high_resolution_clock::now();
    population->generateRandom();
    for (long current{0}; current < (iterations / step); current++) {
        auto iterStartTime = std::chrono::high_resolution_clock::now();
        population.get()->evolve(step);
        lastExecutionTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - iterStartTime);
        emit progressSignal(current * step);
    }
    emit finishedSignal();
}
