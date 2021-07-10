// JSON Engine.cpp : Defines the entry point for the application.
//

//When Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <Windows.h>
#include "rapidjson\document.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\writer.h"
#include "rapidjson\error\en.h"

//when Linux
#else
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "en.h"
#endif

#include "Cities.h"
#include <string>
#include <fstream>
#include <regex>
#include <sstream>
#include <iomanip>


using namespace std;
using namespace rapidjson;

//string* filters = NULL;
std::vector<string> filters;
Value filters_array;
int number_of_filters = 0;

//Neat Json Printer
class JSON final
{
public:
	static void PrintNode(const rapidjson::Value& node, size_t indent = 0, unsigned int level = 0, const std::string& nodeName = "")
	{
		std::cout << GetIndentString(indent, level);

		if (!nodeName.empty())
			std::cout << nodeName << ": ";

		if (node.IsBool())
			std::cout << node.GetBool();

		else if (node.IsInt())
			std::cout << node.GetInt();

		else if (node.IsUint())
			std::cout << node.GetUint();

		else if (node.IsInt64())
			std::cout << node.GetInt64();

		else if (node.IsUint64())
			std::cout << node.GetUint64();

		else if (node.IsDouble())
			std::cout << node.GetDouble();

		else if (node.IsString())
			std::cout << node.GetString();

		else if (node.IsArray())
		{
			if (!nodeName.empty()) std::cout << "\n" << GetIndentString(indent, level);
			PrintArray(node, indent, level);
		}

		else if (node.IsObject())
		{
			if (!nodeName.empty()) std::cout << "\n" << GetIndentString(indent, level);
			PrintObject(node, indent, level);
		}

		std::cout << "\n";
	}

	static void PrintObject(const rapidjson::Value& node, size_t indent = 0, unsigned int level = 0)
	{
		std::cout << "{\n";

		for (rapidjson::Value::ConstMemberIterator childNode = node.MemberBegin(); childNode != node.MemberEnd(); ++childNode)
		{
			PrintNode(childNode->value, indent, level + 1, childNode->name.GetString());
		}

		std::cout << GetIndentString(indent, level) << "}";
	}

	static void PrintArray(const rapidjson::Value& node, size_t indent = 0, unsigned int level = 0)
	{
		std::cout << "[\n";

		for (rapidjson::SizeType i = 0; i < node.Size(); ++i)
		{
			PrintNode(node[i], indent, level + 1);
		}

		std::cout << GetIndentString(indent, level) << "]";
	}

	static std::string GetIndentString(size_t indent = 0, unsigned int level = 0)
	{
		return std::move(std::string(level * indent, ' '));
	}
};


void enter(const Value& obj, size_t indent = 0) { //print JSON tree

	if (obj.IsObject()) { //check if object
		for (Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr) {   //iterate through object   
			const Value& objName = obj[itr->name.GetString()]; //make object value

			for (size_t i = 0; i != indent; ++i) //indent
				cout << " ";

			cout << itr->name.GetString() << ": "; //key name

			if (itr->value.IsNumber()) //if integer
				std::cout << itr->value.GetInt();

			else if (itr->value.IsString()) //if string
				std::cout << itr->value.GetString();


			else if (itr->value.IsBool()) //if bool
				std::cout << itr->value.GetBool();

			else if (itr->value.IsArray()) { //if array

				for (SizeType i = 0; i < itr->value.Size(); i++) {
					if (itr->value[i].IsNumber()) //if array value integer
						std::cout << itr->value[i].GetInt();

					else if (itr->value[i].IsString()) //if array value string
						std::cout << itr->value[i].GetString();

					else if (itr->value[i].IsBool()) //if array value bool
						std::cout << itr->value[i].GetBool();

					else if (itr->value[i].IsObject()) { //if array value object
						cout << "\n  ";
						const Value& m = itr->value[i];
						for (auto& v : m.GetObject()) { //iterate through array object
							if (m[v.name.GetString()].IsString()) //if array object value is string
								cout << v.name.GetString() << ": " << m[v.name.GetString()].GetString();
							else //if array object value is integer
								cout << v.name.GetString() << ": " << m[v.name.GetString()].GetInt();

							cout << "\t"; //indent
						}
					}
					cout << "\t"; //indent
				}
			}

			cout << endl;
			enter(objName, indent + 1); //if couldn't find in object, enter object and repeat process recursively 
		}
	}
}

bool isNumber(const string& str)
{
	return str.find_first_not_of("0123456789.") == string::npos;
}



