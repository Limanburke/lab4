#include "experiment.h"

int main()
{
	DataStructure data;

	int amountOfOperations = 1000000;

	data.generateFile("taskFriendlyA.txt", amountOfOperations, 0);
	data.generateFile("taskEqualA.txt", amountOfOperations, 1);
	data.generateFile("taskOppositeA.txt", amountOfOperations, 2);

	data.generateFile("taskFriendlyB.txt", amountOfOperations, 0);
	data.generateFile("taskEqualB.txt", amountOfOperations, 1);
	data.generateFile("taskOppositeB.txt", amountOfOperations, 2);

	data.generateFile("taskFriendlyC.txt", amountOfOperations, 0);
	data.generateFile("taskEqualC.txt", amountOfOperations, 1);
	data.generateFile("taskOppositeC.txt", amountOfOperations, 2);

	double time1 = data.threadMeasure({ "taskFriendlyA.txt" });
	double time2 = data.threadMeasure({ "taskEqualA.txt" });
	double time3 = data.threadMeasure({ "taskOppositeA.txt" });

	double time4 = data.threadMeasure({ "taskFriendlyA.txt", "taskFriendlyB.txt"});
	double time5 = data.threadMeasure({ "taskEqualA.txt", "taskEqualB.txt" });
	double time6 = data.threadMeasure({ "taskOppositeA.txt", "taskOppositeB.txt" });

	double time7 = data.threadMeasure({ "taskFriendlyA.txt", "taskFriendlyB.txt", "taskFriendlyC.txt"});
	double time8 = data.threadMeasure({ "taskEqualA.txt", "taskEqualB.txt", "taskEqualC.txt" });
	double time9 = data.threadMeasure({ "taskOppositeA.txt", "taskOppositeB.txt", "taskOppositeC.txt" });

	std::cout << "============================================================";
	std::cout << "\nThread Count\tTask Friendly\tTask Equal\tTask Opposite";
	std::cout << "\n1\t\t" << time1 << "\t\t" << time2 << "\t\t" << time3 << "\n";
	std::cout << "\n2\t\t" << time4 << "\t\t" << time5 << "\t\t" << time6 << "\n";
	std::cout << "\n3\t\t" << time7 << "\t\t" << time8 << "\t\t" << time9 << "\n";
	std::cout << "============================================================";
}