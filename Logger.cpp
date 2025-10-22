#include "Logger.h"

const int FileSizeLimit = FILE_SIZE;

string currentDirectory = "";
string logDir = "";

void checkLogDirectory()
{
	#if isRelease
		currentDirectory = "/BIO/Program/" + sProgramName;
		logDir = currentDirectory + "/log";
	#else
		currentDirectory = filesystem::current_path().generic_string();
		logDir = currentDirectory + "/log";
	#endif
	char buf[DEFAULT_BUFLEN] = {0};

	if (!filesystem::exists(logDir))
	{
		filesystem::create_directory(logDir);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "sudo chmod 777 %s",logDir.c_str());
		system(buf);
	}
}

void writeToLog(LogLevel level, char* message)
{
	ofstream file;
	char buf[DEFAULT_BUFLEN] = {0};
	checkLogDirectory();

	string avaFileName = getAvailableFileName();
	string logFilePath = logDir + "/" + avaFileName + ".txt";
	string dateTime = currentDateTimeInString();
	
	memset(buf, 0, sizeof(buf));
	sprintf(buf, "%s %s", dateTime.c_str(), message);

	//ios::out is set to write mode
	//ios::app is append the data in end of line of the file
	file.open(logFilePath, ios::out | ios::app);

	if (file.is_open())
	{
		cout << buf << endl;
		//printf("%s \r\n", buf);
		file << buf << endl;
		fflush(stdout);
	}

	file.close();
}

string getAvailableFileName()
{
	long fileSizeB;
	double fileSizeKb;
	int fileNo = 0;
	bool isAvaFile = false;
	string fileNameRN;
	stringstream ss;
	string tempLogFilePath;
	string tempFileName;
	string defFileNameWithRN = "log_" + currentDateLogFileNameInString() + "_00";
	string defFileName = "log_" + currentDateLogFileNameInString();
	string defLogFilePath = logDir + "/" + defFileNameWithRN + ".txt";
	string avaFileName;

	if (filesystem::exists(defLogFilePath))
	{
		fileSizeB = static_cast<long>(filesystem::file_size(defLogFilePath));
		fileSizeKb = static_cast<double>(fileSizeB) / 1024.0;

		if (fileSizeKb > FileSizeLimit)
		{
			for (int i = 1; i < INT16_MAX; i++)
			{
				ss.str("");
				ss.clear();
				ss << setw(2) << setfill('0') << i;
				fileNameRN = ss.str();
				tempFileName = defFileName + "_" + fileNameRN;
				tempLogFilePath = logDir + "/" + tempFileName + ".txt";

				if (filesystem::exists(tempLogFilePath))
				{
					fileSizeB = static_cast<long>(filesystem::file_size(tempLogFilePath));
					fileSizeKb = static_cast<double>(fileSizeB) / 1024.0;

					if (fileSizeKb < FileSizeLimit)
					{
						avaFileName = tempFileName;
						isAvaFile = true;
						break;
					}
				}
				else
				{
					avaFileName = tempFileName;
					break;
				}
			}
		}
		else
		{
			avaFileName = defFileNameWithRN;
		}
	}
	else
	{
		avaFileName = defFileNameWithRN;
	}

	return avaFileName;
}