template<typename T>
bool MatchFilter(string op, T origin, T filter, T filter2 = static_cast<T>(0))
{

	if (op == "gt" && origin > filter)
	{
		return true;
	}
	if (op == "lt" && origin < filter)
	{
		return true;
	}
	if (op == "le" && origin <= filter)
	{
		return true;
	}
	if (op == "ge" && origin >= filter)
	{
		return true;
	}
	if (op == "eq" && origin == filter)
	{
		return true;
	}

	if (op == "in" && origin >= filter && origin <= filter2)
	{
		return true;
	}


	if (op == "re")
	{
		std::ostringstream str;
		str << filter;
		std::string pattern = str.str();
		//cout << pattern;

		std::ostringstream str2;
		str2 << origin;
		std::string data = str2.str();
		//cout << data;

		if (regex_match(data, regex(pattern)))
		{
			//cout << "found! " << pattern << " in " << data << endl;
			return true;
		}
	}

	return false;
}

bool LoadJSON(char* path)
{
	string json = "";

	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			json += line;
		}
		myfile.close();
	}
	else
		return false;


	Document doc2;
	if (doc2.Parse(json.c_str()).HasParseError()) {
		fprintf(stderr, "\nError(offset %u): %s\n",
			(unsigned)doc2.GetErrorOffset(),
			GetParseError_En(doc2.GetParseError()));

		return false;
	}


	if (!doc2.HasParseError())
	{
		//JSON::PrintNode(doc2, 4, 0);
		//JSON::PrintArray(doc2, 0, 0);

		if (!doc2.HasMember("cities"))
		{
			cout << "Incorrect cities.json file structure." << endl;
			return 0;
		}


		auto& cities = doc2.FindMember("cities")->value;



		cout << "Total number of cities: " << cities.Size() << endl;

		//cin.get();
		//printf("Index | City Country Voivodeship Area Population Population_Density Parks\n");

		cout << left << setw(9) << "Index"
			<< setw(10) << "City" << "\t"
			<< setw(10) << "Country" << "\t"
			<< setw(20) << "Voivodeship" << "\t"
			<< setw(8) << "Area" << "\t"
			<< setw(8) << "Population" << "\t"
			<< setw(8) << "Popul_Density" << "\t"
			<< setw(8) << "Parks" << "\n";


		for (SizeType i = 0; i < cities.Size(); i++)
		{

			//Dodac Obserwacje do maila typu czemu population jest float w jednym miejscu wtf?
			auto city = cities[i].FindMember("city")->value.GetString();
			auto country = cities[i].FindMember("country")->value.GetString();
			auto voivodeship = cities[i].FindMember("voivodeship")->value.GetString();
			auto area = cities[i].FindMember("area")->value.GetFloat();
			auto population = cities[i].FindMember("population")->value.GetFloat();
			auto population_density = cities[i].FindMember("population_density")->value.GetFloat();
			float parks = 0;

			if (cities[i].FindMember("recreations")->value.IsArray())
			{

				auto& recreations = cities[i].FindMember("recreations")->value;
				parks = recreations[0].FindMember("parks")->value.GetFloat(); //float intended

				//cout << i << " " << parks << endl;
			}
			for (int k = 0; k < number_of_filters; k++)
			{

				int index1 = filters[k].find(':') + 1;
				int index2 = filters[k].find(':', index1 + 1);
				//cout << index1 << " " << index2 << endl;
				string op = filters[k].substr(index1, index2 - index1);
				//cout << op << endl;



				index1 = filters[k].find(':', index2) + 1;
				string value = filters[k].substr(index1, filters[k].length() - index1);



				float value_float = -1;
				int* range = new int[2]{ 0, 0 };

				if (isNumber(value))
				{
					value_float = atof(value.c_str());
				}
				else if (value.find('-') != string::npos)
				{
					int index1 = value.find('-');
					range[0] = atoi(value.substr(0, index1).c_str());
					range[1] = atoi(value.substr(index1 + 1, value.length() - index1 + 1).c_str());

					//cout << range[0] << range[1] << endl;

				}
				else
				{
					//cout << value << endl;
				}


				if (filters[k].find("area") != string::npos)
				{
					if (range[1] == 0 && !MatchFilter(op, area, value_float))
					{
						//cout << i << " " << k << "  " << city << " " << area << " " << (float)range[0] << " " << (float)range[1] << endl;
						//cout << i << " " << k << "  " << city << " " << area << " " << value_float << endl;

						break;
					}

					if (range[1] != 0 && !MatchFilter(op, area, (float)range[0], (float)range[1]))
					{
						break;
					}
				}

				if (filters[k].find("country") != string::npos)
				{
					if (!MatchFilter(op, country, value.c_str()))
					{
						cout << i << " " << k << "  " << op << " " << country << " " << value.c_str() << endl;
						break;
					}
				}


				if (filters[k].find("city") != string::npos)
				{
					if (!MatchFilter(op, city, value.c_str()))
					{
						break;
					}
				}

				if (filters[k].find("voivodeship") != string::npos)
				{

					if (!MatchFilter(op, voivodeship, value.c_str()))
					{
						break;
					}
				}

				if (filters[k].find("population") != string::npos && filters[k].find("population_density") == string::npos)
				{

					//cout << i << " " << k << "  " << city << " " << population << " " << (float)range[0] << " " << (float)range[1] << endl;
					//cout << i << " " << k << "  " << city << " " << population << " " << value_float << endl;

					if (range[1] == 0 && !MatchFilter(op, population, value_float))
					{
						break;
					}

					if (range[1] != 0 && !MatchFilter(op, population, (float)range[0], (float)range[1]))
					{
						break;
					}


				}



				if (filters[k].find("population_density") != string::npos || filters[k].find("popul_density") != string::npos)
				{

					if (range[1] == 0 && !MatchFilter(op, population_density, value_float))
					{

						break;
					}

					if (range[1] != 0 && !MatchFilter(op, population_density, (float)range[0], (float)range[1]))
					{

						break;
					}

				}

				if (filters[k].find("recreations/parks") != string::npos)
				{
					if (range[1] == 0 && !MatchFilter(op, parks, value_float))
					{
						break;
					}
					if (range[1] != 0 && !MatchFilter(op, parks, (float)range[0], (float)range[1]))
					{
						break;
					}

				}

				//if (k == number_of_filters-1)
				//cout << number_of_filters << endl;

				//When there is last iteration - city passed all filters
				if (k == number_of_filters - 1)
				{
					cout << fixed << setprecision(2);
					cout << left << setw(9) << i
						<< setw(10) << city << "\t"
						<< setw(14) << country << "\t"
						<< setw(20) << voivodeship << "\t"
						<< setw(8) << area << "\t"
						<< setw(8) << population << "\t"
						<< setw(8) << population_density << "\t"
						<< setw(8) << parks << "\n";



					//cout << setw(10) << left << i << setw(10) << left << city << setw(10) << left << country << endl;
					//printf("%-4d %-12s %-12s\n", i, city, country);
					//printf("%i | %s %s %s %.2f %.2f %.2f %i \n", i, city, country, voivodeship, area, population, population_density, parks);

				}


				delete[] range;

			} //for filters


			//When there is no filters, print all
			if (number_of_filters == 0)
			{
				//to avoid having scientific look of numbers
				cout << fixed << setprecision(2);
				cout << left << setw(9) << i
					<< setw(10) << city << "\t"
					<< setw(14) << country << "\t"
					<< setw(20) << voivodeship << "\t"
					<< setw(8) << area << "\t"
					<< setw(8) << (double)population << "\t"
					<< setw(8) << population_density << "\t"
					<< setw(8) << parks << "\n";

			}


			//printf("%i | %s %s %s %.2f %.2f %i \n", i, city, country, voivodeship, area, population, population_density);
			//cout << city <<  "  << endl;

		}



	}




	return true;
}

