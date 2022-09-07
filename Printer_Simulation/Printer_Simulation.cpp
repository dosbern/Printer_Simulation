// Printer_Simulation.cpp

#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include "omp.h"
#include <mutex>

using namespace std;

//create queues to sort print jobs by length
priority_queue<int, vector<int>, greater<int> > smallPrintJobs;
priority_queue<int, vector<int>, greater<int> > mediumPrintJobs;
priority_queue<int, vector<int>, greater<int> > largePrintJobs;

//function prototypes
void timer();
void createPrintJob(priority_queue<int, vector<int>, greater<int> >& smallPrintJobs, priority_queue<int, vector<int>, greater<int> >& mediumPrintJobs, priority_queue<int, vector<int>, greater<int> >& largePrintJobs);

//constants
//time in ms 60000ms = 60s
CONST int timeBetweenPrintRequestsInMs = 60000;
CONST int timeToPrintPagesInMs = 60000;

int main()
{
    //declare variables
    srand(time(0));
    double totalProcessTimePrinterOne = 0;
    double totalProcessTimePrinterTwo = 0;
    double totalProcessTimePrinterThree = 0;
    int currentPrintJob = 1;
    mutex mutex;

    //create printer class
    class printer
    {
    public:
        string name;
        int currentPrintJob = 0;
        int totalPages = 0;
        priority_queue<int, vector<int>, greater<int> > printingQueue;
        void executePrintJob(int& currentPrintJob, int& totalProcessTime, int& totalPages)
        {
            while (!printingQueue.empty())
            {
                cout << currentPrintJob << ". Printing " << printingQueue.top() << " pages." << endl;
                totalPages = totalPages + printingQueue.top();
                printingQueue.pop();
            }
        };
    };

    //initialize printer objects
    printer printerOne;
    printerOne.name = "printerOne";
    printerOne.totalPages = 0;
    printer printerTwo;
    printerTwo.name = "printerTwo";
    printerTwo.totalPages = 0;
    printer printerThree;
    printerThree.name = "printerThree";
    printerThree.totalPages = 0;

    bool running = true;

    //title screen and startup animation
    cout << "Printer Management Software\n   Program Status: Initialized.";
    for (int i = 0; i < 6; i++)
    {
        Sleep(1000);
        cout << ".";
    }
    cout << endl;

#pragma omp parallel sections
    {
#pragma omp section
        {
            timer();
        }
#pragma omp section
        {
            //queue management algorithm
            while (running == true)
            {
                //printerOne
                if (printerOne.printingQueue.empty())
                {
                    if (!smallPrintJobs.empty())
                    {
                        printerOne.printingQueue.push(smallPrintJobs.top());
                        smallPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && !mediumPrintJobs.empty())
                    {
                        printerOne.printingQueue.push(mediumPrintJobs.top());
                        mediumPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && mediumPrintJobs.empty() && !largePrintJobs.empty())
                    {
                        printerOne.printingQueue.push(largePrintJobs.top());
                        largePrintJobs.pop();
                    }
                }
                //printerTwo
                if (printerTwo.printingQueue.empty())
                {
                    if (!smallPrintJobs.empty())
                    {
                        printerTwo.printingQueue.push(smallPrintJobs.top());
                        smallPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && !mediumPrintJobs.empty())
                    {
                        printerTwo.printingQueue.push(mediumPrintJobs.top());
                        mediumPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && mediumPrintJobs.empty() && !largePrintJobs.empty())
                    {
                        printerTwo.printingQueue.push(largePrintJobs.top());
                        largePrintJobs.pop();
                    }
                }
                //printerThree
                if (printerThree.printingQueue.empty())
                {
                    if (!smallPrintJobs.empty())
                    {
                        printerThree.printingQueue.push(smallPrintJobs.top());
                        smallPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && !mediumPrintJobs.empty())
                    {
                        printerThree.printingQueue.push(mediumPrintJobs.top());
                        mediumPrintJobs.pop();
                    }
                    else if (smallPrintJobs.empty() && mediumPrintJobs.empty() && !largePrintJobs.empty())
                    {
                        printerThree.printingQueue.push(largePrintJobs.top());
                        largePrintJobs.pop();
                    }
                }
                //exit loop conditions
                if (currentPrintJob > 100 && printerOne.printingQueue.empty() && printerTwo.printingQueue.empty() && printerThree.printingQueue.empty())
                {
                    running = false;
                }
            }
        }
#pragma omp section
        {
        //printerOne
            while (running)
            {
                while (!printerOne.printingQueue.empty())
                {
                    mutex.lock();
                    cout << currentPrintJob << ". Printing document on " << printerOne.name << " with " << printerOne.printingQueue.top() << " pages." << endl;
                    currentPrintJob++;
                    mutex.unlock();
                    printerOne.totalPages = printerOne.totalPages + printerOne.printingQueue.top();
                    //printing 10 pages per minute
                    //printing 10 pages per 60000ms
                    //(pages / 10)*60000
                    double numOfPagesConvertToTimeInMS = printerOne.printingQueue.top();
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * 0.167;
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * timeToPrintPagesInMs;
                    Sleep(numOfPagesConvertToTimeInMS);
                    totalProcessTimePrinterOne = totalProcessTimePrinterOne + numOfPagesConvertToTimeInMS;
                    printerOne.printingQueue.pop();
                }
            }
        }
#pragma omp section
        {
            //printerTwo
            while (running)
            {
                while (!printerTwo.printingQueue.empty())
                {
                    mutex.lock();
                    cout << currentPrintJob << ". Printing document on " << printerTwo.name << " with " << printerTwo.printingQueue.top() << " pages." << endl;
                    currentPrintJob++;
                    mutex.unlock();
                    printerTwo.totalPages = printerTwo.totalPages + printerTwo.printingQueue.top();
                    //printing 10 pages per minute
                    //printing 10 pages per 60000ms
                    //(pages / 10)*60000
                    double numOfPagesConvertToTimeInMS = printerTwo.printingQueue.top();
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * 0.167;
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * timeToPrintPagesInMs;
                    Sleep(numOfPagesConvertToTimeInMS);
                    totalProcessTimePrinterTwo = totalProcessTimePrinterTwo + numOfPagesConvertToTimeInMS;
                    printerTwo.printingQueue.pop();
                }
            }
        }
#pragma omp section
        {
            //printerThree
            while (running)
            {
                while (!printerThree.printingQueue.empty())
                {
                    mutex.lock();
                    cout << currentPrintJob << ". Printing document on " << printerThree.name << " with " << printerThree.printingQueue.top() << " pages." << endl;
                    currentPrintJob++;
                    mutex.unlock();
                    printerThree.totalPages = printerThree.totalPages + printerThree.printingQueue.top();
                    //printing 10 pages per minute
                    //printing 10 pages per 60000ms
                    //(pages / 10)*60000
                    double numOfPagesConvertToTimeInMS = printerThree.printingQueue.top();
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * 0.167;
                    numOfPagesConvertToTimeInMS = numOfPagesConvertToTimeInMS * timeToPrintPagesInMs;
                    Sleep(numOfPagesConvertToTimeInMS);
                    totalProcessTimePrinterThree = totalProcessTimePrinterThree + numOfPagesConvertToTimeInMS;
                    printerThree.printingQueue.pop();
                }
            }
        }
    }

    cout << "\n\n" << (currentPrintJob - 1) << " print jobs completed." << endl;
    cout << "printerOne spent " << (totalProcessTimePrinterOne / 60000) << " minutes printing " << printerOne.totalPages << " pages." << endl;
    cout << "printerTwo spent " << (totalProcessTimePrinterTwo / 60000) << " minutes printing " << printerTwo.totalPages << " pages." << endl;
    cout << "printerThree spent " << (totalProcessTimePrinterThree / 60000) << " minutes printing " << printerThree.totalPages << " pages." << endl;
    cout << "The combined total print time was " << ((totalProcessTimePrinterOne + totalProcessTimePrinterTwo + totalProcessTimePrinterThree) / 60000) << " minutes printing " << (printerOne.totalPages + printerTwo.totalPages + printerThree.totalPages) << " pages." << endl;

    cout << "\nProgram Status: Terminated." << endl;

    system("pause");
    return 0;
}

void timer()
{
    //generate print jobs every 60s
    for (int i = 0; i < 100; i++)
    {
        createPrintJob(smallPrintJobs, mediumPrintJobs, largePrintJobs);
        Sleep(timeBetweenPrintRequestsInMs);
    }

}

void createPrintJob(priority_queue<int, vector<int>, greater<int> >& smallPrintJobs, priority_queue<int, vector<int>, greater<int> >& mediumPrintJobs, priority_queue<int, vector<int>, greater<int> >& largePrintJobs)
{
    int numberOfPages = 0;
    numberOfPages = rand() % 49 + 1;
    cout << "Received print request for " << numberOfPages << " pages." << endl;
    if (numberOfPages < 11)
    {
        return smallPrintJobs.push(numberOfPages);
    }
    else if (numberOfPages > 10 && numberOfPages < 21)
    {
        return mediumPrintJobs.push(numberOfPages);
    }
    else if (numberOfPages > 20)
    {
        return largePrintJobs.push(numberOfPages);
    }
};