#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include "Vector.h"
#include "Date.h"
#include "Time.h"
#include "WeatherRecord.h"
#include "StatsUtils.h"

using namespace std;

// Presentation Layer Stream Output Implementations (View decoupled from Model)
ostream & operator<<(ostream & os, const Date & d)
{
    os << d.GetDay() << "/" << d.GetMonth() << "/" << d.GetYear();
    return os;
}

ostream & operator<<(ostream & os, const Time & t)
{
    os << t.GetHour() << ":"
       << setw(2) << setfill('0') << t.GetMinute() << ":"
       << setw(2) << setfill('0') << t.GetSecond();
    return os;
}

// String Formatting Utilities
string Trim(const string & s)
{
    int start{0};
    int end{ static_cast<int>(s.size()) - 1 };
    while (start <= end && isspace(static_cast<unsigned char>(s[start])))
        ++start;
    while (end >= start && isspace(static_cast<unsigned char>(s[end])))
        --end;
    if (end < start)
        return "";
    return s.substr(start, end - start + 1);
}

void SplitByComma(const string & line, Vector<string> & out)
{
    stringstream ss(line);
    string field;
    while (getline(ss, field, ','))
    {
        out.Insert(out.Size(), Trim(field));
    }
}

string GetMonthName(int month)
{
    switch (month)
    {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    default:
        return "Unknown";
    }
}

// --- INPUT IMPLEMENTATION ROUTINES ---
bool GetCsvFileNames(const string & sourceConfig, Vector<string> & csvTargets)
{
    ifstream infile(sourceConfig);
    if (!infile)
    {
        cerr << "Error: Cannot open config file: " << sourceConfig << endl;
        return false;
    }
    string filename;
    while (getline(infile, filename))             // read every line, not just the first
    {
        string trimmed = Trim(filename);
        if (!trimmed.empty())                     // skip blank lines in the config
        {
            csvTargets.Insert(csvTargets.Size(), "data/" + trimmed);
        }
    }
    return csvTargets.Size() > 0;                  // true only if at least one file was listed
}

bool ParseHeaderIndices(const string & headerLine, int & wastIdx, int & sIdx, int & tIdx, int & srIdx)
{
    Vector<string> headers;
    SplitByComma(headerLine, headers);

    wastIdx = sIdx = tIdx = srIdx = -1;
    for (int i{0}; i < headers.Size(); ++i)
    {
        if (headers[i] == "WAST")
            wastIdx = i;
        else if (headers[i] == "S")
            sIdx = i;
        else if (headers[i] == "T")
            tIdx = i;
        else if (headers[i] == "SR")
            srIdx = i;
    }
    return (wastIdx != -1 && sIdx != -1 && tIdx != -1 && srIdx != -1);
}

void ParseDateTime(const string & ts, Date & d, Time & t)
{
    stringstream ss(ts);
    int day, month, year, hr, min;
    char delim;

    ss >> day >> delim >> month >> delim >> year >> hr >> delim >> min;
    d.SetDate(day, month, year);
    t.SetTime(hr, min, 0);
}

bool LoadWeatherData(const string & path, Vector<WeatherRecord> & records)
{
    ifstream infile(path);
    if (!infile)
    {
        cerr << "Error: Unable to locate file target: " << path << endl;
        return false;
    }

    string line;
    if (!getline(infile, line))
        return false;

    int wastIdx, sIdx, tIdx, srIdx;
    if (!ParseHeaderIndices(line, wastIdx, sIdx, tIdx, srIdx))
    {
        cerr << "Error: Column layout mapping target mismatch." << endl;
        return false;
    }

    while (getline(infile, line))
    {
        if (Trim(line).empty())
            continue;

        Vector<string> fields;
        SplitByComma(line, fields);

        // Fix: Banned max({...}) replaced entirely with manual sizing comparisons
        if (wastIdx >= fields.Size() || sIdx >= fields.Size() ||
                tIdx >= fields.Size() || srIdx >= fields.Size())
        {
            continue;
        }

        WeatherRecord rec;

        Date d;
        Time t;
        ParseDateTime(fields[wastIdx], d, t);
        rec.SetDate(d);
        rec.SetTime(t);

        if (!fields[sIdx].empty())
        {
            try
            {
                rec.SetWindSpeed(stod(fields[sIdx]));
            }
            catch (...) {}
        }
        if (!fields[tIdx].empty())
        {
            try
            {
                rec.SetAmbientTemp(stod(fields[tIdx]));
            }
            catch (...) {}
        }
        if (!fields[srIdx].empty())
        {
            try
            {
                rec.SetSolarRad(stod(fields[srIdx]));
            }
            catch (...) {}
        }

        if (rec.IsValidWind() || rec.IsValidTemp() || rec.IsValidSolar())
        {
            records.Insert(records.Size(), rec);
        }
    }
    return true;
}

