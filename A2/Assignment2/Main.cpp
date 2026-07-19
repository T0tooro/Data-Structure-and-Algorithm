#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <map>
#include "Vector.h"
#include "Date.h"
#include "Time.h"
#include "WeatherRecord.h"
#include "BinarySearchTree.h"
#include "StatsUtils.h"

using namespace std;

/// One year's readings, ordered by date and then time
typedef BinarySearchTree<WeatherRecord> WeatherTree;

/// Every loaded year, keyed by the year number
typedef map<int, WeatherTree> YearMap;

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
                double s{stod(fields[sIdx])};
                if (s >= 0.0)          // a negative wind speed is not a reading
                    rec.SetWindSpeed(s);
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
                double sr{stod(fields[srIdx])};
                if (sr >= 0.0)         // solar radiation cannot be negative
                    rec.SetSolarRad(sr);
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

// --- TREE CONSTRUCTION ---
// The data arrives in chronological order. Inserting it in that order would
// build a tree that is one long right-hand branch, which is as slow as a list
// and deep enough to exhaust the call stack during recursion. The tree itself
// is not changed to fix this; the client controls the order of insertion
// instead, which is where the knowledge about the data belongs.

void MergeRuns(const Vector<WeatherRecord> & left,
               const Vector<WeatherRecord> & right,
               Vector<WeatherRecord> & out)
{
    int i{0};
    int j{0};
    while (i < left.Size() && j < right.Size())
    {
        if (right[j] < left[i])
        {
            out.Insert(out.Size(), right[j]);
            ++j;
        }
        else
        {
            out.Insert(out.Size(), left[i]);
            ++i;
        }
    }
    while (i < left.Size())
    {
        out.Insert(out.Size(), left[i]);
        ++i;
    }
    while (j < right.Size())
    {
        out.Insert(out.Size(), right[j]);
        ++j;
    }
}

/**
 * @brief Sorts records into date and time order using merge sort.
 *
 * Merge sort is used rather than a simple insertion sort because two data
 * files can cover the same year, so the combined records are not necessarily
 * already ordered, and the record count is large.
 *
 * @param  data - the records to sort
 * @param  out - receives the records in ascending order
 * @return void
 */
void MergeSortRecords(const Vector<WeatherRecord> & data, Vector<WeatherRecord> & out)
{
    if (data.Size() <= 1)
    {
        for (int i{0}; i < data.Size(); ++i)
            out.Insert(out.Size(), data[i]);
        return;
    }

    int mid{data.Size() / 2};
    Vector<WeatherRecord> leftIn;
    Vector<WeatherRecord> rightIn;

    for (int i{0}; i < mid; ++i)
        leftIn.Insert(leftIn.Size(), data[i]);
    for (int i{mid}; i < data.Size(); ++i)
        rightIn.Insert(rightIn.Size(), data[i]);

    Vector<WeatherRecord> leftSorted;
    Vector<WeatherRecord> rightSorted;
    MergeSortRecords(leftIn, leftSorted);
    MergeSortRecords(rightIn, rightSorted);

    MergeRuns(leftSorted, rightSorted, out);
}

/**
 * @brief Inserts a sorted range into the tree middle element first.
 *
 * Taking the middle element of the range as the subtree root, then repeating
 * on each half, produces a balanced tree from already sorted data. The tree
 * class is untouched.
 *
 * @param  sorted - records already in ascending order
 * @param  lo - first index of the range
 * @param  hi - last index of the range
 * @param  tree - the tree to insert into
 * @return void
 */
void InsertBalanced(const Vector<WeatherRecord> & sorted, int lo, int hi, WeatherTree & tree)
{
    if (lo > hi)
        return;
    int mid{lo + ((hi - lo) / 2)};
    tree.Insert(sorted[mid]);
    InsertBalanced(sorted, lo, mid - 1, tree);
    InsertBalanced(sorted, mid + 1, hi, tree);
}

/**
 * @brief Groups records by year and builds one balanced tree per year.
 *
 * @param  records - every record read from the data files
 * @param  years - receives one tree for each year present
 * @return void
 */
void BuildYearMap(const Vector<WeatherRecord> & records, YearMap & years)
{
    map<int, Vector<WeatherRecord> > buckets;

    for (int i{0}; i < records.Size(); ++i)
    {
        int y{records[i].GetDate().GetYear()};
        buckets[y].Insert(buckets[y].Size(), records[i]);
    }

    map<int, Vector<WeatherRecord> >::const_iterator it;
    for (it = buckets.begin(); it != buckets.end(); ++it)
    {
        Vector<WeatherRecord> sorted;
        MergeSortRecords(it->second, sorted);
        InsertBalanced(sorted, 0, sorted.Size() - 1, years[it->first]);
    }
}

