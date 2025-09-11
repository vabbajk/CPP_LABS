#ifndef SELF_MADE_STRING_H
#define SELF_MADE_STRING_H

#include <string>


using namespace std;

class String {

    private:

    	char* data;
    	int length;

        static int str_length(const char* s);
    	static void str_copy(char* dest, const char* src);
    	static bool str_equal(const char* s1, const char* s2);

    public:

    	String();
    	explicit String(const char* s);
    	String(const String& other);
    	~String();


    	friend bool operator!(const String& s);
    	String operator()(int start, int end) const;
    	char& operator[](int index);
    	const char& operator[](int index) const;
    	friend bool operator==(const String& s1, const String& s2);

        friend ostream& operator<<(ostream& out, const String& s);
        friend istream& operator>>(istream& in, String& s);

        String& operator=(const char* str);
        String& operator=(const String& other);
        int get_length() const;
        void clear() ;
        bool is_empty() const;
};

#endif