// --- HIGH-LEVEL ALGORITHMIC PROCESSING ROUTINES ---

void ExecuteOption1(const Vector<WeatherRecord> & records, int month, int year)
{
    Vector<double> subset;
    for (int i{0}; i < records.Size(); ++i)
    {
        if (records[i].GetDate().GetMonth() == month && records[i].GetDate().GetYear() == year && records[i].IsValidWind())
        {
            subset.Insert(subset.Size(), records[i].GetWindSpeed() * 3.6); // m/s to km/h conversion
        }
    }

    // Exact Specification Format Match (Option 1)
    cout << GetMonthName(month) << " " << year << ":" << endl;
    if (subset.Size() == 0)
    {
        cout << "No Data" << endl;
        return;
    }
    cout << fixed << setprecision(1);
    cout << "Average speed: " << CalculateAverage(subset) << " km/h" << endl;
    cout << "Sample stdev: " << CalculateStandardDeviation(subset) << endl;
}

void ExecuteOption2(const Vector<WeatherRecord> & records, int year)
{
    // Exact Specification Format Match (Option 2): Loops months 1 to 12
    cout << year << endl;
    cout << fixed << setprecision(1);

    for (int m{1}; m <= 12; ++m)
    {
        Vector<double> subset;
        for (int i{0}; i < records.Size(); ++i)
        {
            if (records[i].GetDate().GetYear() == year && records[i].GetDate().GetMonth() == m && records[i].IsValidTemp())
            {
                subset.Insert(subset.Size(), records[i].GetAmbientTemp());
            }
        }

        cout << GetMonthName(m) << ": ";
        if (subset.Size() == 0)
        {
            cout << "No Data" << endl;
        }
        else
        {
            cout << "average: " << CalculateAverage(subset) << " degrees C, "
                 << "stdev: " << CalculateStandardDeviation(subset) << endl;
        }
    }
}

