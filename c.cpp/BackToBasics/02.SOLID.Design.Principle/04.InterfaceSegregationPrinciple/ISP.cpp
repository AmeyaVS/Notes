#include <iostream>
#include <string>

struct Document
{
    std::string name;
    Document(const std::string& name): name{name}
    {}
};

//struct IMachine
//{
//    virtual ~IMachine() = default;
//    virtual void print(Document& doc) = 0;
//    virtual void scan(Document& doc) = 0;
//    virtual void fax(Document& doc) = 0;
//};
//
//struct MFP : public IMachine
//{
//    void print(Document& doc) override;
//    void scan(Document& doc) override;
//    void fax(Document& doc) override;
//};

// 1. Recompile
// 2. Client does not need this
// 3. Forcing implementors to implement too much

struct IPrinter
{
    virtual ~IPrinter() = default;
    virtual void print(Document& doc) = 0;
};

struct IScanner
{
    virtual ~IScanner() = default;
    virtual void scan(Document& doc) = 0;
};

struct Printer : public IPrinter
{
    void print(Document& doc) override
    {
        std::cout << "Printing document: " << doc.name << std::endl;
    }
};

struct Scanner : public IScanner
{
    void scan(Document& doc) override
    {
        std::cout << "Scanning document: " << doc.name << std::endl;
    }
};

struct IMachine : public IPrinter, IScanner
{};

struct Machine : public IMachine
{
    IPrinter& printer;
    IScanner& scanner;

    Machine(IPrinter& printer, IScanner& scanner)
        : printer{printer},
          scanner{scanner}
    {}

    void print(Document& doc) override
    {
        printer.print(doc);
    }

    void scan(Document& doc) override
    {
        scanner.scan(doc);
    }
};

// IPrinter --> Printer
// everything --> Machine

int main()
{
    Document d{"My Document"};
    Printer p;
    Scanner s;

    p.print(d);
    s.scan(d);

    Machine m(p, s);

    m.print(d);
    m.scan(d);

    p.print(d);
    s.scan(d);

    return 0;
}