int GetFilters(char* path)
{
	string filter;
	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			filter += line;
		}
		myfile.close();
	}

	Document doc_filter;
	if (doc_filter.Parse(filter.c_str()).HasParseError()) {
		fprintf(stderr, "\nError(offset %u): %s\n",
			(unsigned)doc_filter.GetErrorOffset(),
			GetParseError_En(doc_filter.GetParseError()));
		// ...
	}



	if (!doc_filter.HasParseError())
	{
		//JSON::PrintNode(doc_filter, 4, 0);
		//JSON::PrintArray(doc2, 0, 0);

		if (!doc_filter.HasMember("filters"))
		{
			cout << "Incorrect filters.json file structure." << endl;
			return 0;
		}

		//auto& filters_array = doc_filter.FindMember("filters")->value;
		filters_array = doc_filter.FindMember("filters")->value;

		//Value GenericMember<UTF8<>, MemoryPoolAllocator<>>::value
		//cout << filters_array[0].FindMember("path")->value.GetString() << endl;

		//cout << filters[0].FindMember("path")->value.GetString() << endl;
		//cout << filters[0].FindMember("path")->value.GetString() << endl;
		//filters = new string[test.Size()];
		//JSON::PrintNode(filters_array, 4, 0);



		number_of_filters = filters_array.Size();

		//cout << number_of_filters << endl;
		//cout << filters_array[0].FindMember("path")->value.GetString() << endl;



		//filters = new string[number_of_filters];

		//cout << number_of_filters << endl;


		printf("Active filters are (%i):\n", number_of_filters);

		for (SizeType i = 0; i < number_of_filters; i++)
		{
			string line = "";
			auto& path = filters_array[i].FindMember("path")->value;
			//cout << path.GetString() << endl;

			line += path.GetString();
			line += ":";

			//filters.push_back(path.GetString());
		/*	filters[i] += path.GetString();
			filters[i] += ":";*/

			auto& op = filters_array[i].FindMember("op")->value;
			//cout << op.GetString() << endl;


			line += op.GetString();
			line += ":";
			//filters.push_back(op.GetString());
			//filters[i] += op.GetString();
			//filters[i] += ":";

			auto& value = filters_array[i].FindMember("value")->value;
			if (value.IsNumber()) {

				//cout << value.GetInt() << endl;

				line += to_string(value.GetInt());
				//filters.push_back(to_string(value.GetInt()));
				//filters[i] += to_string(value.GetInt());

			}
			else if (value.IsString())
			{
				//cout << value.GetString() << endl;


				line += value.GetString();
				//filters.push_back(value.GetString());
				//filters[i] += value.GetString();
			}



			else if (value.IsArray())
			{
				//cout << value.GetArray()[0].GetInt() << " : " << value.GetArray()[1].GetInt() << endl;


				line += to_string(value.GetArray()[0].GetInt());
				line += "-";
				line += to_string(value.GetArray()[1].GetInt());

				//filters.push_back(to_string(value.GetArray()[0].GetInt()) + "-" + to_string(value.GetArray()[1].GetInt()));
				//filters[i] += to_string(value.GetArray()[0].GetInt());
				//filters[i] += "-";
				//filters[i] += to_string(value.GetArray()[1].GetInt());
			}
			else
			{
				//cout << "null" << endl;
			}


			filters.push_back(line);
			cout << line << endl;

		}


		return number_of_filters;


	}
}