void ExecuteOption3(const Vector<WeatherRecord> & records, int month)
{
    // Assignment 2: sample Pearson Correlation Coefficient for one month,
    // across ALL years that have been loaded.
    // A record only contributes to a pair when BOTH fields of that pair are
    // valid, so every pair passed to CalculateSPCC is complete.
    Vector<double> sForT, tForS;   // S_T
    Vector<double> sForR, rForS;   // S_R
    Vector<double> tForR, rForT;   // T_R

    for (int i{0}; i < records.Size(); ++i)
    {
        if (records[i].GetDate().GetMonth() != month)
            continue;

        if (records[i].IsValidWind() && records[i].IsValidTemp())
        {
            sForT.Insert(sForT.Size(), records[i].GetWindSpeed() * 3.6);
            tForS.Insert(tForS.Size(), records[i].GetAmbientTemp());
        }
        if (records[i].IsValidWind() && records[i].IsValidSolar())
        {
            sForR.Insert(sForR.Size(), records[i].GetWindSpeed() * 3.6);
            rForS.Insert(rForS.Size(), records[i].GetSolarRad());
        }
        if (records[i].IsValidTemp() && records[i].IsValidSolar())
        {
            tForR.Insert(tForR.Size(), records[i].GetAmbientTemp());
            rForT.Insert(rForT.Size(), records[i].GetSolarRad());
        }
    }

    cout << "Sample Pearson Correlation Coefficient for "
         << GetMonthName(month) << endl;

    if (sForT.Size() == 0 && sForR.Size() == 0 && tForR.Size() == 0)
    {
        cout << "No Data" << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "S_T: " << CalculateSPCC(sForT, tForS) << endl;
    cout << "S_R: " << CalculateSPCC(sForR, rForS) << endl;
    cout << "T_R: " << CalculateSPCC(tForR, rForT) << endl;
}

void ExecuteOption4(const Vector<WeatherRecord> & records, int year)
{
    // Fix: Writes directly to an output stream file, not cout console
    const string OUTPUT_FILENAME{"WindTempSolar.csv"};
    ofstream outfile(OUTPUT_FILENAME);

    if (!outfile)
    {
        cerr << "Error: Unable to generate output report file context." << endl;
        return;
    }

    outfile << year << "\n";
    outfile << fixed << setprecision(1);
    bool anyMonthWritten{false};
    for (int m{1}; m <= 12; ++m)
    {
        Vector<double> speedSub;
        Vector<double> tempSub;
        double solarSum{0.0};
        bool operational{false};

        for (int i{0}; i < records.Size(); ++i)
        {
            if (records[i].GetDate().GetYear() == year && records[i].GetDate().GetMonth() == m)
            {
                if (records[i].IsValidWind())
                {
                    speedSub.Insert(speedSub.Size(), records[i].GetWindSpeed() * 3.6);
                    operational = true;
                }
                if (records[i].IsValidTemp())
                {
                    tempSub.Insert(tempSub.Size(), records[i].GetAmbientTemp());
                    operational = true;
                }
                if (records[i].IsValidSolar())
                {
                    if (records[i].GetSolarRad() >= 100.0)
                    {
                        solarSum += records[i].GetSolarRad();
                    }
                    operational = true;
                }
            }
        }

        if (!operational)
            continue;

        anyMonthWritten = true;
        double solarKwh = (solarSum * (1.0 / 6.0)) / 1000.0;

        // Write the month name, then each field. If a field has no data, leave
        // it blank between the commas - as shown in the spec example:
        //   February,4.5(3.1, 2.9), ,200.3
        // Assignment 2: mean absolute deviation is printed next to the stdev.
        outfile << GetMonthName(m) << ",";

        if (speedSub.Size() > 0)
            outfile << CalculateAverage(speedSub)
                    << "(" << CalculateStandardDeviation(speedSub)
                    << ", " << CalculateMAD(speedSub) << ")";
        outfile << ",";

        if (tempSub.Size() > 0)
            outfile << CalculateAverage(tempSub)
                    << "(" << CalculateStandardDeviation(tempSub)
                    << ", " << CalculateMAD(tempSub) << ")";
        outfile << ",";

        if (solarSum > 0.0)
            outfile << solarKwh;
        outfile << "\n";
    }

    // Spec: "If the entire year's data is not available, output just the year
    // on the first line and 'No Data' on the second line."
    if (!anyMonthWritten)
    {
        outfile << "No Data\n";
    }

    cout << "Output written to " << OUTPUT_FILENAME << endl;
}
// --- DISPLAY INTERFACE ROUTING MENU LOOP ---
void RunMenuLoop(const Vector<WeatherRecord> & records)
{
    int choice{0};
    while (choice != 5)
    {
        cout << "\n================ WEATHER MENU :) ================\n"
             << "1. Get Wind Speed Metrics (Month/Year Selection)\n"
             << "2. Get Ambient Temperature Profile Breakdown (Yearly Selection)\n"
             << "3. Sample Pearson Correlation Coefficient (Month Selection)\n"
             << "4. Output Weather Data History Matrix File (Yearly Selection)\n"
             << "5. Exit System Tracking Portal\n"
             << "Enter Choice: ";

        if (!(cin >> choice))
        {
            cin.clear();
            string discard;
            cin >> discard;
            continue;
        }

        int m{0}, y{0};
        if (choice == 1)
        {
            cout << "Enter Month (1-12) and Year (YYYY): ";
            cin >> m >> y;
        }
        else if (choice == 3)
        {
            cout << "Enter Month (1-12): ";
            cin >> m;
        }
        else if (choice == 2 || choice == 4)
        {
            cout << "Enter Target Processing Year (YYYY): ";
            cin >> y;
        }

        switch (choice)
        {
        case 1:
            ExecuteOption1(records, m, y);
            break;
        case 2:
            ExecuteOption2(records, y);
            break;
        case 3:
            ExecuteOption3(records, m);
            break;
        case 4:
            ExecuteOption4(records, y);
            break;
        case 5:
            cout << "Exiting database pipeline utility system." << endl;
            break;
        default:
            cout << "Invalid menu item selected." << endl;
        }
    }
}

int main()
{
    // --- 1. INPUT STAGE ---
    const string CONFIG_PATH{"data/data_source.txt"};
    Vector<string> csvPaths;

    if (!GetCsvFileNames(CONFIG_PATH, csvPaths))
    {
        return 1;
    }

    // Load every listed file into the SAME structure. LoadWeatherData appends,
    // so each file's records accumulate. Order in data_source.txt does not matter
    // because the menu options filter by year.
    Vector<WeatherRecord> masterStorageLog; //dynamic
    for (int i{0}; i < csvPaths.Size(); ++i)
    {
        if (!LoadWeatherData(csvPaths[i], masterStorageLog))
        {
            cerr << "Warning: skipping file that failed to load: " << csvPaths[i] << endl;
            // continue with the remaining files rather than aborting everything
        }
    }

    if (masterStorageLog.Size() == 0)
    {
        cerr << "Error: no data was loaded from any listed file." << endl;
        return 1;
    }

    // --- 2. PROCESSING & 3. OUTPUT STAGES (Managed inside coordination loop) ---
    RunMenuLoop(masterStorageLog);

    return 0;
}
