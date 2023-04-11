#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;

class Journal
{
    string title;
    vector<string> entries;

    public:

    explicit Journal(const string& title)
    : title{title}
    {}

    void add(const string& entry);

    // persistance is a separate concern
    void save(const string& filename);

    // Persistence Can be handled in separate class
    friend class PersistenceManager;
};

void Journal::add(const string& entry)
{
    static int count = 0;
    entries.push_back(boost::lexical_cast<string>(++count) + ": " + entry);
}

void Journal::save(const string& filename)
{
    ofstream ofs(filename);
    for(auto& s: entries)
    {
        ofs << s << endl;
    }
}

class PersistenceManager
{
    public:

    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for(auto& s: j.entries)
        {
            ofs << s << endl;
        }
    }
};

int main()
{
    Journal journal{"Dear Diary"};
    journal.add("Hello this is the first entry!");
    journal.add("This is the second entry");
    journal.add("This is the third entry");

    //journal.save("diary.txt");

    PersistenceManager pm;

    pm.save(journal, "diary.txt");

    return 0;
}