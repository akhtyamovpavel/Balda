#include "wordcollector.h"
#include <QTextStream>

WordCollector::WordCollector(QObject *parent) :
    QObject(parent)
{

}

void WordCollector::endMove() {
    QTextStream out(stdout);
    out << "move ended\n";
    out << "approved word "<< word_ <<"\n";
    emit approveWord();

}

void WordCollector::connectToDictionary(QObject* dictionary) {
    connect(this, SIGNAL(sendToDictionary(const QString&)), dictionary, SLOT(checkWord(const QString&)));
}

void WordCollector::connectToBoard(QObject* board) {
    connect(this, SIGNAL(clearState(const QPair<int,int>&)), board, SLOT(resetState(const QPair<int,int>&)));
    connect(this, SIGNAL(endOfTransaction(const QString&)), board, SLOT(remakeMove(const QString&)));
    connect(this, SIGNAL(approveWord()), board, SLOT(setApproved()));
}

void WordCollector::clearWord() {
    if (!isApproved) {
        emit clearState(changedCell);
    }
    QString sentWord = word_;
    word_.clear();
    xList.clear();
    yList.clear();
    isNew.clear();
    std::cout << "Word ended\n";
    std::cout.flush();
    emit endOfTransaction(sentWord);
}

//slots

void WordCollector::addLetter(QChar letter) {
    word_ += letter;

}

void WordCollector::addX(int x) {
    xList.push_back(x);
}

void WordCollector::addY(int y) {
    yList.push_back(y);

}

int WordCollector::hasApproved() {
    if (isApproved) {
        return 1;
    } else {
        return 0;
    }
}

void WordCollector::addNew(int isNewValue) {
    isNew.push_back(isNewValue);
    QTextStream out(stdout);
    out << word_ << "\n";
}

void WordCollector::addChangedCell(QPair<int,int> cell) {
    changedCell = cell;
}

void WordCollector::checkWord() {
    if (xList.empty()) {
        isApproved = false;
        clearWord();
        return;
    }
    isApproved = true;
    int cntNew = 0;
    for (int i = 1; i < xList.size(); ++i) {
        if (labs(xList[i] - xList[i-1]) + labs(yList[i] - yList[i-1]) != 1) {
            isApproved = false;
        }
    }
    for(int i = 0; i < xList.size(); ++i) {
        if(xList[i] == changedCell.first && yList[i] == changedCell.second) {
            ++cntNew;
        }
    }
    if (cntNew != 1) {
        isApproved = false;
    }

    for (int i = 0; i < xList.size(); ++i) {
        for (int j = i+1; j < yList.size(); ++j) {
            if (xList[i] == xList[j] && yList[i] == yList[j]) {
                isApproved = false;
            }
        }
    }

    if (isApproved) {
        emit sendToDictionary(word_);
    }

    if (isApproved) {
        endMove();
    }
    clearWord();

}

void WordCollector::setWordApproved(int value) {
    isApproved = value;
}
