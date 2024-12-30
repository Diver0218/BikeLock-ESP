class iMotor
{   
    public:
        iMotor();
        virtual ~iMotor();
        virtual void rotate(int degree) = 0;
};