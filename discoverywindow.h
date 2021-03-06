#ifndef DISCOVERYWINDOW_H
#define DISCOVERYWINDOW_H

#include <QWidget>
#include <QCloseEvent>

#include <string>
#include <set>

using namespace std;


namespace Ui {
    class DiscoveryWindow;
}

class DiscoveryWindow : public QWidget {
    Q_OBJECT

public:
    explicit DiscoveryWindow(QWidget *parent = 0);
    ~DiscoveryWindow();
    
    void setTopic(QString topic);

private:
    Ui::DiscoveryWindow *ui;
    string topic;
	set<string> topics;
    
private slots:
    void subscriptionStatus(bool status);
	void clearText();
	void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
	void on_tableWidget_cellDoubleClicked(int row, int column);

public slots:
    void receiveMessage(string topic, string);
    
signals:
    void addSubscription(string topic);
    void removeSubscription(string topic);
    void windowClosing(string topic);
    
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DISCOVERYWINDOW_H