// --- CLIENT SIDE CALL-BACK COLLECTORS ---
// A function pointer carries no state, so each collector keeps its own static
// state and is configured before the traversal runs. The tree stays unaware of
// what a weather record is or what is being done with it.

/**
 * @class WindMonthCollector
 * @brief Collects wind speeds for one month, in km/h.
 */
class WindMonthCollector
{
public:
    /// Clears the collector and sets the month to collect
    static void Reset(int month);
    /// Call-back matching WeatherTree::VisitFn
    static void Collect(const WeatherRecord & r);
    /// The speeds collected by the last traversal
    static const Vector<double> & Values();
private:
    static int s_month;
    static Vector<double> s_values;
};

int WindMonthCollector::s_month = 0;
Vector<double> WindMonthCollector::s_values;

void WindMonthCollector::Reset(int month)
{
    s_month = month;
    Vector<double> empty;
    s_values = empty;
}

void WindMonthCollector::Collect(const WeatherRecord & r)
{
    if (r.GetDate().GetMonth() == s_month && r.IsValidWind())
    {
        s_values.Insert(s_values.Size(), r.GetWindSpeed() * 3.6);
    }
}

const Vector<double> & WindMonthCollector::Values()
{
    return s_values;
}

/**
 * @class YearTempCollector
 * @brief Collects ambient temperatures into one bucket per month.
 */
class YearTempCollector
{
public:
    /// Clears every month bucket
    static void Reset();
    /// Call-back matching WeatherTree::VisitFn
    static void Collect(const WeatherRecord & r);
    /// The temperatures collected for the given month, 1 to 12
    static const Vector<double> & Month(int m);
private:
    static Vector<double> s_months[13];
};

Vector<double> YearTempCollector::s_months[13];

void YearTempCollector::Reset()
{
    for (int m{0}; m < 13; ++m)
    {
        Vector<double> empty;
        s_months[m] = empty;
    }
}

void YearTempCollector::Collect(const WeatherRecord & r)
{
    if (r.IsValidTemp())
    {
        int m{r.GetDate().GetMonth()};
        if (m >= 1 && m <= 12)
            s_months[m].Insert(s_months[m].Size(), r.GetAmbientTemp());
    }
}

const Vector<double> & YearTempCollector::Month(int m)
{
    return s_months[m];
}

/**
 * @class PairCollector
 * @brief Collects the three pairs of fields needed by the correlation option.
 *
 * A record only contributes to a pair when both fields of that pair are valid,
 * so every pair passed to the calculation is complete.
 */
class PairCollector
{
public:
    /// Clears the collector and sets the month to collect
    static void Reset(int month);
    /// Call-back matching WeatherTree::VisitFn
    static void Collect(const WeatherRecord & r);

    static const Vector<double> & WindForTemp();
    static const Vector<double> & TempForWind();
    static const Vector<double> & WindForSolar();
    static const Vector<double> & SolarForWind();
    static const Vector<double> & TempForSolar();
    static const Vector<double> & SolarForTemp();
private:
    static int s_month;
    static Vector<double> s_sForT;
    static Vector<double> s_tForS;
    static Vector<double> s_sForR;
    static Vector<double> s_rForS;
    static Vector<double> s_tForR;
    static Vector<double> s_rForT;
};

int PairCollector::s_month = 0;
Vector<double> PairCollector::s_sForT;
Vector<double> PairCollector::s_tForS;
Vector<double> PairCollector::s_sForR;
Vector<double> PairCollector::s_rForS;
Vector<double> PairCollector::s_tForR;
Vector<double> PairCollector::s_rForT;

void PairCollector::Reset(int month)
{
    s_month = month;
    Vector<double> empty;
    s_sForT = empty;
    s_tForS = empty;
    s_sForR = empty;
    s_rForS = empty;
    s_tForR = empty;
    s_rForT = empty;
}

