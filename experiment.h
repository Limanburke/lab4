#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <shared_mutex>

class DataStructure
{
	int field0 = 0;
	int field1 = 0;
	std::string toString() const;

	mutable std::shared_mutex dataMutex0;
	mutable std::shared_mutex dataMutex1;

	void executeOperations(const std::vector<std::string>& operations);
	int read(int fieldIndex) const;
	void write(int fieldIndex, int value);

public:
	DataStructure() = default;
	DataStructure(const DataStructure&) = delete;
	DataStructure &operator=(const DataStructure&) = delete;
	~DataStructure() = default;


	void generateFile(std::string filepath, int amountOfOperations, int fileType = 0);
	static std::vector<std::string> readFile(const std::string& filepath);

	explicit operator std::string() const { return toString(); }
	double threadMeasure(const std::vector<std::string>& filepath);
};