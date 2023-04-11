// Objects in a program should be replaceable with instances of their subtypes
// w/o altering the correctness of the program

#include <iostream>

class Rectangle
{
    protected:
        int width, height;
    public:
        Rectangle(const int width, const int height)
            : width(width), height(height)
        {}

        int getWidth() const
        {
            return width;
        }

        virtual void setWidth(const int width)
        {
            this->width = width;
        }

        int getHeight() const
        {
            return height;
        }

        virtual void setHeight(const int height)
        {
            this->height = height;
        }

        int area() const
        {
            return width * height;
        }

        virtual ~Rectangle() = default;
};

class Square : public Rectangle
{
    public:
        Square(int size): Rectangle(size, size)
        {}

        void setWidth(const int width) override
        {
            this->width = this->height = width;
        }

        void setHeight(const int height) override
        {
            this->height = width = height;
        }
};

struct RectangleFactory
{
    static Rectangle create_rectangle(int w, int h);
    static Rectangle create_square(int size);
};

void process(Rectangle& r)
{
    int w = r.getWidth();
    r.setHeight(10);

    std::cout << "expected ares = " << (w * 10)
              << ", got " << r.area() << std::endl;
}

int main()
{
    Rectangle r{5, 3};
    process(r);

    Square s{5};
    process(s);

    return 0;
}