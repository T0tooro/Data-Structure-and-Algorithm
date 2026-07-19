#include <iostream>
#include "Date.h"
#include "Time.h"
#include "Vector.h"
using namespace std;

// Unit test driver for the Date, Time and Vector classes.
// Build this in a separate target which i didnt include Main.cpp.
// Each block prints the result for one test in the test plan (tests 1-7)... i hope

int main()
{
    cout << "===== Test 1: Date construction and getters =====\n";
    Date d(15, 6, 2014);
    cout << "GetDay=" << d.GetDay()
         << " GetMonth=" << d.GetMonth()
         << " GetYear=" << d.GetYear() << "\n\n";

    cout << "===== Test 2: Time construction and getters =====\n";
    Time t(9, 30, 0);
    cout << "GetHour=" << t.GetHour()
         << " GetMinute=" << t.GetMinute()
         << " GetSecond=" << t.GetSecond() << "\n\n";

    cout << "===== Test 3: Vector Insert - append order then indexed shift =====\n";
    Vector<char> v;
    v.Insert(v.Size(), 'A');
    v.Insert(v.Size(), 'B');
    v.Insert(v.Size(), 'C');
    cout << "Contents: ";
    for (int i = 0; i < v.Size(); ++i) cout << v[i] << " ";
    cout << " Size=" << v.Size() << "\n";
    v.Insert(1, 'X');
    cout << "After Insert(1,'X'): ";
    for (int i = 0; i < v.Size(); ++i) cout << v[i] << " ";
    cout << " Size=" << v.Size() << "\n\n";

    cout << "===== Test 4: Vector Insert invalid index returns false =====\n";
    int before = v.Size();
    bool ok = v.Insert(100, 'Z');
    cout << "Insert(100,'Z') returned " << (ok ? "true" : "false")
         << "; Size before=" << before << " after=" << v.Size() << "\n\n";

    cout << "===== Test 5: Vector deep copy on assignment =====\n";
    Vector<int> a;
    a.Insert(a.Size(), 10);
    a.Insert(a.Size(), 20);
    Vector<int> b;
    b = a;            // copy-assignment
    a[0] = 999;       // change the original
    cout << "a[0]=" << a[0] << " (changed)   b[0]=" << b[0]
         << " (should still be 10)\n\n";

    cout << "===== Test 6: Vector(int n) construction and negative guard =====\n";
    Vector<int> v10(10);
    v10.Insert(v10.Size(), 7);
    cout << "Vector<int> v(10): Size=" << v10.Size() << " v[0]=" << v10[0] << "\n";
    Vector<int> vneg(-1);   // guard should clamp capacity to 1, not crash
    vneg.Insert(vneg.Size(), 42);
    cout << "Vector<int> v(-1): no crash; Size=" << vneg.Size()
         << " v[0]=" << vneg[0] << "\n\n";

    cout << "===== Test 7: Vector destructor releases memory (no double-free) =====\n";
    Vector<int> outer;
    outer.Insert(outer.Size(), 100);
    outer.Insert(outer.Size(), 200);
    {
        Vector<int> inner;
        inner = outer;     // deep copy
        inner[0] = -1;     // change the copy
        // inner is destructed here at the end of the scope
    }
    cout << "After inner copy destructed, outer intact: outer[0]=" << outer[0]
         << " outer[1]=" << outer[1] << " Size=" << outer.Size() << "\n";
    cout << "(reaching here without a crash means the destructor freed cleanly)\n";

    return 0;
}
