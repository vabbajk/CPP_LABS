#ifndef MATRIX_STUFF_H
#define MATRIX_STUFF_H

class MyArray {

    private:

        int size;
        int *data{};


        int allocate_memory();
        int free_memory();

    public:

        MyArray();
        explicit MyArray(int size);
        MyArray(const MyArray& other);
        ~MyArray();
        friend MyArray operator+(const MyArray& a, const MyArray& b);
        friend MyArray operator&(const MyArray& a, const MyArray& b);
        MyArray& operator=(const MyArray &other);
        int equalize(const int* arr, int other_size);
        int append(int value);
        void clear();



        const int get_size();
        const void  print();


};

void input_array(MyArray & array);

#endif
