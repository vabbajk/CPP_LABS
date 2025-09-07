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

        friend MyArray operator+(const MyArray& left, const MyArray& right) {
            MyArray result(left.size + right.size);

            if (result.data == nullptr || left.data == nullptr || right.data == nullptr) {
                exit(1);
            }

            for (int i = 0; i < left.size; i++) {
                result.data[i] = left.data[i];
            }

            for (int i = 0; i < right.size; i++) {
                result.data[left.size + i] = right.data[i];
            }

            return result;
        }

    friend MyArray operator&(const MyArray& left, const MyArray& right) {
            MyArray result;

            auto used = new bool[right.size];
            for (int i = 0; i < right.size; i++) {
                used[i] = false;
            }

            for (int i = 0; i < left.size; i++) {
                for (int j = 0; j < right.size; j++) {
                    if (!used[j] && left.data[i] == right.data[j]) {
                        result.append(left.data[i]);
                        used[j] = true;
                        break;
                    }
                }
            }

            delete[] used;
            return result;
        }


        MyArray& operator=(const MyArray &other);

        int equalize(const int* arr, int other_size);
        int append(int value);
        void clear();


        int get_size() const;
        void  print() const ;


};

void input_array(MyArray & array);

#endif
