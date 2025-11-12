#include "experiment.h"

void DataStructure::generateFile(std::string filepath, int amountOfOperations, int fileType)
{
	if (fileType > 2 || fileType < 0)
		return;

	std::ofstream outF(filepath);
	if (!outF)
	{
		std::cerr << "Error in file: " << filepath << "\n";
		outF.close();
		return;
	}
	std::vector<std::string> operations;
	operations.clear();
	operations.reserve(amountOfOperations);

	int maxRead0;
	int maxWrite0;
	int maxRead1;
	int maxWrite1;
	int maxString;

	if (fileType == 0)//task friendly
	{
		maxRead0 = amountOfOperations * 0.1;	//10%
		maxWrite0 = amountOfOperations * 0.05;	//5%
		maxRead1 = amountOfOperations * 0.5;	//50%
		maxWrite1 = amountOfOperations * 0.1;	//10%
		maxString = amountOfOperations * 0.25;	//25%
	}
	if (fileType == 1)//task equal
	{
		maxRead0 = amountOfOperations * 0.2;	//20%
		maxWrite0 = amountOfOperations * 0.2;	//20%
		maxRead1 = amountOfOperations * 0.2;	//20%
		maxWrite1 = amountOfOperations * 0.2;	//20%
		maxString = amountOfOperations * 0.2;	//20%
	}

	if (fileType == 2) //task opposite
	{
		maxRead0 = amountOfOperations * 0.05;	//5%
		maxWrite0 = amountOfOperations * 0.4;	//40%
		maxRead1 = amountOfOperations * 0.1;	//10%
		maxWrite1 = amountOfOperations * 0.3;	//30%
		maxString = amountOfOperations * 0.15;	//15%
	}

	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution dist(1, 99);

	for (int i = 0; i < maxRead0; i++)
		operations.push_back("read 0");
	for (int i = 0; i < maxWrite0; i++)
		operations.push_back("write 0 " + std::to_string(dist(gen)));
	for (int i = 0; i < maxRead1; i++)
		operations.push_back("read 1");
	for (int i = 0; i < maxWrite1; i++)
		operations.push_back("write 1 " + std::to_string(dist(gen)));
	for (int i = 0; i < maxString; i++)
		operations.push_back("string");

	std::shuffle(operations.begin(), operations.end(), gen);

	for (const auto& op : operations)
		outF << op << "\n";

	outF.close();
}

void DataStructure::executeOperations(const std::vector<std::string>& operations)
{
	for (const auto& operation : operations)
	{
		if (operation == "read 0")
		{
			volatile int avoidOpt = read(0);
			(void)avoidOpt;
		}
		else if (operation.find("write 0") == 0)
		{
			int value = std::stoi(operation.substr(8));
			write(0, value);
		}
		else if (operation == "read 1")
		{
			volatile int avoidOpt = read(1);
			(void)avoidOpt;
		}
		else if (operation.find("write 1") == 0)
		{
			int value = std::stoi(operation.substr(8));
			write(1, value);
		}
		else if (operation == "string")
		{
			volatile std::string avoidOpt = toString();
			(void)avoidOpt;
		}
	}
}

std::vector<std::string> DataStructure::readFile(const std::string& filepath)
{
	std::vector<std::string> fileOperations;
	std::ifstream inpF(filepath);
	if (!inpF)
	{
		std::cerr << "Error in file: " << filepath << "\n";
		inpF.close();
		return fileOperations;
	}

	std::string fileString;
	while (std::getline(inpF, fileString))
	{
		if (!fileString.empty())
		{
			fileOperations.push_back(fileString);
		}
	}
	inpF.close();
	return fileOperations;
}

double DataStructure::threadMeasure(const std::vector<std::string>& filepath)
{
	std::vector<std::vector<std::string>> operations;
	for (const auto& file : filepath)
	{
		operations.push_back(readFile(file));
	}

	std::vector<std::thread> threads;
	threads.reserve(operations.size());

	auto t1 = std::chrono::steady_clock::now();

	for (int i = 0; i < operations.size(); i++)
	{
		threads.emplace_back(([this, i, &operations]()
			{
				this->executeOperations(operations[i]);
			}));
	}

	for (auto& thread : threads)
		thread.join();

	auto t2 = std::chrono::steady_clock::now();

	return std::chrono::duration<double, std::milli>(t2 - t1).count();
}

int DataStructure::read(int fieldIndex) const
{
	switch (fieldIndex)
	{
	case 0:
	{
		std::shared_lock lock(dataMutex0);
		return field0;
	}
	case 1:
	{
		std::shared_lock lock(dataMutex1);
		return field1;
	}
	default:
		return -1;
	}
}

void DataStructure::write(int fieldIndex, int value)
{
	switch (fieldIndex)
	{
	case 0:
	{
		std::unique_lock lock(dataMutex0);
		field0 = value;
		break;
	}
	case 1:
	{
		std::unique_lock lock(dataMutex1);
		field1 = value;
		break;
	}
	default:
		return;
	}
}

std::string DataStructure::toString() const
{
	std::shared_lock lock0(dataMutex0);
	std::shared_lock lock1(dataMutex1);
	return std::to_string(field0) + " " + std::to_string(field1);
}