#include "discoverywindow.h"
#include "ui_discoverywindow.h"

#include <QScrollBar>


// --- CONSTRUCTOR ---
DiscoveryWindow::DiscoveryWindow(QWidget *parent) : QWidget(parent),
    ui(new Ui::DiscoveryWindow) {
	ui->setupUi(this);

	// Connections.
	connect(ui->subscribeCheckBox, SIGNAL(toggled(bool)), this, SLOT(subscriptionStatus(bool)));
	connect(ui->clearButton, SIGNAL(pressed()), this, SLOT(clearText()));

	// Defaults
	ui->subscribeCheckBox->setChecked(false);

	ui->tableWidget->setColumnWidth(0, 180);
	ui->tableWidget->setColumnWidth(1, 80);
	ui->tableWidget->setColumnWidth(2, 40);
}


// --- DECONSTRUCTOR ---
DiscoveryWindow::~DiscoveryWindow() {
	delete ui;
}


// --- SET TOPIC ---
void DiscoveryWindow::setTopic(QString topic) {
	this->topic = topic.toStdString();
	setWindowTitle(topic);
}


// --- SUBSCRIPTION STATUS ---
void DiscoveryWindow::subscriptionStatus(bool status) {
	if (status) { emit addSubscription(topic); }
	else { emit removeSubscription(topic); }
}


// --- CLEAR TEXT ---
// Clear the topics found so far.
void DiscoveryWindow::clearText() {
	ui->tableWidget->setRowCount(0);
}


// --- RECEIVE MESSAGE ---
// Called when a new message is available.
void DiscoveryWindow::receiveMessage(string topic_str, string message) {
	set<string>::const_iterator it;
	QString topic = QString::fromStdString(topic_str);
	it = topics.find(topic_str);
	int row = -1;
	for (int i=0; i < ui->tableWidget->rowCount(); i++)
		if (ui->tableWidget->item(i, 0)->text() == topic) {
			row = i; break;
		}
	if (row == -1) {
		// Topic not found. Insert and add to GUI.
		topics.insert(topic_str);
		QString text = topic + "\n";

		row = ui->tableWidget->rowCount();
		ui->tableWidget->insertRow(row);
		ui->tableWidget->setItem(row, 0, new QTableWidgetItem(topic));
		ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(message)));
		ui->tableWidget->setItem(row, 2, new QTableWidgetItem("1", QVariant::Int));
	} else {
		ui->tableWidget->item(row, 1)->setText(QString::fromStdString(message));
		QTableWidgetItem * counter = ui->tableWidget->item(row, 2);
		counter->setText(QString::number(counter->text().toInt() + 1));
	}
}


// --- CLOSE EVENT ---
// Handle the window being closed by the user.
void DiscoveryWindow::closeEvent(QCloseEvent *event) {
	emit windowClosing(topic);
	event->accept();
}

void DiscoveryWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn) {
	QString message = ui->tableWidget->item(currentRow, 1)->text();
	ui->zoomTextEdit->setText(message);
}

void DiscoveryWindow::on_tableWidget_cellDoubleClicked(int row, int column) {
	QString topic = ui->tableWidget->item(row, 0)->text();
	emit topicSelected(topic.toStdString());
}
