#ifndef SELF_MADE_STRING_H
#define SELF_MADE_STRING_H

#include <string>

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

    	String operator()(int start, int end) const;
        String& operator=(const char* str);
        String& operator=(const String& other);
    	char& operator[](int index);
    	const char& operator[](int index) const;

        int get_length() const;
        void clear() ;
        bool is_empty() const;

        friend bool operator!(const String& s);

        friend std::ostream& operator<<(std::ostream& out, const String& s);
        friend std::istream& operator>>(std::istream& in, String& s);

        friend bool operator==(const String& s1, const String& s2) {
                return str_equal(s1.data, s2.data);
        }
};

#endif
