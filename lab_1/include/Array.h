#ifndef MATRIX_STUFF_H
#define MATRIX_STUFF_H

class MyArray {

    private:

        int *data;
        int size;

        int allocate_memory();
        int free_memory();

    public:

        MyArray();
        MyArray(int size);
        ~MyArray();
        MyArray operator+(const MyArray &other);
        MyArray& operator=(const MyArray &other);
        int equalize(int* arr, int other_size);
        int append(int value);
        void clear();

        MyArray operator&(const MyArray &other) const;

        int get_size();
        void print();


};

void input_array(MyArray & array);

#endif
