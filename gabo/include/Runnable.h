#ifndef RUNNABLE_H
#define RUNNABLE_H


class Runnable
{
    public:
        Runnable();
        virtual ~Runnable();

        void run();
    protected:
        bool child;

        virtual void _run() = 0;
    private:
};

#endif // RUNNABLE_H