bool Reset()
{
	filters.clear();

	//delete[] filters;
	number_of_filters = 0;

	ActiveFilters();

	return true;
}

bool AddFilter(char* value)
{

	filters.push_back(value);



	number_of_filters++;

	ActiveFilters();

	return true;
}

int ActiveFilters()
{

	printf("Active filters are (%i):\n", number_of_filters);


	for (SizeType i = 0; i < number_of_filters; i++)
	{
		cout << filters[i] << endl;
	}
	return number_of_filters;
}

int Start()
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SetConsoleOutputCP(CP_UTF8);
#endif

	int number_of_filters = 0;

	cout << "Cities JSON Filter\nType: \"load C:\\path\\to\\filters.json\" to load json file with demanded filters ex: \"load filters.json\"" << endl;

	cout << "Type: \"f:area:gt:100.5\" to add filter manually.Available filters are: (gt, ge, lt, le, eq, in) ex: \"f:area:in:200-300\" \n";
	cout << "Type: \"reset\" to clear all filters.\n";
	cout << "Type: \"filters\" to get the list of active filters.\n";

	cout << "Type: \"print C:\\path\\to\\cities.json\" to get the results ex: \"print cities.json\".\n\n";


	try
	{

		string command;

		do
		{
			cout << "\n$:";
			std::getline(std::cin, command);

			if (command.length() == 0) continue;

			else if (command.find("load") != string::npos)
			{
				string path = command.substr(5, strlen(command.c_str()) - 5);
				//cout << path << endl;
				GetFilters((char*)path.c_str());
			}

			else if (command.substr(0, 2) == "f:")
			{
				AddFilter((char*)command.substr(2, command.length() - 2).c_str());

			}

			else if (command.find("print") != string::npos)
			{
				string path = command.substr(6, strlen(command.c_str()) - 6);
				//cout << path << endl;
				LoadJSON((char*)path.c_str());
			}
			else if (command == "reset")
			{
				Reset();
			}
			else if (command == "filters")
			{
				ActiveFilters();
			}

			else
				cout << "Sorry I did not understand." << endl;


		} while (1);

	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}



	//JSON::PrintNode(filters_array, 4, 0);


	return 0;
}