void PairCollector::Collect(const WeatherRecord & r)
{
    if (r.GetDate().GetMonth() != s_month)
        return;

    if (r.IsValidWind() && r.IsValidTemp())
    {
        s_sForT.Insert(s_sForT.Size(), r.GetWindSpeed() * 3.6);
        s_tForS.Insert(s_tForS.Size(), r.GetAmbientTemp());
    }
    if (r.IsValidWind() && r.IsValidSolar())
    {
        s_sForR.Insert(s_sForR.Size(), r.GetWindSpeed() * 3.6);
        s_rForS.Insert(s_rForS.Size(), r.GetSolarRad());
    }
    if (r.IsValidTemp() && r.IsValidSolar())
    {
        s_tForR.Insert(s_tForR.Size(), r.GetAmbientTemp());
        s_rForT.Insert(s_rForT.Size(), r.GetSolarRad());
    }
}

const Vector<double> & PairCollector::WindForTemp()
{
    return s_sForT;
}
const Vector<double> & PairCollector::TempForWind()
{
    return s_tForS;
}
const Vector<double> & PairCollector::WindForSolar()
{
    return s_sForR;
}
const Vector<double> & PairCollector::SolarForWind()
{
    return s_rForS;
}
const Vector<double> & PairCollector::TempForSolar()
{
    return s_tForR;
}
const Vector<double> & PairCollector::SolarForTemp()
{
    return s_rForT;
}

/**
 * @class YearReportCollector
 * @brief Collects wind, temperature and solar totals into one bucket per month.
 */
class YearReportCollector
{
public:
    /// Clears every month bucket
    static void Reset();
    /// Call-back matching WeatherTree::VisitFn
    static void Collect(const WeatherRecord & r);

    static const Vector<double> & Wind(int m);
    static const Vector<double> & Temp(int m);
    static double SolarTotal(int m);
    static bool HasData(int m);
private:
    static Vector<double> s_wind[13];
    static Vector<double> s_temp[13];
    static double s_solar[13];
    static bool s_any[13];
};

Vector<double> YearReportCollector::s_wind[13];
Vector<double> YearReportCollector::s_temp[13];
double YearReportCollector::s_solar[13] = {0.0};
bool YearReportCollector::s_any[13] = {false};

void YearReportCollector::Reset()
{
    for (int m{0}; m < 13; ++m)
    {
        Vector<double> emptyW;
        Vector<double> emptyT;
        s_wind[m] = emptyW;
        s_temp[m] = emptyT;
        s_solar[m] = 0.0;
        s_any[m] = false;
    }
}

void YearReportCollector::Collect(const WeatherRecord & r)
{
    int m{r.GetDate().GetMonth()};
    if (m < 1 || m > 12)
        return;

    if (r.IsValidWind())
    {
        s_wind[m].Insert(s_wind[m].Size(), r.GetWindSpeed() * 3.6);
        s_any[m] = true;
    }
    if (r.IsValidTemp())
    {
        s_temp[m].Insert(s_temp[m].Size(), r.GetAmbientTemp());
        s_any[m] = true;
    }
    if (r.IsValidSolar())
    {
        if (r.GetSolarRad() >= 100.0)
            s_solar[m] += r.GetSolarRad();
        s_any[m] = true;
    }
}

const Vector<double> & YearReportCollector::Wind(int m)
{
    return s_wind[m];
}
const Vector<double> & YearReportCollector::Temp(int m)
{
    return s_temp[m];
}
double YearReportCollector::SolarTotal(int m)
{
    return s_solar[m];
}
bool YearReportCollector::HasData(int m)
{
    return s_any[m];
}

// --- HIGH-LEVEL ALGORITHMIC PROCESSING ROUTINES ---

void ExecuteOption1(const YearMap & years, int month, int year)
{
    cout << GetMonthName(month) << " " << year << ":" << endl;

    YearMap::const_iterator it{years.find(year)};
    if (it == years.end())
    {
        cout << "No Data" << endl;
        return;
    }

    WindMonthCollector::Reset(month);
    it->second.InOrderTraversal(WindMonthCollector::Collect);

    const Vector<double> & subset{WindMonthCollector::Values()};
    if (subset.Size() == 0)
    {
        cout << "No Data" << endl;
        return;
    }

    cout << fixed << setprecision(1);
    cout << "Average speed: " << CalculateAverage(subset) << " km/h" << endl;
    cout << "Sample stdev: " << CalculateStandardDeviation(subset) << endl;
}

