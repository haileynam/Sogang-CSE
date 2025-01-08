class Str
{
    private: 
        char *str;
        int len;
    
    public:
        Str(int leng);
        Str(char *neyong);
        ~Str();
        int length();
        char *contents();
        int compare(class Str &a);
        int compare(char *a);
        void operator = (char *a);
        void operator = (class Str &a);
};