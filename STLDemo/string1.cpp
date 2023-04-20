#include <iostream>>
#include <string>

using namespace std;

int main()
{
    string s1("This is a string");
    s1.at(20) = 'H'; // error: assignment of read-only location ‘s1.at(1)’

    string s2(5, 'a');
    string s3(s1, 5, 10);
    string s4(s1, 5);
    string s5("This is a string", 5);
    

    cout << "s1 = " << s1 << "  " << s1.at(1) << endl;
    cout << "s2 = " << s2 << endl;
    cout << "s3 = " << s3 << endl;
    cout << "s4 = " << s4 << endl;
    cout << "s5 = " << s5 << endl;

    if (s1 == s2)
        cout << "s1 and s2 are equal" << endl;
    else
        cout << "s1 and s2 are not equal" << endl;

    if (s1 == s3)
        cout << "s1 and s3 are equal" << endl;
    else
        cout << "s1 and s3 are not equal" << endl;

    return 0;
}