void ExecuteOption2(const YearMap & years, int year)
{
    cout << year << endl;

    YearMap::const_iterator it{years.find(year)};
    if (it == years.end())
    {
        for (int m{1}; m <= 12; ++m)
            cout << GetMonthName(m) << ": No Data" << endl;
        return;
    }

    YearTempCollector::Reset();
    it->second.InOrderTraversal(YearTempCollector::Collect);

    cout << fixed << setprecision(1);
    for (int m{1}; m <= 12; ++m)
    {
        const Vector<double> & subset{YearTempCollector::Month(m)};
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

void ExecuteOption3(const YearMap & years, int month)
{
    // The selected month applies to every year that has been loaded, so each
    // year's tree is traversed in turn and the same collector accumulates.
    PairCollector::Reset(month);

    YearMap::const_iterator it;
    for (it = years.begin(); it != years.end(); ++it)
    {
        it->second.InOrderTraversal(PairCollector::Collect);
    }

    cout << "Sample Pearson Correlation Coefficient for "
         << GetMonthName(month) << endl;

    if (PairCollector::WindForTemp().Size() == 0 &&
            PairCollector::WindForSolar().Size() == 0 &&
            PairCollector::TempForSolar().Size() == 0)
    {
        cout << "No Data" << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "S_T: " << CalculateSPCC(PairCollector::WindForTemp(),
                                     PairCollector::TempForWind()) << endl;
    cout << "S_R: " << CalculateSPCC(PairCollector::WindForSolar(),
                                     PairCollector::SolarForWind()) << endl;
    cout << "T_R: " << CalculateSPCC(PairCollector::TempForSolar(),
                                     PairCollector::SolarForTemp()) << endl;
}

void ExecuteOption4(const YearMap & years, int year)
{
    const string OUTPUT_FILENAME{"WindTempSolar.csv"};
    ofstream outfile(OUTPUT_FILENAME);

    if (!outfile)
    {
        cerr << "Error: Unable to generate output report file context." << endl;
        return;
    }

    outfile << year << "\n";

    YearMap::const_iterator it{years.find(year)};
    if (it == years.end())
    {
        outfile << "No Data\n";
        cout << "Output written to " << OUTPUT_FILENAME << endl;
        return;
    }

    YearReportCollector::Reset();
    it->second.InOrderTraversal(YearReportCollector::Collect);

    outfile << fixed << setprecision(1);
    bool anyMonthWritten{false};

    for (int m{1}; m <= 12; ++m)
    {
        if (!YearReportCollector::HasData(m))
            continue;

        anyMonthWritten = true;

        const Vector<double> & speedSub{YearReportCollector::Wind(m)};
        const Vector<double> & tempSub{YearReportCollector::Temp(m)};
        double solarSum{YearReportCollector::SolarTotal(m)};
        double solarKwh{(solarSum * (1.0 / 6.0)) / 1000.0};

        // A field with no data is left blank between its commas, as the
        // specification shows: February,4.5(3.1, 2.9), ,200.3
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

    if (!anyMonthWritten)
    {
        outfile << "No Data\n";
    }

    cout << "Output written to " << OUTPUT_FILENAME << endl;
}

// --- DISPLAY INTERFACE ROUTING MENU LOOP ---
void RunMenuLoop(const YearMap & years)
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
            ExecuteOption1(years, m, y);
            break;
        case 2:
            ExecuteOption2(years, y);
            break;
        case 3:
            ExecuteOption3(years, m);
            break;
        case 4:
            ExecuteOption4(years, y);
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
    // Read every file listed in the config into a temporary linear container.
    const string CONFIG_PATH{"data/data_source.txt"};
    Vector<string> csvPaths;

    if (!GetCsvFileNames(CONFIG_PATH, csvPaths))
    {
        return 1;
    }

    Vector<WeatherRecord> loaded;
    for (int i{0}; i < csvPaths.Size(); ++i)
    {
        if (!LoadWeatherData(csvPaths[i], loaded))
        {
            cerr << "Warning: skipping file that failed to load: " << csvPaths[i] << endl;
        }
    }

    if (loaded.Size() == 0)
    {
        cerr << "Error: no data was loaded from any listed file." << endl;
        return 1;
    }

    // --- 2. STRUCTURING STAGE ---
    // Group the records by year and build one balanced search tree per year.
    // The map gives the year in O(log n); the tree holds that year's records in
    // date and time order and is read back through function pointer traversals.
    YearMap years;
    BuildYearMap(loaded, years);

    cout << "Loaded " << loaded.Size() << " records across "
         << static_cast<int>(years.size()) << " year(s)." << endl;

    // --- 3. PROCESSING & 4. OUTPUT STAGES ---
    RunMenuLoop(years);

    return 0;
}
